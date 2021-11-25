#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <gtest/gtest.h>

extern "C" {
#include "libavformat/avio.h"
#include "libavformat/avformat.h"
#include "libavutil/log.h"
}

using namespace std;

#define ADTS_HEADER_LEN 7

static const int sampling_frequencies[] = {
	96000,  // 0x0
	88200,  // 0x1
	64000,  // 0x2
	48000,  // 0x3
	44100,  // 0x4
	32000,  // 0x5
	24000,  // 0x6
	22050,  // 0x7
	16000,  // 0x8
	12000,  // 0x9
	11025,  // 0xa
	8000   // 0xb
};

static int adts_header(char* const p_adts_header, const int data_length,
	const int profile, const int samplerate,
	const int channels) {

	int sampling_frequency_index = 3;
	int adtsLen = data_length + 7;

	int frequencies_size = sizeof(sampling_frequencies) / sizeof(sampling_frequencies[0]);
	int i = 0;
	for (i = 0; i < frequencies_size; i++)
	{
		if (sampling_frequencies[i] == samplerate)
		{
			sampling_frequency_index = i;
			break;
		}
	}
	if (i >= frequencies_size)
	{
		printf("unsupport samplerate:%d\n", samplerate);
		return -1;
	}

	p_adts_header[0] = 0xff;         //syncword:0xfff                          高8bits
	p_adts_header[1] = 0xf0;         //syncword:0xfff                          低4bits
	p_adts_header[1] |= (0 << 3);    //MPEG Version:0 for MPEG-4,1 for MPEG-2  1bit
	p_adts_header[1] |= (0 << 1);    //Layer:0                                 2bits
	p_adts_header[1] |= 1;           //protection absent:1                     1bit

	p_adts_header[2] = (profile) << 6;            //profile:profile               2bits
	p_adts_header[2] |= (sampling_frequency_index & 0x0f) << 2; //sampling frequency index:sampling_frequency_index  4bits
	p_adts_header[2] |= (0 << 1);             //private bit:0                   1bit
	p_adts_header[2] |= (channels & 0x04) >> 2; //channel configuration:channels  high 1bit

	p_adts_header[3] = (channels & 0x03) << 6; //channel configuration:channels low 2bits
	p_adts_header[3] |= (0 << 5);               //original：0                1bit
	p_adts_header[3] |= (0 << 4);               //home：0                    1bit
	p_adts_header[3] |= (0 << 3);               //copyright id bit：0        1bit
	p_adts_header[3] |= (0 << 2);               //copyright id start：0      1bit
	p_adts_header[3] |= ((adtsLen & 0x1800) >> 11);           //frame length：value   high 2bits

	p_adts_header[4] = (uint8_t)((adtsLen & 0x7f8) >> 3);     //frame length:value    mid 8bits
	p_adts_header[5] = (uint8_t)((adtsLen & 0x7) << 5);       //frame length:value    low 3bits
	p_adts_header[5] |= 0x1f;                                 //buffer fullness:0x7ff high 5bits
	p_adts_header[6] = 0xfc;      //‭11111100‬       //buffer fullness:0x7ff 低6bits

	return 0;
}

TEST(FFmpegExtractAAC, ExtractAAC) {
	char* in_file = "d:/av_data/720_ue4.mp4";
	char* out_file = "d:/av_data/720_ue4_adts.aac";
	FILE* aac_fp = NULL;
	int audio_idx = -1;

	AVFormatContext* in_fmt_ctx = NULL;
	AVPacket pkt;

	av_log_set_level(AV_LOG_DEBUG);
	av_log(NULL, AV_LOG_DEBUG, "Extract aac\n ");
	
	aac_fp = fopen(out_file, "wb");
	ASSERT_TRUE(aac_fp != NULL);

	// open file
	int ret = -1;
	ret = avformat_open_input(&in_fmt_ctx, in_file, NULL, NULL);
	ASSERT_TRUE(ret >= 0);

	// get stream info
	ret = avformat_find_stream_info(in_fmt_ctx, NULL);
	ASSERT_TRUE(ret >= 0);

	av_dump_format(in_fmt_ctx, 0, in_file, 0);

	av_init_packet(&pkt);
	audio_idx = av_find_best_stream(in_fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
	ASSERT_TRUE(audio_idx >= 0);

	cout << "audio profile: " << in_fmt_ctx->streams[audio_idx]->codecpar->profile
		<< ", FF_PROFILE_AAC_LOW:" << FF_PROFILE_AAC_LOW;

	ASSERT_TRUE(in_fmt_ctx->streams[audio_idx]->codecpar->codec_id
			== AV_CODEC_ID_AAC);
	// read and write aac
	while (av_read_frame(in_fmt_ctx, &pkt) >= 0) {
		if (pkt.stream_index == audio_idx) {
			char adts_header_buf[7] = { 0 };
			adts_header(adts_header_buf, pkt.size,
				in_fmt_ctx->streams[audio_idx]->codecpar->profile,
				in_fmt_ctx->streams[audio_idx]->codecpar->sample_rate,
				in_fmt_ctx->streams[audio_idx]->codecpar->channels);
			fwrite(adts_header_buf, 1, 7, aac_fp);

			int len = fwrite(pkt.data, 1, pkt.size, aac_fp);
			if (len != pkt.size) {
				av_log(NULL, AV_LOG_DEBUG, "warning, length of writed data isn't equal pkt.size(%d, %d)\n",
					len,
					pkt.size);
			}
		}
		av_packet_unref(&pkt);
	}

	if (in_fmt_ctx) {
		avformat_close_input(&in_fmt_ctx);
	}
	fclose(aac_fp);
}