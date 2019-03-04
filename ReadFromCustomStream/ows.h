#pragma once

#pragma comment ( lib, "lib/avcodec.lib" )
#pragma comment ( lib, "lib/avdevice.lib")
#pragma comment ( lib, "lib/avfilter.lib")
#pragma comment ( lib, "lib/avformat.lib")
#pragma comment ( lib, "lib/avutil.lib")
#pragma comment ( lib, "lib/postproc.lib")
#pragma comment ( lib, "lib/swresample.lib")
#pragma comment ( lib, "lib/swscale.lib")

#pragma comment ( lib, "lib/SDL2.lib")
#pragma comment ( lib, "lib/SDL2main.lib")
#pragma comment ( lib, "lib/SDL2test.lib")
extern "C" //FFmpeg가 C라이브러리이기 때문에 이부분이 필요하다.
{
#include "libavcodec/avcodec.h"
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
#include "libavformat/avio.h""
#include"libavutil/file.h"
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include<stdint.h>
#include "SDL/SDL.h"
#include "SDL/SDL_thread.h"
#include"net.h"
#include"thread_m.h"
#include"log.h"


#define OWS_EXPORT __declspec(dllexport)
#define __INT_E int __stdcall
#define __CHARS_E char* __cdecl

#define DEFAULT_BUFFER_SIZE 4*1024
#define DEFAULT_STREAM_BUFFER_SIZE 32*1024*1024

enum OWSType {
	FILE_IO = 0, 
	TCP_IO
};

typedef struct StreamingContext {

	void* input_stream = NULL;
	unsigned int context_id;
	unsigned int threadID;
	//Decode proc properties
	AVFormatContext *in_format_context = NULL;
	AVCodecContext *decode_context = NULL;
	AVCodec *decoder = NULL;

	AVPacket input_packet;
	AVFrame *decode_out_frame = NULL;
	int got_frame;
	int vstream_idx = -1;

	//Input proc properties
	tcpio_stream* stream = NULL;

	AVIOContext *avio_ctx = NULL;
	uint8_t *avio_ctx_buffer = NULL;
	size_t  avio_ctx_buffer_size = DEFAULT_STREAM_BUFFER_SIZE;
	size_t buffer_size = DEFAULT_BUFFER_SIZE;
	struct buffer_data bd = { 0 };
	

	//Encode proc properties

	AVFormatContext *out_format_context = NULL;
	AVCodec *encoder = NULL;
	AVCodecContext *encode_context = NULL;
	AVOutputFormat * out_format = NULL;
	
	AVStream * out_stream= NULL;
	AVPacket *output_packet = NULL;

	void* output_stream = NULL;


	//SDL properties
	SDL_Event event;
	SDL_Window *screen = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Texture *texture = NULL;
	Uint8 *yPlane, *uPlane, *vPlane;
	size_t yPlaneSz, uvPlaneSz;
	int uvPitch;
	struct SwsContext *sws_ctx = NULL;

};

int init_context(StreamingContext *s_cxt);
extern "C"
{



	OWS_EXPORT __CHARS_E text_out(char* str);
	OWS_EXPORT __INT_E math_add1(int a, int b);
	OWS_EXPORT __CHARS_E text(char* str);
	OWS_EXPORT __INT_E start_main_thread();
}

int stream_proc(void* args);
//int read_packet(void *opaque, uint8_t *buf, int buf_size)
//{
//
//
//	struct tcpio_stream *bd = (tcpio_stream *)opaque;
//	//printf("buffer ptr: %d		buffer size: %d		buf_size:%d\n", bd->buffer->ptr, bd->buffer->size,buf_size);
//	buf_size = FFMIN(buf_size, bd->buffer->size);
//	if (!buf_size)
//		return AVERROR_EOF;
//	printf("ptr:%p		pre:%p		size:%d		buf_size: %d\n", bd->buffer->ptr, bd->buffer->pre, bd->buffer->size, buf_size);
//	if (recv(bd->socket, (char*)bd->buffer->ptr, buf_size, 0) <= 0) {
//		return 0;
//	}
//	/* copy internal buffer data to buf */
//	memcpy(buf, bd->buffer->ptr, buf_size);
//	bd->buffer->ptr += buf_size;
//	bd->buffer->size -= buf_size;
//	if (bd->buffer->size <= 0) {
//
//		printf("buffer ptr move to pre");
//		bd->buffer->ptr -= 32 * 1024 * 1024;
//		printf("move ptr:	%p	->	%p\n", bd->buffer->ptr, bd->buffer->pre);
//		bd->buffer->size = 32 * 1024 * 1024;
//	}
//	return buf_size;
//}