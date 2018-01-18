#include "Audio.h"

#include <iostream>
#include <fstream>
extern "C" {
#include "libswresample\swresample.h"
}

extern bool quit;

AudioState::AudioState()
	:BUFFER_SIZE(192000)
{
	m_pACodecCtx = nullptr;
	m_streamIdx = -1;
	m_pAStream = nullptr;
	audio_clock = 0;

	audio_buff = new uint8_t[BUFFER_SIZE];
	audio_buff_size = 0;
	audio_buff_index = 0;
}

AudioState::AudioState(AVCodecContext *audioCtx, int index)
	:BUFFER_SIZE(192000)
{
	m_pACodecCtx = audioCtx;
	m_streamIdx = index;

	audio_buff = new uint8_t[BUFFER_SIZE];
	audio_buff_size = 0;
	audio_buff_index = 0;
}

AudioState::~AudioState()
{
	if (audio_buff)
	{
		delete[] audio_buff;
		audio_buff = NULL; 
	}
}

bool AudioState::AudioPlay()
{
	SDL_AudioSpec desired;
	desired.freq = m_pACodecCtx->sample_rate;
	desired.channels = m_pACodecCtx->channels;
	desired.format = AUDIO_S16SYS;
	desired.samples = 1024;
	desired.silence = 0;
	desired.userdata = this;
	desired.callback = audio_callback;

	if (SDL_OpenAudio(&desired, nullptr) < 0)
	{
		return false;
	}

	SDL_PauseAudio(0);

	return true;
}

double AudioState::GetAudioClock()
{
	int hw_buf_size = audio_buff_size - audio_buff_index;
	int bytes_per_sec = m_pAStream->codec->sample_rate * m_pACodecCtx->channels * 2;

	double pts = audio_clock - static_cast<double>(hw_buf_size) / bytes_per_sec;

	return pts;
}

void audio_callback(void* userdata, Uint8 *stream, int len)
{
	AudioState *audio_state = (AudioState*)userdata;

	SDL_memset(stream, 0, len);

	int audio_size = 0;
	int len_left = 0;
	while (len > 0)
	{
		// no data in buffer
		if (audio_state->audio_buff_index >= audio_state->audio_buff_size)
		{
			// decode the audio packet
			audio_size = decode_audio_packet(audio_state, audio_state->audio_buff, sizeof(audio_state->audio_buff));
			if (audio_size < 0)
			{
				audio_state->audio_buff_size = 0;
				memset(audio_state->audio_buff, 0, audio_state->audio_buff_size);
			}
			else
			{
				audio_state->audio_buff_size = audio_size;
			}

			audio_state->audio_buff_index = 0;
		}

		len_left = audio_state->audio_buff_size - audio_state->audio_buff_index; 
		if (len_left > len) 
		{
			len_left = len;
		}
		SDL_MixAudio(stream, audio_state->audio_buff + audio_state->audio_buff_index, len, SDL_MIX_MAXVOLUME);

		len -= len_left;
		stream += len_left;
		audio_state->audio_buff_index += len_left;
	}
}

int decode_audio_packet(AudioState *audio_state, uint8_t *audio_buf, int buf_size)
{
	AVFrame *pFrame = av_frame_alloc();
	int data_size = 0;
	AVPacket pkt;
	SwrContext *swr_ctx = nullptr;
	static double clock = 0;
	
	if (quit)
	{
		return -1;
	}

	if (!audio_state->m_audioPktQ.DeQueue(&pkt, true))
	{
		return -1;
	}

	if (pkt.pts != AV_NOPTS_VALUE)
	{
		audio_state->audio_clock = av_q2d(audio_state->m_pAStream->time_base) * pkt.pts;
	}

	int ret = avcodec_send_packet(audio_state->m_pACodecCtx, &pkt);
	if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF)
	{
		return -1;
	}

	ret = avcodec_receive_frame(audio_state->m_pACodecCtx, pFrame);
	if (ret < 0 && ret != AVERROR_EOF)
	{
		return -1;
	}

	if (pFrame->channels > 0 && pFrame->channel_layout == 0)
	{
		pFrame->channel_layout = av_get_default_channel_layout(pFrame->channels);
	}
	else if (pFrame->channels == 0 && pFrame->channel_layout > 0)
	{
		pFrame->channels = av_get_channel_layout_nb_channels(pFrame->channel_layout);
	}

	AVSampleFormat dst_format = AV_SAMPLE_FMT_S16;
	Uint64 dst_layout = av_get_default_channel_layout(pFrame->channels);
	swr_ctx = swr_alloc_set_opts(nullptr, dst_layout, dst_format, pFrame->sample_rate,
		pFrame->channel_layout, (AVSampleFormat)pFrame->format, pFrame->sample_rate, 0, nullptr);
	if (!swr_ctx || swr_init(swr_ctx) < 0)
	{
		return -1;
	}

	uint64_t dst_nb_samples = av_rescale_rnd(swr_get_delay(swr_ctx, pFrame->sample_rate) + pFrame->nb_samples, pFrame->sample_rate, pFrame->sample_rate, AVRounding(1));
	int nb = swr_convert(swr_ctx, &audio_buf, static_cast<int>(dst_nb_samples), (const uint8_t**)pFrame->data, pFrame->nb_samples);
	data_size = pFrame->channels * nb * av_get_bytes_per_sample(dst_format);

	audio_state->audio_clock += static_cast<double>(data_size) / (2 * audio_state->m_pAStream->codec->channels * audio_state->m_pAStream->codec->sample_rate);

	av_frame_free(&pFrame);
	swr_free(&swr_ctx);

	return data_size;
}