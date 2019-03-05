
#include"ows.h"
//#define __MIN_SCREEN
#define __MIN_ENCODE

static int read_packet(void *opaque, uint8_t *buf, int buf_size)
{

	
	struct tcpio_stream *bd = (tcpio_stream *)opaque;
	//printf("buffer ptr: %d		buffer size: %d		buf_size:%d\n", bd->buffer->ptr, bd->buffer->size,buf_size);
	buf_size = FFMIN(buf_size, bd->buffer->size);
	if (!buf_size)
		return AVERROR_EOF;
	printf("ptr:%p		pre:%p		size:%d		buf_size: %d\n", bd->buffer->ptr, bd->buffer->pre, bd->buffer->size, buf_size);
	if (recv(bd->socket, (char*)bd->buffer->ptr, buf_size, 0) <= 0) {
		return 0;
	}
	/* copy internal buffer data to buf */
	memcpy(buf, bd->buffer->ptr, buf_size);
	bd->buffer->ptr += buf_size;
	bd->buffer->size -= buf_size;
	if (bd->buffer->size <= 0) {
		
		printf("buffer ptr move to pre");
		bd->buffer->ptr -= 32 * 1024 * 1024;
		printf("move ptr:	%p	->	%p\n", bd->buffer->ptr, bd->buffer->pre);
		bd->buffer->size = 32 * 1024 * 1024;
	}
	return buf_size;
}

static int64_t seek(void *opaque, int64_t offset, int whence) {
	printf("call seek function!		offset: %d		whence: %d\n",offset, whence);
	return 0;
}

#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(55,28,1)
#define av_frame_alloc avcodec_alloc_frame
#define av_frame_free avcodec_free_frame
#endif
int main(int argc, char *argv[]) {

	
	

	AVFormatContext *fmt_ctx = NULL;
	AVCodecContext *dec_ctx = NULL;
	AVCodec *dec;
	int vst_idx = -1;
	const char* filename = "C:\\Users\\ken13\\Desktop\\media\\Sample.mp4";
	AVPacket pkt;
	AVFrame *frm = NULL;
	int got_frame;
	av_register_all();

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
		fprintf(stderr, "Could not initialize SDL - %s\n", SDL_GetError());
		exit(1);
	}

	////////////////////////////////
	//printf("net: %d\n", avformat_network_init());
	
	AVIOContext *avio_ctx = NULL;
	uint8_t *avio_ctx_buffer = NULL;
	size_t buffer_size = 1024*128 , avio_ctx_buffer_size = 32*1024*1024;
	struct buffer_data bd = { 0 };
	int ret;
	if (!(fmt_ctx = avformat_alloc_context())) {
		printf("format context alloctaion error!\n");
		exit(-1);
	}


	avio_ctx_buffer = (uint8_t*)av_malloc(avio_ctx_buffer_size);
	

	if (!avio_ctx_buffer) {
		ret = AVERROR(ENOMEM);
		printf("avio_ctx_buffer alloctaion error!\n");
		exit(-1);
	}
	//buffer = 0;
	//bd.ptr = 0;
	bd.size = avio_ctx_buffer_size;

	tcpio_stream* stream = custom_tcp_open((char*)"127.0.0.1", 8888,&bd);
	avio_ctx = avio_alloc_context(avio_ctx_buffer, buffer_size, 0, stream, &read_packet, NULL, &seek);
	
	fmt_ctx->pb = avio_ctx;
	/////////////////////////////////


	

	if (avformat_open_input(&fmt_ctx, "", NULL, NULL) != 0) {
		printf("IO ERROR!\n");
		return -1;
	}
	else {
		printf("avformat_open success\n");
	}
	if (avformat_find_stream_info(fmt_ctx, NULL) < 0)
		return -1; // Couldn't find stream information

	//av_dump_format(fmt_ctx, 0, filename, 0);


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
	printf("min debug duration: %d		codec id: %d	%d\n", fmt_ctx->duration, pCodecCtxOrig->codec_id, dec->name);
	if (dec == NULL) {
		printf("unsupported codec\n");
	}




	dec_ctx = avcodec_alloc_context3(dec);
	if (avcodec_copy_context(dec_ctx, fmt_ctx->streams[vst_idx]->codec) != 0) {
		fprintf(stderr, "Couldn't copy codec context");
		return -1; // Error copying codec context
	}
	/* init the video decoder */
	if (avcodec_open2(dec_ctx, dec, NULL) < 0) {
		printf("Could not open avcodec\n");
		return -1;
	}
	






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
	enc_ctx->framerate = { 25, 1 };
	enc_ctx->gop_size = dec_ctx->gop_size;
	enc_ctx->max_b_frames = dec_ctx->max_b_frames;
	enc_ctx->pix_fmt = dec_ctx->pix_fmt;
	
	/* init the video encoder */


	const char* outfile = "C:\\Users\\ken13\\Desktop\\minpro\\FFmpegTest3\\WebApp\\media\\ttest.m3u8";
	//const char* outfile = "C:\\Users\\ken13\\Desktop\\media\\testsample.mp4";
	AVOutputFormat * outFmt = NULL;
	AVFormatContext *outFmtCtx = NULL;
	avformat_alloc_output_context2(&outFmtCtx, outFmt, NULL, outfile);
	AVStream * outStrm = avformat_new_stream(outFmtCtx, enc_ctx->codec);

	printf("%s\n", outFmtCtx->oformat->name);
	//outFmtCtx->oformat = outFmt;
	outStrm->codec = enc_ctx;
	outStrm->time_base = enc_ctx->time_base;
	outStrm->avg_frame_rate = { 25,1 };
	
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

	///////////////////////SDL init
	SDL_Event event;
	SDL_Window *screen;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	Uint8 *yPlane, *uPlane, *vPlane;
	size_t yPlaneSz, uvPlaneSz;
	int uvPitch;

	// Make a screen to put our video
	screen = SDL_CreateWindow(
		"FFmpeg Tutorial",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		dec_ctx->width,
		dec_ctx->height,
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
		dec_ctx->width,
		dec_ctx->height
	);
	if (!texture) {
		fprintf(stderr, "SDL: could not create texture - exiting\n");
		exit(1);
	}

	// initialize SWS context for software scaling
	printf("%d	%d	%d	%d	%d\n", dec_ctx->width, dec_ctx->height,
		dec_ctx->pix_fmt, dec_ctx->width, dec_ctx->height);


	struct SwsContext *sws_ctx = sws_getContext(dec_ctx->width, dec_ctx->height,
		dec_ctx->pix_fmt, dec_ctx->width, dec_ctx->height,
		AV_PIX_FMT_YUV420P,
		SWS_BILINEAR,
		NULL,
		NULL,
		NULL);


	// set up YV12 pixel array (12 bits per pixel)
	yPlaneSz = dec_ctx->width * dec_ctx->height;
	uvPlaneSz = dec_ctx->width * dec_ctx->height / 4;
	yPlane = (Uint8*)malloc(yPlaneSz);
	uPlane = (Uint8*)malloc(uvPlaneSz);
	vPlane = (Uint8*)malloc(uvPlaneSz);
	if (!yPlane || !uPlane || !vPlane) {
		fprintf(stderr, "Could not allocate pixel buffers - exiting\n");
		exit(1);
	}

	uvPitch = dec_ctx->width / 2;
	///////////////////////////////////////////////
	// set up YV12 pixel array (12 bits per pixel)

	int size = 0;
	frm = av_frame_alloc();
	
	while (av_read_frame(fmt_ctx, &pkt) >= 0) {
		//printf("%d	%d\n", pkt.size, pkt.stream_index);
		//printf("%d	%d\n", pkt.size,pkt.stream_index);
		if (pkt.stream_index == vst_idx) {


			/* decode the frame */
			ret = avcodec_decode_video2(dec_ctx, frm, &got_frame, &pkt);
		//	printf("decode size: %d\n",ret );
#ifdef __MIN_SCREEN
			if (got_frame) {
				AVPicture pict;
				pict.data[0] = yPlane;
				pict.data[1] = uPlane;
				pict.data[2] = vPlane;
				pict.linesize[0] = dec_ctx->width;
				pict.linesize[1] = uvPitch;
				pict.linesize[2] = uvPitch;
				
				//printf("frame size:%d		total:%d\n", frm->pkt_size,size+=frm->pkt_size);
				// Convert the image into YUV format that SDL uses
				sws_scale(sws_ctx, (uint8_t const * const *)frm->data,
					frm->linesize, 0, dec_ctx->height, pict.data,
					pict.linesize);

				SDL_UpdateYUVTexture(
					texture,
					NULL,
					yPlane,
					dec_ctx->width,
					uPlane,
					uvPitch,
					vPlane,
					uvPitch
				);

				SDL_RenderClear(renderer);
				SDL_RenderCopy(renderer, texture, NULL, NULL);
				SDL_RenderPresent(renderer);

			}
#endif

#ifdef __MIN_ENCODE
			if (got_frame) {



				//av_frame_make_writable(frm);
				int gg;
				avcodec_encode_video2(enc_ctx, outpkt1, frm, &gg);

				if (gg) {
					//printf("write%d\n", outpkt1->size);
					av_write_frame(outFmtCtx, outpkt1);

					size += outpkt1->size;
					av_init_packet(outpkt1);
					//av_free_packet(outpkt1);
				}


			}
#endif
			
#ifdef __MIN_SCREEN
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
#endif
			





		}
		av_free_packet(outpkt1);
		av_free_packet(&pkt);
	


	}
	free(yPlane);
	free(uPlane);
	free(vPlane);
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
	
	av_free(avio_ctx_buffer);

	av_frame_free(&frm);
	avformat_close_input(&fmt_ctx);

	if (avio_ctx) {
		av_freep(&avio_ctx->buffer);
		av_freep(&avio_ctx);
	}
	return 0;
}