#include"ows.h"


OWS_EXPORT __INT_E init_proc() {
	return 0;
}

OWS_EXPORT __INT_E print_test() {
	printf("test\n");
	return 0;
}
static int read_packet(void *opaque, uint8_t *buf, int buf_size)
{


	In_packet *bd = (In_packet*)opaque;
	//printf("buffer ptr: %d		buffer size: %d		buf_size:%d\n", bd->buffer->ptr, bd->buffer->size,buf_size);
	buf_size = FFMIN(buf_size, bd->size - bd->seek_point);

	if (!buf_size)
		return AVERROR_EOF;
	bd->seek_point += buf_size;
	memcpy(buf, bd->main_buffer, buf_size);
	
	bd->main_buffer += buf_size;
	bd->size -= buf_size;
	//printf("ptr:%p		pre:%p		size:%d		buf_size: %d\n", bd->buffer->ptr, bd->buffer->pre, bd->buffer->size, buf_size);
	//if (recv(bd->socket, (char*)bd->buffer->ptr, buf_size, 0) <= 0) {
	//	return 0;
	//}
	///* copy internal buffer data to buf */
	//memcpy(buf, bd->buffer->ptr, buf_size);
	//bd->buffer->ptr += buf_size;
	//bd->buffer->size -= buf_size;
	//if (bd->buffer->size <= 0) {

	//	printf("buffer ptr move to pre");
	//	bd->buffer->ptr -= 32 * 1024 * 1024;
	//	printf("move ptr:	%p	->	%p\n", bd->buffer->ptr, bd->buffer->pre);
	//	bd->buffer->size = 32 * 1024 * 1024;
	//}
	return buf_size;
}
int init_context(StreamingContext *st) {

	int ret = 0;
	st->context_id = 10;
	av_register_all();
	

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
		ret = -1;
		print_log("Could not initialize SDL\n", ret);
		return ret;
	}

	if (!(st->decode_ctx.in_format_context = avformat_alloc_context())) {
		ret = -1;
		print_log("Format context alloctaion error!", ret);
		return ret;
	}

	st->avio_ctx_buffer = (uint8_t*)av_malloc(DEFAULT_STREAM_BUFFER_SIZE);
	if (!st->avio_ctx_buffer) {
		ret = -1;
		print_log("avio_ctx_buffer alloctaion error!\n");
		return ret;
	}

	st->bd.size = DEFAULT_BUFFER_SIZE;

	//////////////////////////////////////////////////
	
	st->avio_ctx = avio_alloc_context(st->avio_ctx_buffer, DEFAULT_BUFFER_SIZE, 0, &st->in_packet, &read_packet, NULL, NULL);
	st->decode_ctx.in_format_context->pb = st->avio_ctx;
	return ret;
}

int stream_proc(void* args) {
	StreamingContext* st = (StreamingContext*)args;

	return 0;

	_endthreadex(1);
}
OWS_EXPORT __INT_E reset_buffer_index() {

}
OWS_EXPORT __INT_E start_main_thread(uint8_t *buffer,uint8_t *backBuffer, size_t buf_size) {

	int ret;
	StreamingContext st;
	st.in_packet.back_buffer = backBuffer;
	st.in_packet.main_buffer = buffer;
	st.in_packet.buffer_ref = &buffer;
	st.in_packet.size = buf_size;
	//st.in_packet.seek_point = 
	st.avio_ctx_buffer_size = buf_size;
	
	if (ret = init_context(&st) < 0) {
		print_log("Int streaming context error", ret);
	}
	



	if (create_thread(&stream_proc, &st.threadID, &st)) {
		
		ret = 1;
		print_log(" main thread start\n", ret);

	}
	else {
		ret = -1;
		print_log(" main thread start fail\n", ret);
	}


	return st.context_id;
}

void print_log(const char* str, int err_code) {

#if (_DEBUG == 1)
	printf("debug-logger:%s		code>>%d\n", str, err_code);
#else
	printf("release	%s	code>>%d\n", str, err_code);
#endif

}
OWS_EXPORT __CHARS_E text(char* str) {
	static char strTemp2[128] = { 0, };	//임시저장용 문자열
	sprintf_s(strTemp2, "%s strOnTest3 에서 리턴", str);	//문자열 합치기

	return strTemp2;
}