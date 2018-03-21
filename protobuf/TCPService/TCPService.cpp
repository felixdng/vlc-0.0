// TCPserver.cpp : 定义控制台应用程序的入口点。  
//  
//#include "stdafx.h"  
#include <iostream>  
#include <cstdlib>  
#include <WinSock2.h>  
#include <WS2tcpip.h>  
#pragma comment(lib,"ws2_32.lib")  

#include "protocol.h"
#include "msg_handler.h"
#include "protob.h"

using namespace std;
#define SERVERIP "192.168.153.1"//IP地址  
#define SERVERPORT 5050//端口号  

static inline void conn_handler(cmsg_t *msg, SOCKET sock)
{
	ConnectReq *req = (ConnectReq *)msg->data;
	printf("%s(): serial_num: %s\n", __func__, req->serial_num);

	/* 服务端: 返回连接请求 编码 */
	ConnectResp resp;
	memcpy(resp.serial_num, req->serial_num, sizeof(resp.serial_num));
	resp.connect_state = 1;
	resp.version_code = 3;
	resp.state = 200;

	char *sbuf = NULL;
	int sbufsize = 0;
	if (NULL == serv_to_cli_encode(msg->type, msg->sn, 0xf0001000, 200, &resp, &sbufsize, &sbuf)) {
		fprintf(stderr, "serv_to_cli_encode failed\n");
		return;
	}
	send(sock, sbuf, sbufsize, 0);

	free(sbuf);
}

static inline void disconn_handler(cmsg_t *msg, SOCKET sock)
{
	DisconnectReq *req = (DisconnectReq *)msg->data;
	printf("%s() : serial_num: %s\n", __func__, req->serial_num);
}

static inline void add_handler(cmsg_t *msg, SOCKET sock)
{
	AddActionReq *req = (AddActionReq *)msg->data;
	printf("%s()\n", __func__);
	printf("action_name: %s\n", req->action_name);
	printf("execute_order: 0x%llx\n", req->execute_order);
	printf("execute_time: %d\n", req->execute_time);
	printf("arguments: %s\n", req->arguments);

	/* 服务端: 返回连接请求 编码 */
	AddActionResp resp;
	resp.state = 200;

	char *sbuf = NULL;
	int sbufsize = 0;
	if (NULL == serv_to_cli_encode(msg->type, msg->sn, 0xf0001000, 200, &resp, &sbufsize, &sbuf)) {
		fprintf(stderr, "serv_to_cli_encode failed\n");
		return;
	}
	send(sock, sbuf, sbufsize, 0);

	free(sbuf);

}

static inline void modify_handler(cmsg_t *msg, SOCKET sock)
{
	ModifyActionReq *req = (ModifyActionReq *)msg->data;
	printf("%s()\n", __func__);
	printf("action_name: %s\n", req->action_name);
	printf("execute_order: 0x%llx\n", req->execute_order);
	printf("execute_time: %d\n", req->execute_time);
	printf("arguments: %s\n", req->arguments);

	/* 服务端: 返回连接请求 编码 */
	ModifyActionResp resp;
	resp.state = 200;

	char *sbuf = NULL;
	int sbufsize = 0;
	if (NULL == serv_to_cli_encode(msg->type, msg->sn, 0xf0001000, 200, &resp, &sbufsize, &sbuf)) {
		fprintf(stderr, "serv_to_cli_encode failed\n");
		return;
	}
	send(sock, sbuf, sbufsize, 0);

	free(sbuf);
}

static inline void delete_handler(cmsg_t *msg, SOCKET sock)
{
	DeleteActionReq *req = (DeleteActionReq *)msg->data;
	printf("%s()\n", __func__);
	printf("action_id: %lld\n", req->action_id);

	/* 服务端: 返回连接请求 编码 */
	DeleteActionResp resp;
	resp.state = 200;

	char *sbuf = NULL;
	int sbufsize = 0;
	if (NULL == serv_to_cli_encode(msg->type, msg->sn, 0xf0001000, 200, &resp, &sbufsize, &sbuf)) {
		fprintf(stderr, "serv_to_cli_encode failed\n");
		return;
	}
	send(sock, sbuf, sbufsize, 0);

	free(sbuf);
}

static inline void page_handler(cmsg_t *msg, SOCKET sock)
{
	SearchActionByPageReq *req = (SearchActionByPageReq *)msg->data;
	printf("%s()\n", __func__);
	printf("page_size: %d\n", req->page_size);
	printf("page_num: %d\n", req->page_num);

#if 1
	/* 服务端: 返回连接请求 编码 */
	SearchActionByPageResp resp = {
		4,
		{
			{1, "play", 0xabcd000000000001, 20, "key"},
			{2, "pause", 0xabcd000000000002, 20, "key"},
			{3, "quick", 0xabcd000000000004, 20, "key"},
			{4, "back", 0xabcd000000000008, 20, "key"},
		},
	};
#endif
	char *sbuf = NULL;
	int sbufsize = 0;
	if (NULL == serv_to_cli_encode(msg->type, msg->sn, 0xf0001000, 200, &resp, &sbufsize, &sbuf)) {
		fprintf(stderr, "serv_to_cli_encode failed\n");
		return;
	}
	send(sock, sbuf, sbufsize, 0);

	free(sbuf);
}
static inline void getorders_handler(cmsg_t *msg, SOCKET sock)
{
	printf("%s()\n", __func__);
	GetOrdersResp resp = {
		4,
		{
			0xabcd000000000001,
			0xabcd000000000002,
			0xabcd000000000004,
			0xabcd000000000008,
		},
	};

	char *sbuf = NULL;
	int sbufsize = 0;
	if (NULL == serv_to_cli_encode(msg->type, msg->sn, 0xf0001000, 200, &resp, &sbufsize, &sbuf)) {
		fprintf(stderr, "serv_to_cli_encode failed\n");
		return;
	}
	send(sock, sbuf, sbufsize, 0);

	free(sbuf);
}
static inline void searchorder_handler(cmsg_t *msg, SOCKET sock)
{
	printf("%s()\n", __func__);
	SearchActionByOrderReq *req = (SearchActionByOrderReq *)msg->data;
	printf("order: %llx\n", req->order);

	/* 服务端: 返回连接请求 编码 */
	SearchActionByOrderResp resp = {
		1,
		{
			{ 1, "play", 0xabcd000000000001, 20, "key"},
		},
	};

	char *sbuf = NULL;
	int sbufsize = 0;
	if (NULL == serv_to_cli_encode(msg->type, msg->sn, 0xf0001000, 200, &resp, &sbufsize, &sbuf)) {
		fprintf(stderr, "serv_to_cli_encode failed\n");
		return;
	}
	send(sock, sbuf, sbufsize, 0);

	free(sbuf);
}
static inline void getnames_handler(cmsg_t *msg, SOCKET sock)
{
	printf("%s()\n", __func__);
	GetActionNamesResp resp = {
		4,
		{
			"play",
			"pause",
			"quick",
			"back",
		},
	};

	char *sbuf = NULL;
	int sbufsize = 0;
	if (NULL == serv_to_cli_encode(msg->type, msg->sn, 0xf0001000, 200, &resp, &sbufsize, &sbuf)) {
		fprintf(stderr, "serv_to_cli_encode failed\n");
		return;
	}
	send(sock, sbuf, sbufsize, 0);

	free(sbuf);
}
static inline void searchname_handler(cmsg_t *msg, SOCKET sock)
{
	printf("%s()\n", __func__);
	SearchActionByNameReq *req = (SearchActionByNameReq *)msg->data;
	printf("name: %s\n", req->name);

	/* 服务端: 返回连接请求 编码 */
	SearchActionByNameResp resp = {
		4, "back", 0xabcd000000000008, 20, "key"
	};

	char *sbuf = NULL;
	int sbufsize = 0;
	if (NULL == serv_to_cli_encode(msg->type, msg->sn, 0xf0001000, 200, &resp, &sbufsize, &sbuf)) {
		fprintf(stderr, "serv_to_cli_encode failed\n");
		return;
	}
	send(sock, sbuf, sbufsize, 0);

	free(sbuf);
}
static inline void upload_handler(cmsg_t *msg, SOCKET sock)
{
	printf("%s()\n", __func__);
	UploadResp resp = {200};
	char *sbuf = NULL;
	int sbufsize = 0;
	if (NULL == serv_to_cli_encode(msg->type, msg->sn, 0xf0001000, 200, &resp, &sbufsize, &sbuf)) {
		fprintf(stderr, "serv_to_cli_encode failed\n");
		return;
	}
	send(sock, sbuf, sbufsize, 0);

	free(sbuf);
}
static inline void download_handler(cmsg_t *msg, SOCKET sock)
{
	printf("%s()\n", __func__);
	DownloadResp resp = { 200 };
	char *sbuf = NULL;
	int sbufsize = 0;
	if (NULL == serv_to_cli_encode(msg->type, msg->sn, 0xf0001000, 200, &resp, &sbufsize, &sbuf)) {
		fprintf(stderr, "serv_to_cli_encode failed\n");
		return;
	}
	send(sock, sbuf, sbufsize, 0);

	free(sbuf);
}
static inline void activation_handler(cmsg_t *msg, SOCKET sock)
{
	printf("%s()\n", __func__);
	ActivationReq *req = (ActivationReq *)msg->data;
	printf("activityai: %s\n", req->activity_ai);

	ActivationResp resp = {200};
	char *sbuf = NULL;
	int sbufsize = 0;
	if (NULL == serv_to_cli_encode(msg->type, msg->sn, 0xf0001000, 200, &resp, &sbufsize, &sbuf)) {
		fprintf(stderr, "serv_to_cli_encode failed\n");
		return;
	}
	send(sock, sbuf, sbufsize, 0);

	free(sbuf);
}
static void msg_handler(cmsg_t *msg, SOCKET sock)
{
	if (msg->type == MULTIMEDIA_CONNECT) {
		conn_handler(msg, sock);
	}
	else if (msg->type == MULTIMEDIA_DISCONNECT) {
		disconn_handler(msg, sock);
	}
	else if (msg->type == MULTIMEDIA_ADD_ACTION) {
		add_handler(msg, sock);
	}
	else if (msg->type == MULTIMEDIA_MODIFY_ACTION) {
		modify_handler(msg, sock);
	}
	else if (msg->type == MULTIMEDIA_DELETE_ACTION) {
		delete_handler(msg, sock);
	}
	else if (msg->type == MULTIMEDIA_SEARCH_ACTION_BY_PAGE) {
		page_handler(msg, sock);
	}
	else if (msg->type == MULTIMEDIA_GET_ORDERS) {
		getorders_handler(msg, sock);
	}
	else if (msg->type == MULTIMEDIA_SEARCH_ACTION_BY_ORDER) {
		searchorder_handler(msg, sock);
	}
	else if (msg->type == MULTIMEDIA_GET_ACTION_NAMES) {
		getnames_handler(msg, sock);
	}
	else if (msg->type == MULTIMEDIA_SEARCH_ACTION_BY_NAME) {
		searchname_handler(msg, sock);
	}
	else if (msg->type == MULTIMEDIA_UPLOAD) {
		upload_handler(msg, sock);
	}
	else if (msg->type == MULTIMEDIA_DOWNLOAD) {
		download_handler(msg, sock);
	}
	else if (msg->type == MULTIMEDIA_ACTIVATION) {
		activation_handler(msg, sock);
	}
	
}


int main(int argc, char **argv)
{
	if (argc != 3) {
		printf("Usg: %s <ip> <port>\n", argv[0]);
		return -1;
	}
	char ip[16];
	memcpy(ip, argv[1], sizeof(ip));
	int port = atoi(argv[2]);
	
	protob_init();

	WORD verision = MAKEWORD(2, 2);
	WSADATA lpData;
	int intEr = WSAStartup(verision, &lpData);//指定winsock版本并初始化  
	if (intEr != 0)
	{
		cout << "winsock init failed!" << endl;
		return 0;
	}
	else
		cout << "winsock init success!" << endl;

	//创建侦听socket  
	SOCKET listenScok = socket(AF_INET, SOCK_STREAM, 0);
	if (listenScok == INVALID_SOCKET)
	{
		cout << "socket error" << endl;
		return 0;
	}
	else
		cout << "create socket success" << endl;

	sockaddr_in hostAddr;
	hostAddr.sin_family = AF_INET;
	hostAddr.sin_port = htons(port);//转换成网络字节序  
										  //hostAddr.sin_addr.S_un.S_addr = inet_addr(SERVERIP);//转换成网络字节序  
										  //cout << "net IP:" << hostAddr.sin_addr.S_un.S_addr << endl;  
										  /*
										  inet_addr()版本太低，被弃用使用inet_pton(协议族，字符串IP地址，void目标in_addr*)
										  头文件：WS2tcpip.h
										  */
	in_addr addr;
	inet_pton(AF_INET, ip, (void*)&addr);
	hostAddr.sin_addr = addr;
	cout << "ip:" << addr.S_un.S_addr << endl;


	//侦听套接字listenSock绑定本机地址信息  
	int err;
	err = bind(listenScok, (struct sockaddr*)&hostAddr, sizeof(sockaddr));
	if (err != 0)
	{
		cout << "hostAddr bind failed!" << endl;
		return 0;
	}

	err = listen(listenScok, 3);
	if (err != 0)
	{
		cout << "listen socket listen failed!" << endl;
		return 0;
	}
	cout << "listening..." << endl;

	int no = 1;
	//while (true)
	//{
		sockaddr_in clientAddr;
		int len = sizeof(struct sockaddr);//必须指定长度，否则会导致accept返回10014错误  

										  //accept会循环等待客户端连接   
		SOCKET clientSock = accept(listenScok, (struct sockaddr*)&clientAddr, &len);
		if (clientSock == INVALID_SOCKET)
		{
			cout << "accept failed!" << endl;
			cout << WSAGetLastError() << endl;
			system("pause");
			return 0;
		}

		while (true) {
			printf("\n---------> 等待客户端的请求......\n");
			char buf[8096] = "\0";
			int bufsize = recv(clientSock, buf, sizeof(buf), 0);
			if (bufsize <= 0)
				break;

			cmsg_t *msg = cli_to_serv_decode(buf, bufsize);
			if (NULL == msg) {
				fprintf(stderr, "cli_to_serv_decode failed\n");
				continue;
			}
			msg_handler(msg, clientSock);
			cmsg_free(msg);
		}

		err = shutdown(clientSock, 2);
		if (err == SOCKET_ERROR)
		{
			cout << "shutdown failed!" << endl;
			return 0;
		}
	//}

	err = shutdown(listenScok, 2);
	if (err == SOCKET_ERROR)
	{
		cout << "shutdown failed!" << endl;
		return 0;
	}

	err = closesocket(listenScok);
	if (err == SOCKET_ERROR)
	{
		cout << "closesocket failed!" << endl;
		return 0;
	}

	if (WSACleanup() != 0)
	{
		cout << "WSACleanup failed!" << endl;
		return 0;
	}
	system("pause");
	return 0;
}