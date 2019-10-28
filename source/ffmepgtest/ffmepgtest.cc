#define __STDC_CONSTANT_MACROS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
using namespace std; 
extern "C"
{
#include "libavutil/avassert.h"
#include "libavutil/channel_layout.h"
#include "libavutil/opt.h"
#include "libavutil/mathematics.h"
#include "libavutil/timestamp.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
}

 /**
  * @file
  * video encoding with libavcodec API example
  *
  * @example encode_video.c
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libavcodec/avcodec.h>

#include <libavutil/opt.h>
#include <libavutil/imgutils.h>


int main(int argc, char **argv)
{
	char outfile[] = "outld.aac";
	AVFormatContext *out_fmt = NULL;
	avformat_alloc_output_context2(&out_fmt, NULL, NULL, outfile);

	return 0;
}
