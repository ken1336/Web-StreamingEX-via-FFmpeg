#include"ows.h"


OWS_EXPORT __INT_E init_proc() {
	return 0;
}
OWS_EXPORT __CHARS_E text_out(char* str);


int init_context(StreamingContext *st) {

	int ret = 0;
	st->context_id = 10;
	av_register_all();

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
		ret = -1;
		print_log("Could not initialize SDL\n", ret);
		return ret;
	}

	if (!(st->in_format_context = avformat_alloc_context())) {
		ret = -1;
		print_log("Format context alloctaion error!", ret);
		return ret;
	}

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
	HANDLE main_thread;
	


	if (main_thread = create_thread(&stream_proc, &st.threadID, &st)) {
		WaitForSingleObject(main_thread, INFINITE);
		ret = 1;
		print_log(" main thread start\n", ret);
		
	}
	else {
		ret = -1;
		print_log(" main thread start fail\n", ret);
	}

	
	return ret;
}

void print_log(const char* str,int err_code) {

#if (_DEBUG == 1)
	printf("debug-logger:%s		code>>%d\n",str,err_code);
#else
	printf("release	%s	code>>%d\n", str, err_code); 
#endif

}
