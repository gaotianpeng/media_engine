#ifndef _PACKET_QUEUE_H
#define _PACKET_QUEUE_H

#include <queue>

#include "SDL2/SDL.h"
#include "SDL2/SDL_thread.h"

extern "C"{
#include "libavcodec\avcodec.h"
}

struct PacketQueue
{
	std::queue<AVPacket> m_pktQueue;

	Uint32    m_nbPkts;
	Uint32    m_nSize;
	SDL_mutex *m_pMutex;
	SDL_cond  *m_pCond;

	PacketQueue();
	bool EnQueue(const AVPacket *packet);
	bool DeQueue(AVPacket *packet, bool block);
};

#endif