#include"net.h"


tcpio_stream* custom_tcp_open(char* ip, int port, buffer_data* buf) {
	WSADATA wasData;
	SOCKADDR_IN servAddr;
	SOCKET hSocket;
	const char* ipaddr = ip;
	



	//MessageBox(NULL, (wchar_t*)ip, L"asdf", MB_OK);
	if (WSAStartup(MAKEWORD(2, 2), &wasData) != 0) {
		//MessageBox(NULL, L"wsastartup error!", L"ERROR", MB_OK);
		printf("wsastartup error");
		return NULL;
	}

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		printf("socket error");
	//MessageBox(NULL, L"socket error", L"ERROR", MB_OK);
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(ipaddr);
	servAddr.sin_port = htons(port);

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		//MessageBox(NULL, L"Connect error!", L"ERROR", MB_OK);
		printf("connect error");
		return NULL;
	}
	//char sendBuf[10] = "hello";
	//MessageBox(NULL, L"connection on", L"good", MB_OK);

	//send(hSocket, sendBuf, 10, 0);
	printf("connection on\n");


	tcpio_stream *stream = (tcpio_stream*)malloc(sizeof(tcpio_stream));
	stream->buffer = buf;
	stream->buffer->ptr = (uint8_t*)malloc(stream->buffer->size);
	stream->socket = hSocket;

	
	return stream;
}

int custom_tcp_close(tcpio_stream* stream) {
	closesocket(stream->socket);
	//_beginthreadex(NULL, 0, sendThread, (void*)hSocket, 0, (unsigned*)&dwThreadID);
	WSACleanup();
	return 0;
}