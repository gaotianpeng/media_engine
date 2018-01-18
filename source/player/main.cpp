extern "C" {
#include "libavcodec\avcodec.h"
#include "libavformat\avformat.h"
#include "libswscale\swscale.h"
#include "libswresample\swresample.h"
}

#include "SDL2/SDL.h"
#include "SDL2/SDL_thread.h"

#include <iostream>

#include "PacketQueue.h"
#include "Audio.h"
#include "Media.h"
#include "VideoDisplay.h"
using namespace std;

bool quit = false;

int main(int argv, char* argc[])
{
	av_register_all();

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);

	if (argv != 2) {
		return -1; 
	}

	char* filename = argc[1];
	MediaState media(filename);

	if (media.OpenInput())
	{
		SDL_CreateThread(get_packet_thread, "", &media); 
	}

	media.m_pAudio->AudioPlay(); 
	media.m_pVideo->VideoPlay(&media); 

	AVStream *audio_stream = media.m_pFmtCtx->streams[media.m_pAudio->m_streamIdx];
	AVStream *video_stream = media.m_pFmtCtx->streams[media.m_pVideo->m_streamIdx];

	double audio_duration = audio_stream->duration * av_q2d(audio_stream->time_base);
	double video_duration = video_stream->duration * av_q2d(video_stream->time_base);

	cout << "audio duration£º" << audio_duration << endl;
	cout << "video duration£º" << video_duration << endl;

	SDL_Event event;
	while (true) 
	{
		SDL_WaitEvent(&event);
		switch (event.type)
		{
		case FF_QUIT_EVENT:
		case SDL_QUIT:
			quit = 1;
			SDL_Quit();

			return 0;
			break;

		case FF_REFRESH_EVENT:
			video_refresh_timer(&media);
			break;

		default:
			break;
		}
	}

	getchar();
	return 0;
}