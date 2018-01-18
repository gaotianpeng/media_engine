#include "VideoDisplay.h"
#include <iostream>

extern "C"{
#include "libswscale\swscale.h"
#include "libavutil\time.h"
}

static const double SYNC_THRESHOLD = 0.01;
static const double NOSYNC_THRESHOLD = 10.0;

void schedule_refresh(MediaState *media, int delay)
{
	SDL_AddTimer(delay, sdl_refresh_timer_cb, media);
}

uint32_t sdl_refresh_timer_cb(uint32_t interval, void *opaque)
{
	SDL_Event event;
	event.type = FF_REFRESH_EVENT;
	event.user.data1 = opaque;
	SDL_PushEvent(&event);
	return 0; 
}

void video_refresh_timer(void *userdata)
{
	MediaState *media = (MediaState*)userdata;
	VideoState *video = media->m_pVideo;

	if (video->m_streamIdx >= 0)
	{
		if (video->m_videPktQ->m_pktQueue.empty())
			schedule_refresh(media, 1);
		else
		{
			video->m_vFrameQ.DeQueue(&video->m_pVFrame);

			// ����Ƶͬ������Ƶ�ϣ�������һ֡���ӳ�ʱ��
			double current_pts = *(double*)video->m_pVFrame->opaque;
			double delay = current_pts - video->frame_last_pts;
			if (delay <= 0 || delay >= 1.0)
				delay = video->frame_last_delay;

			video->frame_last_delay = delay;
			video->frame_last_pts = current_pts;

			// ��ǰ��ʾ֡��PTS��������ʾ��һ֡���ӳ�
			double ref_clock = media->m_pAudio->GetAudioClock();

			double diff = current_pts - ref_clock;// diff < 0 => video slow,diff > 0 => video quick

			double threshold = (delay > SYNC_THRESHOLD) ? delay : SYNC_THRESHOLD;

			if (fabs(diff) < NOSYNC_THRESHOLD) // ��ͬ��
			{
				if (diff <= -threshold) // ���ˣ�delay��Ϊ0
					delay = 0;
				else if (diff >= threshold) // ���ˣ��ӱ�delay
					delay *= 2;
			}
			video->frame_timer += delay;
			double actual_delay = video->frame_timer - static_cast<double>(av_gettime()) / 1000000.0;
			if (actual_delay <= 0.010)
				actual_delay = 0.010; 

			schedule_refresh(media, static_cast<int>(actual_delay * 1000 + 0.5));

			SwsContext *sws_ctx = sws_getContext(video->m_pVCodecCtx->width, video->m_pVCodecCtx->height, video->m_pVCodecCtx->pix_fmt,
			video->m_pDisplayFrame->width,video->m_pDisplayFrame->height,(AVPixelFormat)video->m_pDisplayFrame->format, SWS_BILINEAR, nullptr, nullptr, nullptr);

			sws_scale(sws_ctx, (uint8_t const * const *)video->m_pVFrame->data, video->m_pVFrame->linesize, 0, 
				video->m_pVCodecCtx->height, video->m_pDisplayFrame->data, video->m_pDisplayFrame->linesize);

			SDL_UpdateTexture(video->m_pTex, &(video->m_pRect), video->m_pDisplayFrame->data[0], video->m_pDisplayFrame->linesize[0]);
			SDL_RenderClear(video->m_pRenderer);
			SDL_RenderCopy(video->m_pRenderer, video->m_pTex, &video->m_pRect, &video->m_pRect);
			SDL_RenderPresent(video->m_pRenderer);

			sws_freeContext(sws_ctx);
			av_frame_unref(video->m_pVFrame);
		}
	}
	else
	{
		schedule_refresh(media, 100);
	}
}