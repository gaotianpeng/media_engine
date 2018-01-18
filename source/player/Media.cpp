#include "Media.h"
#include <iostream>

extern "C"{
#include "libavutil/time.h"
}
extern bool quit;

MediaState::MediaState(char* input_file)
	:m_fileName(input_file)
{
	m_pFmtCtx = nullptr;
	m_pAudio = new AudioState();
	m_pVideo = new VideoState();
}

MediaState::~MediaState()
{
	if (m_pAudio)
	{
		delete m_pAudio;
		m_pAudio = NULL; 
	}
		 
	if (m_pVideo)
	{
		delete m_pVideo;
		m_pVideo = NULL; 
	}
}

bool MediaState::OpenInput()
{
	if (avformat_open_input(&m_pFmtCtx, m_fileName, nullptr, nullptr) < 0)
	{
		return false;
	}
		
	if (avformat_find_stream_info(m_pFmtCtx, nullptr) < 0)
	{
		return false;
	}

	av_dump_format(m_pFmtCtx, 0, m_fileName, 0);

	for (uint32_t i = 0; i < m_pFmtCtx->nb_streams; i++)
	{
		if (m_pFmtCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO && m_pAudio->m_streamIdx < 0)
		{
			m_pAudio->m_streamIdx = i;
		}
			
		if (m_pFmtCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO && m_pVideo->m_streamIdx < 0)
		{
			m_pVideo->m_streamIdx = i;
		}
	}

	if (m_pAudio->m_streamIdx < 0 || m_pVideo->m_streamIdx < 0)
	{
		return false;
	}
	
	AVCodec *pACodec = avcodec_find_decoder(m_pFmtCtx->streams[m_pAudio->m_streamIdx]->codec->codec_id);
	if (!pACodec)
	{
		return false;
	}

	m_pAudio->m_pAStream = m_pFmtCtx->streams[m_pAudio->m_streamIdx];
	m_pAudio->m_pACodecCtx = avcodec_alloc_context3(pACodec);
	if (avcodec_copy_context(m_pAudio->m_pACodecCtx, m_pFmtCtx->streams[m_pAudio->m_streamIdx]->codec) != 0)
	{
		return false;
	}
	avcodec_open2(m_pAudio->m_pACodecCtx, pACodec, nullptr);


	AVCodec *pVCodec = avcodec_find_decoder(m_pFmtCtx->streams[m_pVideo->m_streamIdx]->codec->codec_id);
	if (!pVCodec)
	{
		return false;
	}
	
	m_pVideo->m_pVStream = m_pFmtCtx->streams[m_pVideo->m_streamIdx];
	m_pVideo->m_pVCodecCtx = avcodec_alloc_context3(pVCodec);
	if (avcodec_copy_context(m_pVideo->m_pVCodecCtx, m_pFmtCtx->streams[m_pVideo->m_streamIdx]->codec) != 0)
	{
		return false;
	}
	avcodec_open2(m_pVideo->m_pVCodecCtx, pVCodec, nullptr);

	m_pVideo->frame_timer = static_cast<double>(av_gettime()) / 1000000.0;
	m_pVideo->frame_last_delay = 40e-3;

	return true;
}

int get_packet_thread(void *data)
{
	MediaState *pMedia = (MediaState*)data;
	AVPacket *pPkt = av_packet_alloc();

	while (true)
	{
		int ret = av_read_frame(pMedia->m_pFmtCtx, pPkt);
		if (ret < 0)
		{
			if (ret == AVERROR_EOF)
			{
				break;
			}

			if (pMedia->m_pFmtCtx->pb->error == 0) 
			{
				SDL_Delay(100);
				continue;
			}
			else
			{
				break;
			}
		}

		if (pPkt->stream_index == pMedia->m_pAudio->m_streamIdx) 
		{
			pMedia->m_pAudio->m_audioPktQ.EnQueue(pPkt);
			av_packet_unref(pPkt);
		}		
		else if (pPkt->stream_index == pMedia->m_pVideo->m_streamIdx) // video stream
		{
			pMedia->m_pVideo->m_videPktQ->EnQueue(pPkt);
			av_packet_unref(pPkt);
		}		
		else
		{
			av_packet_unref(pPkt);
		}
	}

	av_packet_free(&pPkt);

	return 0;
}