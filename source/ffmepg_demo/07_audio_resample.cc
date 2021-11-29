extern "C" {
#include "libavutil/opt.h"
#include "libavutil/channel_layout.h"
#include "libavutil/samplefmt.h"
#include "libswresample/swresample.h"
}
#include "gtest/gtest.h"

static int get_format_from_sample_fmt(const char** fmt,
	enum AVSampleFormat sample_fmt)
{
	int i;
	struct sample_fmt_entry {
		enum AVSampleFormat sample_fmt; const char* fmt_be, * fmt_le;
	} sample_fmt_entries[] = {
	{ AV_SAMPLE_FMT_U8,  "u8",    "u8"    },
	{ AV_SAMPLE_FMT_S16, "s16be", "s16le" },
	{ AV_SAMPLE_FMT_S32, "s32be", "s32le" },
	{ AV_SAMPLE_FMT_FLT, "f32be", "f32le" },
	{ AV_SAMPLE_FMT_DBL, "f64be", "f64le" },
	};
	*fmt = NULL;

	for (i = 0; i < FF_ARRAY_ELEMS(sample_fmt_entries); i++) {
		struct sample_fmt_entry* entry = &sample_fmt_entries[i];
		if (sample_fmt == entry->sample_fmt) {
			*fmt = AV_NE(entry->fmt_be, entry->fmt_le);
			return 0;
		}
	}

	fprintf(stderr,
		"Sample format %s not supported as output format\n",
		av_get_sample_fmt_name(sample_fmt));
	return AVERROR(EINVAL);
}

static void fill_samples(double* dst, int nb_samples, int nb_channels, int sample_rate, double* t)
{
	int i, j;
	double tincr = 1.0 / sample_rate, * dstp = dst;
	const double c = 2 * M_PI * 440.0;

	/* generate sin tone with 440Hz frequency and duplicated channels */
	for (i = 0; i < nb_samples; i++) {
		*dstp = sin(c * *t);
		for (j = 1; j < nb_channels; j++)
			dstp[j] = dstp[0];
		dstp += nb_channels;
		*t += tincr;
	}
}

TEST(FFmpegAudioResample, AudioResample) {
	// in parameter
	int64_t src_ch_layout = AV_CH_LAYOUT_STEREO;
	int src_rate = 48000;
	enum AVSampleFormat src_sample_fmt = AV_SAMPLE_FMT_DBL;
	int src_nb_channels = 0;
	uint8_t** src_data = NULL;
	int src_linesize;
	int src_nb_samples = 1024;

	// out parameter
	int64_t dst_ch_layout = AV_CH_LAYOUT_STEREO;
	int dst_rate = 44100;
	enum AVSampleFormat dst_sample_fmt = AV_SAMPLE_FMT_S16;
	int dst_nb_channels = 0;
	uint8_t** dst_data = NULL;
	int dst_linesize;
	int dst_nb_samples;
	int max_dst_nb_samples;

	const char* dst_file = "d:/av_data/out_reample.pcm";
	FILE* dst_fp;

	int dst_buf_size;
	const char* fmt;
	struct SwrContext* swr_ctx;

	double t;
	int ret;
	
	dst_fp = fopen(dst_file, "wb");
	ASSERT_TRUE(dst_fp != NULL);

	swr_ctx = swr_alloc();
	ASSERT_TRUE(swr_ctx != NULL);

	av_opt_set_int(swr_ctx, "in_channel_layout", src_ch_layout, 0);
	av_opt_set_int(swr_ctx, "in_sample_rate", src_rate, 0);
	av_opt_set_sample_fmt(swr_ctx, "in_sample_fmt", src_sample_fmt, 0);

	av_opt_set_int(swr_ctx, "out_channel_layout", dst_ch_layout, 0);
	av_opt_set_int(swr_ctx, "out_sample_rate", dst_rate, 0);
	av_opt_set_sample_fmt(swr_ctx, "out_sample_fmt", dst_sample_fmt, 0);

	ret = swr_init(swr_ctx);
	ASSERT_TRUE(ret >= 0);

	src_nb_channels = av_get_channel_layout_nb_channels(src_ch_layout);
	ret = av_samples_alloc_array_and_samples(&src_data, &src_linesize, src_nb_channels,
		src_nb_samples, src_sample_fmt, 0);
	ASSERT_TRUE(ret >= 0);

	max_dst_nb_samples = dst_nb_samples = av_rescale_rnd(src_nb_samples, dst_rate, src_rate, AV_ROUND_UP);
	dst_nb_channels = av_get_channel_layout_nb_channels(dst_ch_layout);

	ret = av_samples_alloc_array_and_samples(&dst_data, &dst_linesize, dst_nb_channels,
		dst_nb_samples, dst_sample_fmt, 0);
	ASSERT_TRUE(ret >= 0);

	t = 0;
	do {
		fill_samples((double*)src_data[0], src_nb_samples, src_nb_channels, src_rate, &t);
		int64_t delay = swr_get_delay(swr_ctx, src_rate);
		dst_nb_samples = av_rescale_rnd(delay + src_nb_samples, dst_rate, src_rate, AV_ROUND_UP);
		if (dst_nb_samples > max_dst_nb_samples) {
			av_freep(&dst_data[0]);
			ret = av_samples_alloc(dst_data, &dst_linesize, dst_nb_channels, dst_nb_samples, dst_sample_fmt, 1);
			if (ret < 0) {
				break;			
			}
			max_dst_nb_samples = dst_nb_samples;
		}

		ret = swr_convert(swr_ctx, dst_data, dst_nb_samples, (const uint8_t**)src_data, src_nb_samples);
		ASSERT_TRUE(ret >= 0);

		dst_buf_size = av_samples_get_buffer_size(&dst_linesize, dst_nb_samples, ret, dst_sample_fmt, 1);
		ASSERT_TRUE(dst_buf_size >= 0);

		printf("t:%f in:%d out:%d\n", t, src_nb_samples, ret);
		fwrite(dst_data[0], 1, dst_buf_size, dst_fp);
	} while (t < 10);

	ret = swr_convert(swr_ctx, dst_data, dst_nb_samples, NULL, 0);
	ASSERT_TRUE(ret >= 0);

	dst_buf_size = av_samples_get_buffer_size(&dst_linesize, dst_nb_channels,
		ret, dst_sample_fmt, 1);
	ASSERT_TRUE(dst_buf_size >= 0);
	printf("flush in:%d out:%d\n", 0, ret);
	fwrite(dst_data[0], 1, dst_buf_size, dst_fp);

	ret = get_format_from_sample_fmt(&fmt, dst_sample_fmt);
	ASSERT_TRUE(ret >= 0);

	fprintf(stderr, "Resampling succeeded. Play the output file with the command:\n"
		"ffplay -f %s -channel_layout %ld" " -channels %d -ar %d %s\n",
		fmt, dst_ch_layout, dst_nb_channels, dst_rate, dst_file);

	fclose(dst_fp);
	if (src_data) {
		av_freep(&src_data[0]);
	}
	av_freep(&src_data);
	if (dst_data) {
		av_freep(&dst_data[0]);
	}
	av_freep(&dst_data);
}