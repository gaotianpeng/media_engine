#ifndef _FRAME_QUEUE_H
#define _FRAME_QUEUE_H

#include "SDL2/SDL_thread.h"
#include <queue>

extern "C"{
#include "libavcodec\avcodec.h"
}

struct FrameQueue
{
	static const int m_nCapacity = 30;

	std::queue<AVFrame*> m_queue;

	uint32_t m_nbFrames;

	SDL_mutex* m_pMutex;
	SDL_cond * m_pCond;

	FrameQueue();
	bool EnQueue(const AVFrame* frame);
	bool DeQueue(AVFrame **frame);
};

#endif