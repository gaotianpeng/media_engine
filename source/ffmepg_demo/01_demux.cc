#include <stdio.h>
#include "gtest/gtest.h"

extern "C" {
#include "libavutil/log.h"
#include "libavformat/avformat.h"
}

#define ADTS_HEADER_LEN  7;

const int sampling_frequencies[] = {
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

int adts_header(char* const p_adts_header, const int data_length,
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
		printf("not support sample rate:%d\n", samplerate);
		return -1;
	}

	p_adts_header[0] = 0xff;
	p_adts_header[1] = 0xf0;
	p_adts_header[1] |= (0 << 3);
	p_adts_header[1] |= (0 << 1);
	p_adts_header[1] |= 1;

	p_adts_header[2] = (profile) << 6;
	p_adts_header[2] |= (sampling_frequency_index & 0x0f) << 2;
	p_adts_header[2] |= (0 << 1);
	p_adts_header[2] |= (channels & 0x04) >> 2;

	p_adts_header[3] = (channels & 0x03) << 6;
	p_adts_header[3] |= (0 << 5);
	p_adts_header[3] |= (0 << 4);
	p_adts_header[3] |= (0 << 3);
	p_adts_header[3] |= (0 << 2);
	p_adts_header[3] |= ((adtsLen & 0x1800) >> 11);

	p_adts_header[4] = (uint8_t)((adtsLen & 0x7f8) >> 3);
	p_adts_header[5] = (uint8_t)((adtsLen & 0x7) << 5);
	p_adts_header[5] |= 0x1f;                          
	p_adts_header[6] = 0xfc;
	
	return 0;
}


//#define DECODE 1
#ifdef DECODE 
TEST(FFmpegDecode, Decode) {
	char* in_file = "d:/av_data/720_ue4.mp4";
	char* out_h264_file = "d:/av_data/out_720_ue4.h264";
	char* out_aac_file = "d:/av_data/out_720_ue4.aac";

	FILE* aac_fd = NULL;
	FILE* h264_fd = NULL;

	aac_fd = fopen(out_aac_file, "wb");
	ASSERT_TRUE(aac_fd != NULL);

	h264_fd = fopen(out_h264_file, "wb");
	ASSERT_TRUE(h264_fd != NULL);

	AVFormatContext* in_fmt_ctx = NULL;
	int video_idx = -1;
	int audio_idx = -1;
	AVPacket* pkt = NULL;
	int ret = 0;

	in_fmt_ctx = avformat_alloc_context();
	ASSERT_TRUE(in_fmt_ctx != NULL);

	ret = avformat_open_input(&in_fmt_ctx, in_file, NULL, NULL);
	ASSERT_TRUE(ret >= 0);

	video_idx = av_find_best_stream(in_fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
	ASSERT_TRUE(video_idx >= 0);

	audio_idx = av_find_best_stream(in_fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
	ASSERT_TRUE(audio_idx >= 0);

	const AVBitStreamFilter* bsfilter = av_bsf_get_by_name("h264_mp4toannexb");
	ASSERT_TRUE(bsfilter != NULL);

	AVBSFContext* bsf_ctx = NULL;
	ret = av_bsf_alloc(bsfilter, &bsf_ctx);
	ASSERT_TRUE(ret >= 0);

	ret = avcodec_parameters_copy(bsf_ctx->par_in, in_fmt_ctx->streams[video_idx]->codecpar);
	ASSERT_TRUE(ret >= 0);

	ret = av_bsf_init(bsf_ctx);
	ASSERT_TRUE(ret >= 0);

	pkt = av_packet_alloc();
	ASSERT_TRUE(pkt != NULL);
	av_init_packet(pkt);

	while (1) {
		ret = av_read_frame(in_fmt_ctx, pkt);
		if (ret < 0) {
			break;
		}

		if (pkt->stream_index == video_idx) {
			ret = av_bsf_send_packet(bsf_ctx, pkt);
			if (ret < 0) {
				av_packet_unref(pkt);
				continue;
			}
			while (1) {
				ret = av_bsf_receive_packet(bsf_ctx, pkt);
				if (ret != 0) {
					break;
				}
				int size = fwrite(pkt->data, 1, pkt->size, h264_fd);
				if (size != pkt->size) {
					av_log(NULL, AV_LOG_DEBUG, "h264 warning, length of writted data isn't equal pkt->size(%d, %d)\n",
						size,
						pkt->size);
				}

				av_packet_unref(pkt);
			}
		}
		else if (pkt->stream_index == audio_idx) {
			char adts_header_buf[7] = { 0 };
			adts_header(adts_header_buf, pkt->size,
					in_fmt_ctx->streams[audio_idx]->codecpar->profile,
					in_fmt_ctx->streams[audio_idx]->codecpar->sample_rate,
					in_fmt_ctx->streams[audio_idx]->codecpar->channels);
			fwrite(adts_header_buf, 1, 7, aac_fd);
			int size = fwrite(pkt->data, 1, pkt->size, aac_fd);
			if (size != pkt->size) {
				av_log(NULL, AV_LOG_DEBUG, "aac warning, length of writted data isn't equal pkt->size(%d, %d)\n",
					size,
					pkt->size);
			}
			av_packet_unref(pkt);
		}
		else {
			av_packet_unref(pkt);
		}
	}


	if (h264_fd) {
		fclose(h264_fd);
	}

	if (aac_fd) {
		fclose(aac_fd);
	}

	if (pkt) {
		av_packet_free(&pkt);
	}

	avformat_close_input(&in_fmt_ctx);
	av_bsf_free(&bsf_ctx);
}

#endif // DECODE

