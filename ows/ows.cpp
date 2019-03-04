#include"ows.h"


OWS_EXPORT __INT_E init_proc() {
	return 0;
}

OWS_EXPORT __INT_E print_test() {
	printf("test\n");
	return 0;
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
	return ret;
}

int stream_proc(void* args) {
	StreamingContext* st = (StreamingContext*)args;

	return 0;

	_endthreadex(1);
}
OWS_EXPORT __INT_E start_main_thread() {

	int ret;
	StreamingContext st;
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


	return ret;
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