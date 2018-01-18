#ifndef _AUDIO_H
#define _AUDIO_H

#include "PacketQueue.h"

extern "C"{
#include "libavformat/avformat.h"
}

struct AudioState
{
	const uint32_t BUFFER_SIZE;

	PacketQueue m_audioPktQ;

	double audio_clock; 
	AVStream *m_pAStream; 

	uint8_t *audio_buff;       
	uint32_t audio_buff_size;  
	uint32_t audio_buff_index; 

	int m_streamIdx;        
	AVCodecContext *m_pACodecCtx; 
	AudioState();            
	AudioState(AVCodecContext *audio_ctx, int audio_stream);
	
	~AudioState();

	bool AudioPlay();

	double GetAudioClock();
};

void audio_callback(void* userdata, Uint8 *stream, int len);

int decode_audio_packet(AudioState *audio_state, uint8_t *audio_buf, int buf_size);

#endif