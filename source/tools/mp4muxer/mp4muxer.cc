extern "C" {
#include "libavcodec/avcodec.h"
#include "libavutil/avutil.h"
#include "libavformat/avformat.h"
#include "libswresample/swresample.h"
}

#include <iostream>
#include "glog/logging.h"
#include "video_encoder.h"
#include "audio_encoder.h"
#include "audio_resampler.h"
#include "mux.h"
using namespace std;

#define YUV_WIDTH 720
#define YUV_HEIGHT 576
#define YUV_FPS  25

#define VIDEO_BIT_RATE 500*1024

#define PCM_SAMPLE_FORMAT AV_SAMPLE_FMT_S16
#define PCM_SAMPLE_RATE 44100
#define PCM_CHANNELS 2

#define AUDIO_BIT_RATE 128*1024

#define AUDIO_TIME_BASE 1000000
#define VIDEO_TIME_BASE 1000000

//ffmpeg -i sound_in_sync_test.mp4 -pix_fmt yuv420p 720x576_yuv420p.yuv
//ffmpeg -i sound_in_sync_test.mp4 -vn -ar 44100 -ac 2 -f s16le 44100_2_s16le.pcm

int main(int argc, char* argv[]) {
	google::InitGoogleLogging(argv[0]);

	FLAGS_logtostderr = 1;
	FLAGS_colorlogtostderr = true;
	LOG(INFO) << "mp4muxer starting...";
	char* in_yuv_name = "d:/av_data/720x576_yuv420p.yuv";
	char* in_pcm_name = "d:/av_data/44100_2_s16le.pcm";
	char* out_mp4_name = "d:/av_data/mux_out.mp4";

	FILE* in_yuv_fp= NULL;
	FILE* in_pcm_fp = NULL;
	
	in_yuv_fp = fopen(in_yuv_name, "rb");
	in_pcm_fp = fopen(in_pcm_name, "rb");

	if (!in_yuv_fp) {
		LOG(ERROR) << "Failed to open " << in_yuv_name << "file\n";
		return -1;
	}

	if (!in_pcm_fp) {
		LOG(ERROR) << "Failed to open " << in_pcm_name << "file\n";
		return -1;
	}

	int ret = 0;
	int yuv_width = YUV_WIDTH;
	int yuv_height = YUV_HEIGHT;
	int yuv_fps = YUV_FPS;

	int video_bit_rate = VIDEO_BIT_RATE;
	VideoEncoder video_encoder;
	ret = video_encoder.InitH264(yuv_width, yuv_height, yuv_fps, video_bit_rate);
	if (ret < 0) {
		LOG(ERROR) << "video_encoder.InitH264 failed\n";
		return -1;
	}

	int y_frame_size = yuv_width * yuv_height;
	int u_frame_size = yuv_width * yuv_height / 4;
	int v_frame_size = yuv_width * yuv_height / 4;
	int yuv_frame_size = y_frame_size + u_frame_size + v_frame_size;
	uint8_t* yuv_frame_buf = (uint8_t*)malloc(yuv_frame_size);
	if (!yuv_frame_buf) {
		LOG(ERROR) << "malloc(yuv_frame_size) failed \n";
		return -1;
	}

	int pcm_channels = PCM_CHANNELS;
	int pcm_sample_rate = PCM_SAMPLE_RATE;
	int pcm_sample_format = PCM_SAMPLE_FORMAT;
	int audio_bit_rate = AUDIO_BIT_RATE;
	AudioEncoder audio_encoder;
	ret = audio_encoder.InitAAC(pcm_channels, pcm_sample_rate, audio_bit_rate);
	if (ret < 0) {
		LOG(ERROR) << "audio_encoder.InitAAC failed\n";
		return -1;
	}

	int pcm_frame_size = av_get_bytes_per_sample((AVSampleFormat)pcm_sample_format)
				* pcm_channels * audio_encoder.GetFrameSize();
	if (pcm_frame_size <= 0) {
		LOG(ERROR) << "pcm_frame_size <= 0\n";
		return -1;
	}

	uint8_t* pcm_frame_buf = (uint8_t*)malloc(pcm_frame_size);
	if (!pcm_frame_buf) {
		LOG(ERROR) << "malloc(pcm_frame_size) \n";
		return -1;
	}

	AudioResampler audio_resample;
	ret = audio_resample.InitFromS16ToFLTP(pcm_channels, pcm_sample_rate,
				audio_encoder.GetChannels(), audio_encoder.GetSampleRate());
	if (ret < 0) {
		LOG(ERROR) << "audio_resampler.InitFromS16ToFLTP failed\n";
		return -1;
	}

	Muxer mp4_muxer;
	ret = mp4_muxer.Init(out_mp4_name);
	if (ret < 0) {
		LOG(ERROR) << "mp4_muxer.Init failed\n";
		return -1;
	}

	ret = mp4_muxer.AddStream(video_encoder.GetCodecContext());
	if (ret < 0) {
		LOG(ERROR) << "mp4_muxer..AddStream video failed\n";
		return -1;
	}

	ret = mp4_muxer.AddStream(audio_encoder.GetCodecContext());
	if (ret < 0) {
		LOG(ERROR) << "mp4_muxer.AddStream audio failed\n";
		return -1;
	}

	ret = mp4_muxer.Open();
	if (ret < 0) {
		LOG(ERROR) << "mp4_muxer.Open failed\n";
		return -1;
	}

	ret = mp4_muxer.SendHeader();
	if (ret < 0) {
		LOG(ERROR) << "mp4_muxer.SendHeader failed\n";
		return -1;
	}

	int64_t audio_time_base = AUDIO_TIME_BASE;
	int64_t video_time_base = VIDEO_TIME_BASE;
	double audio_pts = 0;
	double video_pts = 0;
	double audio_frame_duration = 1.0 * audio_encoder.GetFrameSize() /
			pcm_sample_rate * audio_time_base;
	double video_frame_duration = 1.0 / yuv_fps * video_time_base;
	int audio_finish = 0;
	int video_finish = 0;
	int read_len = 0;
	AVPacket* pkt = NULL;
	std::vector<AVPacket*> pkts;
	int audio_idx = mp4_muxer.GetAudioStreamIndex();
	int video_idx = mp4_muxer.GetVideoStreamIndex();

	while (true) {
		if (audio_finish && video_finish) {
			break;
		}

		LOG(INFO) << "apts: " << audio_pts/1000 << ", vpts: " << video_pts/1000;
		if ((video_finish != 1 && audio_pts > video_pts) ||
				(video_finish != 1 && audio_finish == 1)) {
			read_len = fread(yuv_frame_buf, 1, yuv_frame_size, in_yuv_fp);
			if (read_len < yuv_frame_size) {
				video_finish = 1;
				LOG(INFO) << "fread yuv_frame_buf finish\n";
			}

			if (video_finish != 1) {
				ret = video_encoder.Encode(yuv_frame_buf, yuv_frame_size,
							video_idx, video_pts, video_time_base,
							pkts);
			} else {
				LOG(INFO) << "flush video encoder\n";
				ret = video_encoder.Encode(NULL, 0, video_idx, video_pts,
					video_time_base, pkts);
			}

			video_pts += video_frame_duration;
			if (ret >= 0) {
				for (int i = 0; i < pkts.size(); ++i) {
					ret = mp4_muxer.SendPacket(pkts[i]);
				}
			}
			pkts.clear();
		} else if (audio_finish != 1) {
			read_len = fread(pcm_frame_buf, 1, pcm_frame_size, in_pcm_fp);
			if (read_len < pcm_frame_size) {
				audio_finish = 1;
				LOG(INFO) << "fread pcm_frame_buf finished\n";
			}

			if (audio_finish != 1) {
				AVFrame* fltp_frame = AllocFltpPcmFrame(pcm_channels,
						audio_encoder.GetFrameSize());

				ret = audio_resample.ResampleFromS16ToFLTP(pcm_frame_buf, fltp_frame);
				if (ret < 0) {
					LOG(INFO) << "ResampleFromS16ToFLTP error\n";
				}

				ret = audio_encoder.Encode(fltp_frame, audio_idx, audio_pts,
						audio_time_base, pkts);
				FreePcmFrame(fltp_frame);
			} else {
				LOG(INFO) << "flush audio encoder\n";
				ret = audio_encoder.Encode(NULL,
						audio_idx, audio_pts, audio_time_base, pkts);
			}

			audio_pts += audio_frame_duration;
			if (ret >= 0) {
				for (int i = 0; i < pkts.size(); i++) {
					ret = mp4_muxer.SendPacket(pkts[i]);
				}
			}
			pkts.clear();
		}
	}

	ret = mp4_muxer.SendTailer();
	if (ret < 0) {
		LOG(ERROR) << "mp4_muxer.SendTailer failed\n";
	}

	printf("write mp4 finish\n");

	if (yuv_frame_buf) {
		free(yuv_frame_buf);
	}

	if (pcm_frame_buf) {
		free(pcm_frame_buf);
	}

	if (in_yuv_fp) {
		fclose(in_yuv_fp);
	}

	if (in_pcm_fp) {
		fclose(in_pcm_fp);
	}


	return 0; 
}