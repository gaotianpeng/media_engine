#include <stdio.h>
#include "gtest/gtest.h"

extern "C" {
#include "libavutil/log.h"
#include "libavformat/avio.h"
#include "libavformat/avformat.h"
}

TEST(FFmpegExtractH264, ExtractH264) {
	AVFormatContext* in_fmt_ctx = NULL;
	int video_idx = -1;
	AVPacket* pkt = NULL;
	
	FILE* out_fp = fopen("d:/av_data/720_ue4.h264", "wb");
	ASSERT_TRUE(out_fp != NULL);

	in_fmt_ctx = avformat_alloc_context();
	ASSERT_TRUE(in_fmt_ctx != NULL);

	int ret = -1;
	ret = avformat_open_input(&in_fmt_ctx, "d:/av_data/720_ue4.mp4", NULL, NULL);
	ASSERT_TRUE(ret == 0);

	ret = -1;
	ret = avformat_find_stream_info(in_fmt_ctx, NULL);
	ASSERT_TRUE(ret >= 0);

	video_idx = av_find_best_stream(in_fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
	ASSERT_TRUE(video_idx >= 0);

	pkt = av_packet_alloc();
	av_init_packet(pkt);

	const AVBitStreamFilter* bsfilter = av_bsf_get_by_name("h264_mp4toannexb");
	AVBSFContext* bsf_ctx = NULL;
	av_bsf_alloc(bsfilter, &bsf_ctx);
	avcodec_parameters_copy(bsf_ctx->par_in, in_fmt_ctx->streams[video_idx]->codecpar);
	av_bsf_init(bsf_ctx);

	int file_end = 0;
	while (0 == file_end) {
		if (ret = av_read_frame(in_fmt_ctx, pkt) < 0) {
			file_end = 1;
			printf("read file end: ret:%d\n", ret);
		}

		if (ret == 0 && pkt->stream_index == video_idx) {
#if 1
			int input_size = pkt->size;
			int out_pkt_cnt = 0;
			if (av_bsf_send_packet(bsf_ctx, pkt) != 0) {
				av_packet_unref(pkt);
				continue;
			}
			av_packet_unref(pkt);
			while (av_bsf_receive_packet(bsf_ctx, pkt) == 0) {
				out_pkt_cnt++;
				int size = fwrite(pkt->data, 1, pkt->size, out_fp);
				if (size != pkt->size) {
					printf("fwrite failed-> write:%u, pkt_size:%u\n", size, pkt->size);
				}
				av_packet_unref(pkt);
			}

			if (out_pkt_cnt >= 2) {
				printf("cur pkt(size:%d) only get 1 out pkt, it get %d pkts\n",
					input_size, out_pkt_cnt);
			}
#else		
			// TS流可直接写入
			int size = fwrite(pkt->data, 1, pkt->size, out_fp);
			if (size != pkt->size) {
				printf("fwrite failed-> write:%u, pkt_size:%u\n", size, pkt->size);
			}
			av_packet_unref(pkt);
#endif 
		}
		else {
			if (ret == 0) {
				av_packet_unref(pkt);
			}
		}
	}

	if (out_fp) {
		fclose(out_fp);
	}

	if (bsf_ctx) {
		av_bsf_free(&bsf_ctx);
	}

	if (pkt) {
		av_packet_free(&pkt);
	}

	if (in_fmt_ctx) {
		avformat_close_input(&in_fmt_ctx);
	}
}