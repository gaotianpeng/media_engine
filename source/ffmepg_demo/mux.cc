#include <gtest/gtest.h>
#include <iostream>
extern "C" {
#include <libavformat/avformat.h>
}

using namespace std;

TEST(FFmpegMux, MuxTest) {
	const char* file_name = "d:/av_data/believe.mp4";
	AVFormatContext* fmt_ctx = NULL;
    int video_idx = -1;
    int audio_idx = -1;
    int ret = avformat_open_input(&fmt_ctx, file_name, NULL, NULL);
    EXPECT_TRUE(fmt_ctx != NULL);
    EXPECT_TRUE(ret >= 0);

    cout << "=================before dump ======================" << endl;
    av_dump_format(fmt_ctx, 0, file_name, 0);
    cout << "=================after dump ======================" << endl;

    cout << "media name: " << fmt_ctx->url << endl;
    cout << "stream number: " << fmt_ctx->nb_streams << endl;
    cout << "media average ratio: " << fmt_ctx->bit_rate << endl;

    ret = avformat_find_stream_info(fmt_ctx, NULL);
    EXPECT_TRUE(ret >= 0);
    cout << "media average ratio: " << fmt_ctx->bit_rate/1024 << endl;
    int seconds = fmt_ctx->duration / AV_TIME_BASE;
    cout << "total duration: " << (int)(seconds / 3600) << ":"
         << (int)(seconds / 60) << ":" << seconds % 60 << endl;


    cout << "total stream number: " << fmt_ctx->nb_streams << endl << endl;
    for (unsigned int i = 0; i < fmt_ctx->nb_streams; ++i) {
        AVStream* stream = fmt_ctx->streams[i];
        if (stream->codecpar->codec_type  == AVMEDIA_TYPE_AUDIO) {
            cout << "audio index: " << stream->index << endl;
            cout << "audio sample rate " << stream->codecpar->sample_rate << endl;
            if (AV_SAMPLE_FMT_FLTP == stream->codecpar->format) {
                cout << "sample format:AV_SAMPLE_FMT_FLTP" << endl;
            } else if (AV_SAMPLE_FMT_S16P == stream->codecpar->format) {
                cout << "sample format:AV_SAMPLE_FMT_S16P\n";
            }

            cout << "audio channel number " << stream->codecpar->channels
                 << endl;
            cout << "audio stream timebase " << stream->time_base.num << "/"
                 << stream->time_base.den << endl;
            cout << endl;
        }

        if (stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            cout << "video index: " << stream->index << endl;
            cout << "video fps " << av_q2d(stream->avg_frame_rate) << endl;
            if (AV_CODEC_ID_MPEG4 == stream->codecpar->codec_id) {
                cout << "video codec:MPEG4\n";
            } 
            else if (AV_CODEC_ID_H264 == stream->codecpar->codec_id) {
                cout << "video codec:H264\n";
            } else {
              cout << "video codec_id: " << stream->codecpar->codec_id << endl;
            }

            cout << "video time base " << stream->time_base.num << "/"
                 << stream->time_base.den << endl;
            cout << "video duration in seconds "
                 << stream->duration * av_q2d(stream->time_base) << endl;
            cout << "video width: " << stream->codecpar->width << ",height "
                 << stream->codecpar->height << endl; 
            cout << endl;
        }
    }


    if (fmt_ctx) {
        avformat_close_input(&fmt_ctx);
    }


}

