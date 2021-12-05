#include "mux.h"
#include "glog/logging.h"

Muxer::Muxer() {
}

Muxer::~Muxer() {
}

int Muxer::Init(const char* url) {
    int ret = avformat_alloc_output_context2(&fmt_ctx_, NULL,
            NULL, url);

    if (ret < 0) {
        LOG(INFO) << "avformat_alloc_output_context2 failed\n";
        return -1;
    }

    url_ = url;
    return 0;
}

void Muxer::DeInit() {
    if (fmt_ctx_) {
        avformat_close_input(&fmt_ctx_);
    }

    url_ = "";
    audio_codec_ctx_ = NULL;
    audio_stream_ = NULL;
    audio_idx_ = -1;

    video_codec_ctx_ = NULL;
    video_stream_ = NULL;
    video_idx_ = -1;
}

int Muxer::AddStream(AVCodecContext* codec_ctx) {
    if (!fmt_ctx_) {
        LOG(INFO) << "fmt ctx is NULL\n";
        return -1;
    }

    if (!codec_ctx) {
        LOG(INFO) << "codec ctx is NULL\n";
        return -1;
    }

    AVStream* st = avformat_new_stream(fmt_ctx_, NULL);
    if (!st) {
        LOG(ERROR) << "avformat_new_stream failed\n";
        return -1;
    }

    avcodec_parameters_from_context(st->codecpar, codec_ctx);
    av_dump_format(fmt_ctx_, 0, url_.c_str(), 1);

    if (codec_ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
        audio_codec_ctx_ = codec_ctx;
        audio_stream_ = st;
        audio_idx_ = st->index;
    } else if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO) {
        video_codec_ctx_ = codec_ctx;
        video_stream_ = st;
        video_idx_ = st->index;
    }

    return 0;
}

int Muxer::SendHeader() {
    if (!fmt_ctx_) {
        LOG(ERROR) << "fmt ctx is NULL\n";
        return -1;
    }

    int ret = avformat_write_header(fmt_ctx_, NULL);
    if (ret < 0) {
        LOG(ERROR) << "avformat_write_header failed\n";
        return -1;
    }

    return 0;
}

int Muxer::SendPacket(AVPacket* pkt) {
    int stream_idx = pkt->stream_index;
    LOG(INFO) << "stream idx " << stream_idx << " , pkts: " << pkt->pts;

    if (!pkt || pkt->size <= 0 || !pkt->data) {
        LOG(ERROR) << "packet is null\n";
        if (pkt) {
            av_packet_free(&pkt);
        }

        return -1;
    }

    AVRational src_time_base;
    AVRational dst_time_base;
    if (video_stream_ && video_codec_ctx_ && stream_idx == video_idx_) {
        src_time_base = video_codec_ctx_->time_base;
        dst_time_base = video_stream_->time_base;
    } else if (audio_stream_ && audio_codec_ctx_ && stream_idx == audio_idx_) {
        src_time_base = audio_codec_ctx_->time_base;
        dst_time_base = audio_stream_->time_base;
    }

    pkt->pts = av_rescale_q(pkt->pts, src_time_base, dst_time_base);
    pkt->dts = av_rescale_q(pkt->dts, src_time_base, dst_time_base);
    pkt->duration = av_rescale_q(pkt->duration, src_time_base, dst_time_base);

    int ret = 0;
    ret = av_interleaved_write_frame(fmt_ctx_, pkt);

    av_packet_free(&pkt);
    if (ret == 0) {
        return 0;
    } else {
        LOG(ERROR) << "av_write_frame failed\n";
        return -1;
    }
}

int Muxer::SendTailer() {
    if (!fmt_ctx_) {
        LOG(ERROR) << "fmt ctx is NULL\n";
        return -1;
    }
    int ret = av_write_trailer(fmt_ctx_);
    if (ret != 0) {
        LOG(ERROR) << "av_write_tailer failed\n";
        return -1;
    }
    return 0;
}

int Muxer::Open() {
    int ret = avio_open(&fmt_ctx_->pb, url_.c_str(), AVIO_FLAG_WRITE);
    if (ret < 0) {
        LOG(ERROR) << "avio_open " << url_.c_str() << "failed\n";
        return -1;
    }

    return 0;
}

int Muxer::GetAudioStreamIndex() {
    return audio_idx_;
}

int Muxer::GetVideoStreamIndex() {
    return video_idx_;
}