#ifndef _VIDEO_H
#define _VIDEO_H

#include "PacketQueue.h"
#include "FrameQueue.h"
#include "Media.h"

struct MediaState;

struct VideoState
{
	PacketQueue* m_videPktQ;        

	int m_streamIdx;         
	AVCodecContext *m_pVCodecCtx;  
	AVStream *m_pVStream;       

	FrameQueue m_vFrameQ;      
	AVFrame *m_pVFrame;
	AVFrame *m_pDisplayFrame;

	double frame_timer;         // Sync fields
	double frame_last_pts;
	double frame_last_delay;
	double video_clock;

	SDL_Window *m_pWnd;
	SDL_Renderer *m_pRenderer;
	SDL_Texture *m_pTex;
	SDL_Rect m_pRect;

	void VideoPlay(MediaState *media);

	double SynchronizeAToV(AVFrame *srcFrame, double pts);
	
	VideoState();

	~VideoState();
};

int decode_video_packet(void *arg); 

#endif