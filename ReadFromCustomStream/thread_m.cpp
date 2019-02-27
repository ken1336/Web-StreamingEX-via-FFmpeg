
#include"ows.h"
#ifdef _WIN64 || WIN32
void* create_thread(void* function, unsigned int* threadID, void* args) {


	
	void* hThread = (void*)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)function, (void*)args, 0, threadID);
	

	if (hThread == 0) {

		return NULL;
	}
	else {
		return hThread;
	}

}
#endif