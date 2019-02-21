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

#include "SDL/SDL.h"
#include "SDL/SDL_thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

  // Make a screen to put our video
// compatibility with newer API
#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(55,28,1)
#define av_frame_alloc avcodec_alloc_frame
#define av_frame_free avcodec_free_frame
#endif
int main(int argc, char *argv[]) {



	AVFormatContext *pFormatCtx = NULL;
	int videoStream;
	unsigned i;
	AVCodecContext *pCodecCtxOrig = NULL;
	AVCodecContext *pCodecCtx = NULL;
	AVCodec *pCodec = NULL;
	AVFrame *pFrame = NULL;
	AVPacket packet;
	int frameFinished;
	struct SwsContext *sws_ctx = NULL;
	SDL_Event event;
	SDL_Window *screen;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	Uint8 *yPlane, *uPlane, *vPlane;
	size_t yPlaneSz, uvPlaneSz;
	int uvPitch;

	if (argc < 2) {
		fprintf(stderr, "Usage: test <file>\n");
		exit(1);
	}
	// Register all formats and codecs
	av_register_all();

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
		fprintf(stderr, "Could not initialize SDL - %s\n", SDL_GetError());
		exit(1);
	}

	// Open video file
	if (avformat_open_input(&pFormatCtx, argv[1], NULL, NULL) != 0)
		return -1; // Couldn't open file
	
	// Retrieve stream information
	if (avformat_find_stream_info(pFormatCtx, NULL) < 0)		
		return -1; // Couldn't find stream information


	

	// Dump information about file onto standard error


	/**
	* Print detailed information about the input or output format, such as
	* duration, bitrate, streams, container, programs, metadata, side data,
	* codec and time base.
	*
	* @param ic        the context to analyze
	* @param index     index of the stream to dump information about
	* @param url       the URL to print, such as source or destination file
	* @param is_output Select whether the specified context is an input(0) or output(1)
	*/
	av_dump_format(pFormatCtx, 0, argv[1], 0);
	
	// Find the first video stream
	videoStream = -1;
	for (i = 0; i < pFormatCtx->nb_streams; i++)
		if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
			videoStream = i;
			break;
		}
	if (videoStream == -1)
		return -1; // Didn't find a video stream
	
	// Get a pointer to the codec context for the video stream
	pCodecCtxOrig = pFormatCtx->streams[videoStream]->codec;
	// Find the decoder for the video stream
	pCodec = avcodec_find_decoder(pCodecCtxOrig->codec_id);
	printf("min debug duration: %d		codec id: %d	%d\n", pFormatCtx->duration, pCodecCtxOrig->codec_id,pCodec->name);
	if (pCodec == NULL) {
		fprintf(stderr, "Unsupported codec!\n");
		return -1; // Codec not found
	}

	// Copy context
	pCodecCtx = avcodec_alloc_context3(pCodec);
	if (avcodec_copy_context(pCodecCtx, pCodecCtxOrig) != 0) {
		fprintf(stderr, "Couldn't copy codec context");
		return -1; // Error copying codec context
	}

	// Open codec
	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
		return -1; // Could not open codec

	// Allocate video frame
	pFrame = av_frame_alloc();

	// Make a screen to put our video
	screen = SDL_CreateWindow(
		"FFmpeg Tutorial",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		pCodecCtx->width,
		pCodecCtx->height,
		0
	);

	if (!screen) {
		fprintf(stderr, "SDL: could not create window - exiting\n");
		exit(1);
	}

	renderer = SDL_CreateRenderer(screen, -1, 0);
	if (!renderer) {
		fprintf(stderr, "SDL: could not create renderer - exiting\n");
		exit(1);
	}

	// Allocate a place to put our YUV image on that screen
	texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_YV12,
		SDL_TEXTUREACCESS_STREAMING,
		pCodecCtx->width,
		pCodecCtx->height
	);
	if (!texture) {
		fprintf(stderr, "SDL: could not create texture - exiting\n");
		exit(1);
	}

	// initialize SWS context for software scaling
	printf("%d	%d	%d	%d", pCodecCtx->width, pCodecCtx->height,
		pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height);
	sws_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height,
		pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height,
		AV_PIX_FMT_YUV420P,
		SWS_BILINEAR,
		NULL,
		NULL,
		NULL);

	// set up YV12 pixel array (12 bits per pixel)
	yPlaneSz = pCodecCtx->width * pCodecCtx->height;
	uvPlaneSz = pCodecCtx->width * pCodecCtx->height / 4;
	yPlane = (Uint8*)malloc(yPlaneSz);
	uPlane = (Uint8*)malloc(uvPlaneSz);
	vPlane = (Uint8*)malloc(uvPlaneSz);
	if (!yPlane || !uPlane || !vPlane) {
		fprintf(stderr, "Could not allocate pixel buffers - exiting\n");
		exit(1);
	}

	uvPitch = pCodecCtx->width / 2;
	while (av_read_frame(pFormatCtx, &packet) >= 0) {
		//_sleep(40);
		// Is this a packet from the video stream?
		printf("%d	%d\n", packet.size,packet.stream_index);
		if (packet.stream_index == videoStream) {
			// Decode video frame
			avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);

			// Did we get a video frame?
			if (frameFinished) {
				AVPicture pict;
				pict.data[0] = yPlane;
				pict.data[1] = uPlane;
				pict.data[2] = vPlane;
				pict.linesize[0] = pCodecCtx->width;
				pict.linesize[1] = uvPitch;
				pict.linesize[2] = uvPitch;
				printf("frame size:%d\n",pFrame->linesize);
				// Convert the image into YUV format that SDL uses
				sws_scale(sws_ctx, (uint8_t const * const *)pFrame->data,
					pFrame->linesize, 0, pCodecCtx->height, pict.data,
					pict.linesize);

				SDL_UpdateYUVTexture(
					texture,
					NULL,
					yPlane,
					pCodecCtx->width,
					uPlane,
					uvPitch,
					vPlane,
					uvPitch
				);

				SDL_RenderClear(renderer);
				SDL_RenderCopy(renderer, texture, NULL, NULL);
				SDL_RenderPresent(renderer);

			}
		}

		// Free the packet that was allocated by av_read_frame
		av_free_packet(&packet);
		SDL_PollEvent(&event);
		switch (event.type) {
		case SDL_QUIT:
			SDL_DestroyTexture(texture);
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(screen);
			SDL_Quit();
			exit(0);
			break;
		default:
			break;
		}

	}

	// Free the YUV frame
	av_frame_free(&pFrame);
	free(yPlane);
	free(uPlane);
	free(vPlane);

	// Close the codec
	avcodec_close(pCodecCtx);
	avcodec_close(pCodecCtxOrig);

	// Close the video file
	avformat_close_input(&pFormatCtx);

	return 0;
}