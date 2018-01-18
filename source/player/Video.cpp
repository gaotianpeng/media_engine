#include "Video.h"
#include "VideoDisplay.h"

extern "C"{
#include "libswscale\swscale.h"
}

VideoState::VideoState()
{
	m_pVCodecCtx = nullptr;
	m_streamIdx = -1;
	m_pVStream = nullptr;

	m_pWnd = nullptr;
	m_pTex = nullptr;
	m_pRenderer = nullptr;

	m_pVFrame = nullptr;
	m_pDisplayFrame = nullptr;

	m_videPktQ = new PacketQueue();

	frame_timer = 0.0;
	frame_last_delay = 0.0;
	frame_last_pts = 0.0;
	video_clock = 0.0;
}

VideoState::~VideoState()
{
	delete m_videPktQ;

	av_frame_free(&m_pVFrame);
	av_free(m_pDisplayFrame->data[0]);
	av_frame_free(&m_pDisplayFrame);
}

void VideoState::VideoPlay(MediaState *media)
{
	int width = 800;
	int height = 600;

	m_pWnd = SDL_CreateWindow("FFmpeg Decode", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		width, height, SDL_WINDOW_OPENGL);
	m_pRenderer = SDL_CreateRenderer(m_pWnd, -1, 0);
	m_pTex = SDL_CreateTexture(m_pRenderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING,
		width, height);

	m_pRect.x = 0;
	m_pRect.y = 0;
	m_pRect.w = width;
	m_pRect.h = height;

	m_pVFrame = av_frame_alloc();
	m_pDisplayFrame = av_frame_alloc();

	m_pDisplayFrame->format = AV_PIX_FMT_YUV420P;
	m_pDisplayFrame->width = width;
	m_pDisplayFrame->height = height;

	int numBytes = avpicture_get_size((AVPixelFormat)m_pDisplayFrame->format,m_pDisplayFrame->width, m_pDisplayFrame->height);
	uint8_t *buffer = (uint8_t*)av_malloc(numBytes * sizeof(uint8_t));

	avpicture_fill((AVPicture*)m_pDisplayFrame, buffer, (AVPixelFormat)m_pDisplayFrame->format, m_pDisplayFrame->width, m_pDisplayFrame->height);

	SDL_CreateThread(decode_video_packet, "", this);

	// start display
	schedule_refresh(media, 40); 
}

double VideoState::SynchronizeAToV(AVFrame *srcFrame, double pts)
{
	double frame_delay;

	if (pts != 0) {
		video_clock = pts; 
	}
	else
	{	
		pts = video_clock; 
	}

	frame_delay = av_q2d(m_pVStream->codec->time_base);
	frame_delay += srcFrame->repeat_pict * (frame_delay * 0.5);

	video_clock += frame_delay;

	return pts;
}


int  decode_video_packet(void *arg)
{
	VideoState *video = (VideoState*)arg;

	AVFrame *frame = av_frame_alloc();

	AVPacket packet;
	double pts;

	while (true)
	{
		video->m_videPktQ->DeQueue(&packet, true);

		int ret = avcodec_send_packet(video->m_pVCodecCtx, &packet);
		if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF)
		{
			continue;
		}

		ret = avcodec_receive_frame(video->m_pVCodecCtx, frame);
		if (ret < 0 && ret != AVERROR_EOF)
		{
			continue;
		}

		if ((pts = av_frame_get_best_effort_timestamp(frame)) == AV_NOPTS_VALUE)
		{
			pts = 0;
		}
		
		pts *= av_q2d(video->m_pVStream->time_base);

		pts = video->SynchronizeAToV(frame, pts);

		frame->opaque = &pts;

		if (video->m_vFrameQ.m_nbFrames >= FrameQueue::m_nCapacity)
		{
			SDL_Delay(500 * 2);
		}

		video->m_vFrameQ.EnQueue(frame);

		av_frame_unref(frame);
	}

	av_frame_free(&frame);

	return 0;
}

