#include "video_encoder.h"
#include "glog/logging.h"

extern "C" {
#include "libavutil/imgutils.h"
}

VideoEncoder::VideoEncoder() {
}


VideoEncoder::~VideoEncoder() {
    if (codec_ctx_) {
        DeInit();
    }
}

int VideoEncoder::InitH264(int width, int height, int fps, int bit_rate) {
    width_ = width;
    height_ = height;
    fps_ = fps;
    bit_rate_ = bit_rate;

    AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!codec) {
        LOG(ERROR) << "avcodec_find_encoder AV_CODEC_ID_H264 failed\n";
        return -1;
    }

    codec_ctx_ = avcodec_alloc_context3(codec);
    if (!codec_ctx_) {
        LOG(ERROR) << "avcodec_alloc_codectx3 AV_CODEC_ID_H264 failed\n";
        return -1;
    }
 
    codec_ctx_->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    codec_ctx_->bit_rate = bit_rate_;
    codec_ctx_->width = width_;
    codec_ctx_->height = height_;
    codec_ctx_->framerate.num = fps_;
    codec_ctx_->framerate.den = 1;

    codec_ctx_->time_base.num = 1;
    codec_ctx_->time_base.den = 1000000; // 微秒
    codec_ctx_->gop_size = fps_;
    codec_ctx_->max_b_frames = 0;
    codec_ctx_->pix_fmt = AV_PIX_FMT_YUV420P;

    int ret = avcodec_open2(codec_ctx_, NULL, &dict_);
    if (ret < 0) {
        LOG(ERROR) << "avcodec_open2 failed\n";
        return -1;
    }

    frame_ = av_frame_alloc();
    if (!frame_) {
        LOG(ERROR) << "av_frame_alloc failed\n";
        return -1;
    }

    frame_->width = width_;
    frame_->height = height_;
    frame_->format = codec_ctx_->pix_fmt;

    LOG(INFO) << "Inith264 success\n";

    return 0;
}

void VideoEncoder::DeInit() {
    if (codec_ctx_) {
        avcodec_free_context(&codec_ctx_);
    }
    if (frame_) {
        av_frame_free(&frame_);
    }
    if (dict_) {
        av_dict_free(&dict_);
    }
}

AVPacket* VideoEncoder::Encode(uint8_t* yuv_data, int yuv_size, int stream_idx,
        int64_t pts, int64_t time_base) {
    if (!codec_ctx_) {
        LOG(ERROR) <<"codec_ctx_ null\n";
        return NULL;
    }

    int ret = 0;
    AVRational tb;
    tb.num = 1, tb.den = time_base;
    pts = av_rescale_q(pts, tb, codec_ctx_->time_base);
    frame_->pts = pts;
    if (yuv_data) {
        int ret_size = av_image_fill_arrays(frame_->data, frame_->linesize,
                yuv_data, (AVPixelFormat)frame_->format,
                frame_->width, frame_->height, 1);
        if (ret_size != yuv_size) {
            LOG(ERROR) << "ret_size: " << ret_size << " != yuv_size: " << yuv_size
                    << " -> failed\n";
            return NULL; 
        }

        ret = avcodec_send_frame(codec_ctx_, frame_);
    } else {
        ret = avcodec_send_frame(codec_ctx_, NULL);
    }

    if (ret != 0) {
        LOG(INFO) << "avcodec_send_frame failed \n";
        return NULL;
    }

    AVPacket* pkt = av_packet_alloc();
    ret = avcodec_receive_packet(codec_ctx_, pkt);
    if (ret < 0) {
        LOG(INFO) << "h264 avcodec_receive_packet failed\n";
        av_packet_free(&pkt);
        return NULL;
    }

    pkt->stream_index = stream_idx;
    return pkt;
}
    
int VideoEncoder::Encode(uint8_t* yuv_data, int yuv_size, int stream_idx,
        int64_t pts, int64_t time_base, std::vector<AVPacket*>& pkts) {
    if (!codec_ctx_) {
        LOG(ERROR) << "codec_ctx null\n";
        return -1;
    }

    int ret = 0;

    AVRational tb;
    tb.num = 1, tb.den = time_base;
    pts = av_rescale_q(pts, tb, codec_ctx_->time_base);
    frame_->pts = pts;
    if (yuv_data) {
        int ret_size = av_image_fill_arrays(frame_->data, frame_->linesize,
                yuv_data, (AVPixelFormat)frame_->format,
                frame_->width, frame_->height, 1);
        if (ret_size != yuv_size) {
            LOG(ERROR) << "ret_size " << ret_size << " != yuv_size: " << yuv_size
                << " ->failed \n";
            return -1;
        }

        ret = avcodec_send_frame(codec_ctx_, frame_);
    } else {
        ret = avcodec_send_frame(codec_ctx_, NULL);
    }

    if (ret != 0) {
        LOG(ERROR) << "avcodec_send_frame failed\n";
        return -1;
    }

    while (true) {
        AVPacket* pkt = av_packet_alloc();
        ret = avcodec_receive_packet(codec_ctx_, pkt);
        pkt->stream_index = stream_idx;
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            ret = 0;
            av_packet_free(&pkt);
            break;
        } else if (ret < 0) {
            LOG(ERROR) << "h264 avcodec_receive_packed failed\n";
            av_packet_free(&pkt);
            ret = -1;
        }
        LOG(INFO) << "h264 pts: " << pkt->pts << "\n";
        pkts.push_back(pkt);
    }

    return ret;
}

AVCodecContext* VideoEncoder::GetCodecContext() {
    return codec_ctx_;
}