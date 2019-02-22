#pragma once

#pragma comment(lib,"ws2_32.lib")
#include<WinSock2.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<malloc.h>

typedef struct buffer_data {
	uint8_t *ptr;
	size_t size; ///< size left in the buffer
}buffer_data;

typedef struct tcpio_stream {
	buffer_data buffer;
	SOCKET socket;
}tcpio_stream;
tcpio_stream* custom_tcp_open(char* ip, int port);
int custom_tcp_close(tcpio_stream* stream);