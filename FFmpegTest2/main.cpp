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
void SaveFrame(AVFrame *pFrame, int width, int height, int iFrame);

int main(int argc, char *argv[]) {
	AVFormatContext   *pFormatCtx = NULL;
	int               i, videoStream;
	AVCodecContext    *pCodecCtxOrig = NULL;
	AVCodecContext    *pCodecCtx = NULL;
	AVCodec           *pCodec = NULL;
	AVFrame           *pFrame = NULL;
	AVFrame           *pFrameRGB = NULL;
	AVPacket          packet;
	int               frameFinished = 0;
	int               numBytes;
	uint8_t           *buffer = NULL;
	struct SwsContext *sws_ctx = NULL;

	if (argc < 2) {
		//printf("Please provide a movie file\n");
		//return -1;
		argv[1] = (char*)"C:\\Users\\ken13\\Desktop\\sample.mp4";
	}
	
	// Register all formats and codecs
	av_register_all();


	if (avformat_open_input(&pFormatCtx, argv[1], NULL, 0) != 0)
		return -1; // Couldn't open file
	if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
		return -1; // Couldn't find stream information
	// Dump information about file onto standard error
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
	pCodecCtx = pFormatCtx->streams[videoStream]->codec;

	printf("pCodecContext: %d	%d\n", pCodecCtx->codec_id, pCodecCtx->codec_type);

	printf("pCodecContext: %d	%d\n", pCodecCtx->width, pCodecCtx->height);


	// Get a pointer to the codec context for the video stream
	pCodecCtxOrig = pFormatCtx->streams[videoStream]->codec;
	// Find the decoder for the video stream
	pCodec = avcodec_find_decoder(pCodecCtxOrig->codec_id);
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

	// Allocate an AVFrame structure
	pFrameRGB = av_frame_alloc();
	if (pFrameRGB == NULL)
		return -1;



	// Determine required buffer size and allocate buffer
	numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, pCodecCtx->width,
		pCodecCtx->height);
	buffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));

	printf("numBytes: %d\n", numBytes);


	avpicture_fill((AVPicture *)pFrameRGB, buffer, AV_PIX_FMT_RGB24,
		pCodecCtx->width, pCodecCtx->height);







	printf("start\n");

	
	// initialize SWS context for software scaling
	sws_ctx = sws_getContext(pCodecCtx->width,
		pCodecCtx->height,
		pCodecCtx->pix_fmt,
		pCodecCtx->width,
		pCodecCtx->height,
		AV_PIX_FMT_RGB24,
		SWS_BILINEAR,
		NULL,
		NULL,
		NULL
	);

	i = 0;
	while (av_read_frame(pFormatCtx, &packet) >= 0) {
		printf("read\n");
		// Is this a packet from the video stream?
		if (packet.stream_index == videoStream) {
			printf("	decode\n");
			// Decode video frame
			printf("finished:%d\n", frameFinished);
			avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);
		//	printf("finished:%d\n", frameFinished);
			// Did we get a video frame?
			if (frameFinished) {
				printf("		finish\n");
				// Convert the image from its native format to RGB
				sws_scale(sws_ctx, (uint8_t const * const *)pFrame->data,
					pFrame->linesize, 0, pCodecCtx->height,
					pFrameRGB->data, pFrameRGB->linesize);

				// Save the frame to disk
				if (++i <= 5) {
					printf("			save\n");
					SaveFrame(pFrameRGB, pCodecCtx->width, pCodecCtx->height, i);

				}
					
			}
		}

		// Free the packet that was allocated by av_read_frame
		av_free_packet(&packet);
	}

	printf("end\n");

	av_free(buffer);
	av_free(pFrameRGB);

	// Free the YUV frame
	av_free(pFrame);

	// Close the codecs
	avcodec_close(pCodecCtx);
	avcodec_close(pCodecCtxOrig);

	// Close the video file
	avformat_close_input(&pFormatCtx);

	return 0;
}


void SaveFrame(AVFrame *pFrame, int width, int height, int iFrame) {
	FILE *pFile;
	char szFilename[36];
	int  y;

	// Open file
	sprintf(szFilename, "C:\\Users\\ken13\\Desktop\\frame%d.ppm", iFrame);
	pFile = fopen(szFilename, "wb");
	if (pFile == NULL)
		return;

	// Write header
	fprintf(pFile, "P6\n%d %d\n255\n", width, height);

	// Write pixel data
	for (y = 0; y < height; y++)
		fwrite(pFrame->data[0] + y * pFrame->linesize[0], 1, width * 3, pFile);

	// Close file
	fclose(pFile);
}

