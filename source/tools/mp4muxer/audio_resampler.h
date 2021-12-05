#ifndef MP4_MUXER_AUDIO_RESAMPLE_H
#define MP4_MUXER_AUDIO_RESAMPLE_H

extern "C" {
#include "libavcodec/avcodec.h"
#include "libswresample/swresample.h"
#include "libavformat/avformat.h"
}

AVFrame* AllocFltpPcmFrame(int channels, int nb_samples);
void FreePcmFrame(AVFrame* frame);

class AudioResampler {
public:
    AudioResampler();
    ~AudioResampler();

public:
    int InitFromS16ToFLTP(int in_channels, int in_sample_rate, int out_channels, int out_sample_rate);
    int ResampleFromS16ToFLTP(uint8_t *in_data, AVFrame *out_frame);
    void DeInit();

private:
    int in_channels_;
    int in_sample_rate_;
    int out_channels_;
    int out_sample_rate_;
    SwrContext* swr_ctx_ = NULL;
};

#endif // MP4_MUXER_AUDIO_RESAMPLE_H