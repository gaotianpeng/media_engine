#include "FrameQueue.h"

FrameQueue::FrameQueue()
{
	m_nbFrames = 0;
	m_pMutex = SDL_CreateMutex();
	m_pCond = SDL_CreateCond();
}

bool FrameQueue::EnQueue(const AVFrame* frame)
{
	AVFrame* pFrame = av_frame_alloc();

	int ret = av_frame_ref(pFrame, frame);
	if (ret < 0)
	{
		return false;
	}
	pFrame->opaque = (void *)new double(*(double*)pFrame->opaque); 

	SDL_LockMutex(m_pMutex);
	m_queue.push(pFrame);
	m_nbFrames++;
	
	SDL_CondSignal(m_pCond);
	SDL_UnlockMutex(m_pMutex);
	
	return true;
}

bool FrameQueue::DeQueue(AVFrame **frame)
{
	bool ret = true;

	SDL_LockMutex(m_pMutex);
	while (true)
	{
		if (!m_queue.empty())
		{
			if (av_frame_ref(*frame, m_queue.front()) < 0)
			{
				ret = false;
				break;
			}

			auto tmp = m_queue.front();
			m_queue.pop();

			av_frame_free(&tmp);
			m_nbFrames--;
			ret = true;
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