#ifndef MP4_MUXER_MUX_H
#define MP4_MUXER_MUX_H

#include <iostream>
extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
}

class Muxer {
public:
    Muxer();
    ~Muxer();

public:
    int Init(const char* url);
    void DeInit();
    int AddStream(AVCodecContext* codec_ctx);

    int SendHeader();
    int SendPacket(AVPacket* pkt);
    int SendTailer();

    int Open();
    int GetAudioStreamIndex();
    int GetVideoStreamIndex();

private:
    AVFormatContext* fmt_ctx_ = NULL;
    std::string url_;

    AVCodecContext* audio_codec_ctx_ = NULL;
    AVStream* audio_stream_ = NULL;
    AVCodecContext* video_codec_ctx_ = NULL;
    AVStream* video_stream_ = NULL;

    int audio_idx_ = -1;
    int video_idx_ = -1;
};

#endif // MP4_MUXER_MUX_H
