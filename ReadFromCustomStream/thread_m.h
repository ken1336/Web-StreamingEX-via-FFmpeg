#pragma once
#include <windows.h>
#include <stdio.h>
#include<process.h>

#define CREATE_THREAD_FAIL -2

void* create_thread(void* function, unsigned int* threadID, void* args);