#ifndef MP4_MUXER_VIDEO_ENCODER_H
#define MP4_MUXER_VIDEO_ENCODER_H

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
}

#include <vector>

class VideoEncoder {
public:
    VideoEncoder();
    ~VideoEncoder();

public:
    int InitH264(int width, int height, int fps, int bit_rate);
    void DeInit();
    AVPacket* Encode(uint8_t* yuv_data, int yuv_size, int stream_idx,
            int64_t pts, int64_t time_base);
    int Encode(uint8_t* yuv_data, int yuv_size, int stream_idx,
            int64_t pts, int64_t time_base, std::vector<AVPacket*>& pkts);

    AVCodecContext* GetCodecContext();

private:
    int width_ = 0;
    int height_ = 0;
    int fps_ = 25;
    int bit_rate_ = 500*1024;
    int64_t pts_ = 0;
    AVCodecContext* codec_ctx_ = NULL;
    AVFrame* frame_ = NULL;
    AVDictionary* dict_ = NULL;
};

#endif // MP4_MUXER_VIDEO_ENCODER_H
