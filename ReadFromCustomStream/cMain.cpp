

#pragma comment ( lib, "avcodec.lib" )
#pragma comment ( lib, "avdevice.lib")
#pragma comment ( lib, "avfilter.lib")
#pragma comment ( lib, "avformat.lib")
#pragma comment ( lib, "avutil.lib")
#pragma comment ( lib, "postproc.lib")
#pragma comment ( lib, "swresample.lib")
#pragma comment ( lib, "swscale.lib")

#pragma comment ( lib, "SDL2.lib")
#pragma comment ( lib, "SDL2main.lib")
#pragma comment ( lib, "SDL2test.lib")
extern "C" //FFmpeg가 C라이브러리이기 때문에 이부분이 필요하다.

{

#include "libavcodec\avcodec.h"
#include "libavdevice\avdevice.h"
#include "libavfilter\avfilter.h"
#include "libavformat\avformat.h"
#include "libavutil\avutil.h"
#include "libpostproc\postprocess.h"
#include "libswresample\swresample.h"
#include "libswscale\swscale.h"
#include "libavutil/mathematics.h"

#include "libavutil/opt.h"
#include "libavutil/imgutils.h"


#include "libavutil/avassert.h"
#include "libavutil/channel_layout.h"


#include "libavutil/timestamp.h"




}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include<stdint.h>
#include "SDL/SDL.h"
//#include "SDL/SDL_thread.h"

//
//static AVFormatContext *fmt_ctx = NULL;
//static AVCodecContext *dec_ctx = NULL;
//AVCodec *dec;
//static int vst_idx = -1;
//const char* filename = "C:\\Users\\ken13\\Desktop\\media\\Sample.mp4";

void open_input_file()
{


	///* open input streams */
	//avformat_open_input(&fmt_ctx, filename, NULL, NULL);

	///* select the video stream */
	//vst_idx = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, &dec, 0);
	//dec_ctx = fmt_ctx->streams[vst_idx]->codec;

	///* init the video decoder */
	//avcodec_open2(dec_ctx, dec, NULL);
}

void close_input_file()
{

	/*for (int i = 0; i < fmt_ctx->nb_streams; i++) {
		AVStream *st = fmt_ctx->streams[i];
		avcodec_close(st->codec);
	}
	avformat_close_input(&fmt_ctx);*/
}

void encode_video()
{
}
#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(55,28,1)
#define av_frame_alloc avcodec_alloc_frame
#define av_frame_free avcodec_free_frame
#endif
int main(int argc, char *argv[]) {

	//open_input_file();
	//encode_video();
	//close_input_file();


	AVFormatContext *fmt_ctx = NULL;
	AVCodecContext *dec_ctx = NULL;
	AVCodec *dec;
	int vst_idx = -1;
	const char* filename = "C:\\Users\\ken13\\Desktop\\media\\Sample.mp4";
	AVPacket pkt;
	AVFrame *frm;
	int got_frame, got_output = 0;
	av_register_all();

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
		fprintf(stderr, "Could not initialize SDL - %s\n", SDL_GetError());
		exit(1);
	}
	printf("net: %d\n", avformat_network_init());

	if (avformat_open_input(&fmt_ctx, "tcp://127.0.0.1:8888", NULL, NULL) != 0) {
		printf("IO ERROR!\n");
		return -1;
	}
	else {
		printf("avformat_open success\n");
	}
	if (avformat_find_stream_info(fmt_ctx, NULL) < 0)
		return -1; // Couldn't find stream information

	av_dump_format(fmt_ctx, 0, filename, 0);


	vst_idx = -1;
	int i;
	for (i = 0; i < fmt_ctx->nb_streams; i++)
		if (fmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
			vst_idx = i;
			break;
		}
	if (vst_idx == -1)
		return -1; // Didn't find a video stream

	/* select the video stream */
	//vst_idx = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, &dec, 0);
	AVCodecContext *pCodecCtxOrig = fmt_ctx->streams[vst_idx]->codec;



	dec = avcodec_find_decoder(pCodecCtxOrig->codec_id);
	if (dec == NULL) {
		printf("unsupported codec\n");
	}




	dec_ctx = avcodec_alloc_context3(dec);
	if (avcodec_copy_context(dec_ctx, pCodecCtxOrig) != 0) {
		fprintf(stderr, "Couldn't copy codec context");
		return -1; // Error copying codec context
	}
	/* init the video decoder */
	if (avcodec_open2(dec_ctx, dec, NULL) < 0) {
		printf("Could not open avcodec\n");
		return -1;
	}
	frm = av_frame_alloc();


	struct SwsContext *sws_ctx = sws_getContext(dec_ctx->width, dec_ctx->height,
		dec_ctx->pix_fmt, dec_ctx->width, dec_ctx->height,
		AV_PIX_FMT_YUV420P,
		SWS_BILINEAR,
		NULL,
		NULL,
		NULL);



	const AVCodec *enc = avcodec_find_encoder(AV_CODEC_ID_H265);
	//const AVCodec *enc = avcodec_find_encoder(AV_CODEC_ID_VP8);
	AVCodecContext *enc_ctx = NULL;

	/* find the h.264 video encoder */


	enc_ctx = avcodec_alloc_context3(enc);

	///* put sample parameters */
	//enc_ctx->bit_rate = 400000;
	////enc_ctx->sample_rate = 44100;
	///* resolution must be a multiple of two */
	//enc_ctx->width = 1280;
	//enc_ctx->height = 720;

	///* frames per second */
	//enc_ctx->time_base = { 1, 25 };
	////enc_ctx->framerate = { 25, 1 };
	//enc_ctx->gop_size = 12; /* emit one intra frame every ten frames */
	//enc_ctx->max_b_frames = 2;
	//enc_ctx->pix_fmt = AV_PIX_FMT_YUV420P;

	enc_ctx->bit_rate = dec_ctx->bit_rate;
	enc_ctx->width = dec_ctx->width;
	enc_ctx->height = dec_ctx->height;
	enc_ctx->time_base = { 1, 25 };
	//enc_ctx->framerate = { 25, 1 };
	enc_ctx->gop_size = dec_ctx->gop_size;
	enc_ctx->max_b_frames = dec_ctx->max_b_frames;
	enc_ctx->pix_fmt = dec_ctx->pix_fmt;
	/* init the video encoder */



	const char* outfile = "C:\\Users\\ken13\\Desktop\\media\\testsample";
	AVOutputFormat * outFmt = NULL;
	AVFormatContext *outFmtCtx = NULL;
	avformat_alloc_output_context2(&outFmtCtx, outFmt, "hls", outfile);
	AVStream * outStrm = avformat_new_stream(outFmtCtx, enc_ctx->codec);

	printf("%s\n", outFmtCtx->oformat->name);
	//outFmtCtx->oformat = outFmt;
	outStrm->codec = enc_ctx;
	outStrm->time_base = enc_ctx->time_base;
	//outStrm->duration = 5;
	if (enc->id == AV_CODEC_ID_H264)
		av_opt_set(enc_ctx->priv_data, "preset", "slow", 0);

	avio_open(&outFmtCtx->pb, outfile, AVIO_FLAG_WRITE);

	//outFmt = outFmtCtx->oformat;

	//outStrm->codec->coder_type = 0;
	avcodec_open2(enc_ctx, enc, NULL);
	outStrm->codec = enc_ctx;

	avformat_write_header(outFmtCtx, &(outFmtCtx->metadata));

	//pkt = av_packet_alloc();
	AVPacket *outpkt1 = av_packet_alloc();




	// set up YV12 pixel array (12 bits per pixel)

	int size = 0;

	while (av_read_frame(fmt_ctx, &pkt) >= 0) {

		//printf("%d	%d\n", pkt.size,pkt.stream_index);
		if (pkt.stream_index == vst_idx) {


			/* decode the frame */
			printf("decode size: %d\n", avcodec_decode_video2(dec_ctx, frm, &got_frame, &pkt));
			if (got_frame) {



				//av_frame_make_writable(frm);
				int gg;
				avcodec_encode_video2(enc_ctx, outpkt1, frm, &gg);

				if (gg) {
					printf("write%d\n", outpkt1->size);
					av_write_frame(outFmtCtx, outpkt1);

					size += outpkt1->size;
					av_init_packet(outpkt1);
					//av_free_packet(outpkt1);
				}



				/*	int ret = avcodec_send_frame(enc_ctx, frm);
					while (ret >= 0) {

						ret = avcodec_receive_packet(enc_ctx, outpkt1);
					}
					printf("write%d\n", outpkt1->size);
					av_write_frame(outFmtCtx, outpkt1);
					av_free_packet(outpkt1);*/
			}
			/*int ret = avcodec_send_frame(dec_ctx, frm);
			while (ret >= 0) {

				ret = avcodec_receive_packet(dec_ctx, &pkt);
			}
			printf("frame size: %d\n",pkt.size);*/





		}
		av_free_packet(outpkt1);
		av_free_packet(&pkt);

	}
	//av_init_packet(&outpkt);
	//outpkt.data = NULL;
	//outpkt.size = 0;

	///* get the delayed frames */
	//got_output = 1;
	//printf("free outpkt\n");
	//while (got_output) {
	//	/* encode the frame */
	//	avcodec_encode_video2(enc_ctx, &outpkt, frm, &got_output);
	//	if (got_output) {
	//		/* write or send the frame */
	//		printf("free outpkt\n");
	//		av_free_packet(&outpkt);
	//	}
	//}
	//av_frame_free(&frm);

	printf("bit rate:%d	duration: %d	data size: %d	%d\n", outFmtCtx->bit_rate, outFmtCtx->duration, size, outStrm->duration);
	av_write_trailer(outFmtCtx);
	avio_close(outFmtCtx->pb);
	avformat_free_context(outFmtCtx);



	av_frame_free(&frm);
	avformat_close_input(&fmt_ctx);


	return 0;
}