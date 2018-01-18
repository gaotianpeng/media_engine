#ifndef _MEDIA_H
#define _MEDIA_H

#include <string>
#include "Audio.h"
#include "Video.h"

extern "C" {
#include "libavformat\avformat.h"
}

struct VideoState;

struct MediaState
{
	AudioState *m_pAudio;
	VideoState *m_pVideo;
	AVFormatContext *m_pFmtCtx;

	char* m_fileName;

	MediaState(char *filename);
	~MediaState();

	bool OpenInput();
};

int get_packet_thread(void *data);

#endif