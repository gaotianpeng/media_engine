#include "PacketQueue.h"
#include <iostream>

extern bool quit;

PacketQueue::PacketQueue()
{
	m_nbPkts = 0;
	m_nSize = 0;

	m_pMutex = SDL_CreateMutex();
	m_pCond = SDL_CreateCond();
}

bool PacketQueue::EnQueue(const AVPacket *packet)
{
	AVPacket *pkt = av_packet_alloc();
	if (av_packet_ref(pkt, packet) < 0)
	{
		return false;
	}
	SDL_LockMutex(m_pMutex);
	m_pktQueue.push(*pkt);

	m_nSize += pkt->size;
	m_nbPkts++;

	SDL_CondSignal(m_pCond);
	SDL_UnlockMutex(m_pMutex);
	return true;
}

bool PacketQueue::DeQueue(AVPacket *packet, bool block)
{
	bool ret = false;

	SDL_LockMutex(m_pMutex);
	while (true)
	{
		if (quit)
		{
			ret = false;
			break;
		}

		if (!m_pktQueue.empty())
		{
			if (av_packet_ref(packet, &m_pktQueue.front()) < 0)
			{
				ret = false;
				break;
			}
			AVPacket pkt = m_pktQueue.front();
			m_pktQueue.pop();
			av_packet_unref(&pkt);
			m_nbPkts--;
			m_nSize -= packet->size;

			ret = true;
			break;
		}
		else if (!block)
		{
			ret = false;
			break;
		}
		else
		{
			SDL_CondWait(m_pCond, m_pMutex);
		}
	}
	SDL_UnlockMutex(m_pMutex);
	return ret;
}