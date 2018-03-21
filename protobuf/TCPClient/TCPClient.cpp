// TCPclient.cpp : 定义控制台应用程序的入口点。  
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

#define SERVERIP "192.168.153.1"
#define SERVERPORT 5050  


void conn_req(SOCKET sock)
{
	char *buf = NULL;
	int count = 0;

	/* 客户端: 连接请求 编码 */
	ConnectReq req = {"123456789"};

	if (NULL == cli_to_serv_encode(MULTIMEDIA_CONNECT, &req, &count, &buf)) {
		fprintf(stderr, "cli_to_serv_encode failed\n");
		return;
	}

	send(sock, buf, count, 0);
	free(buf);
}

void conn_resp(SOCKET sock)
{
	char buf[8096] = "\0";
	int count = recv(sock, buf, sizeof(buf), 0);

	/* 客户端: 返回连接请求 解码 */
	smsg_t *smsg = serv_to_cli_decode(buf, count);
	if (NULL == smsg) {
		fprintf(stderr, "serv_to_cli_decode failed\n");
		return;
	}

	printf("\n%s()\n", __func__);
	printf("type: 0x%x\n", smsg->type);
	printf("sn: %d\n", smsg->sn);
	printf("udp: 0x%x\n", smsg->updflags);
	printf("resp: %d\n", smsg->respcode);
	ConnectResp *tmp = (ConnectResp *)smsg->data;
	printf("serial: %s\n", tmp->serial_num);
	printf("connect state: %d\n", tmp->connect_state);
	printf("version: %d\n", tmp->version_code);
	printf("state: %d\n", tmp->state);
	smsg_free(smsg);
}

void disconn_req(SOCKET sock)
{
	char *buf = NULL;
	int count = 0;

	/* 客户端: 连接请求 编码 */
	DisconnectReq req = { "123456789" };

	if (NULL == cli_to_serv_encode(MULTIMEDIA_DISCONNECT, &req, &count, &buf)) {
		fprintf(stderr, "cli_to_serv_encode failed\n");
		return;
	}

	send(sock, buf, count, 0);
	free(buf);
}

void add_req(SOCKET sock)
{
	char *buf = NULL;
	int count = 0;

	/* 客户端: 连接请求 编码 */
	AddActionReq req = {"play", 0xabcd000000000001, 30, "key1:val1"};

	if (NULL == cli_to_serv_encode(MULTIMEDIA_ADD_ACTION, &req, &count, &buf)) {
		fprintf(stderr, "cli_to_serv_encode failed\n");
		return;
	}

	send(sock, buf, count, 0);
	free(buf);
}

void add_resp(SOCKET sock)
{
	char buf[8096] = "\0";
	int count = recv(sock, buf, sizeof(buf), 0);

	/* 客户端: 返回连接请求 解码 */
	smsg_t *smsg = serv_to_cli_decode(buf, count);
	if (NULL == smsg) {
		fprintf(stderr, "serv_to_cli_decode failed\n");
		return;
	}

	printf("\n%s()\n", __func__);
	printf("type: 0x%x\n", smsg->type);
	printf("sn: %d\n", smsg->sn);
	printf("udp: 0x%x\n", smsg->updflags);
	printf("resp: %d\n", smsg->respcode);

	AddActionResp *tmp = (AddActionResp *)smsg->data;
	printf("state: %d\n", tmp->state);
	smsg_free(smsg);
}

void modify_req(SOCKET sock)
{
	char *buf = NULL;
	int count = 0;

	/* 客户端: 连接请求 编码 */
	ModifyActionReq req = { "play", 0xabcd000000000001, 30, "key1:val1" };

	if (NULL == cli_to_serv_encode(MULTIMEDIA_MODIFY_ACTION, &req, &count, &buf)) {
		fprintf(stderr, "cli_to_serv_encode failed\n");
		return;
	}

	send(sock, buf, count, 0);
	free(buf);
}

void modify_resp(SOCKET sock)
{
	char buf[8096] = "\0";
	int count = recv(sock, buf, sizeof(buf), 0);

	/* 客户端: 返回连接请求 解码 */
	smsg_t *smsg = serv_to_cli_decode(buf, count);
	if (NULL == smsg) {
		fprintf(stderr, "serv_to_cli_decode failed\n");
		return;
	}

	printf("\n%s()\n", __func__);
	printf("type: 0x%x\n", smsg->type);
	printf("sn: %d\n", smsg->sn);
	printf("udp: 0x%x\n", smsg->updflags);
	printf("resp: %d\n", smsg->respcode);

	ModifyActionResp *tmp = (ModifyActionResp *)smsg->data;
	printf("state: %d\n", tmp->state);
	smsg_free(smsg);
}

void delete_req(SOCKET sock)
{
	char *buf = NULL;
	int count = 0;

	/* 客户端: 连接请求 编码 */
	DeleteActionReq req = {2};

	if (NULL == cli_to_serv_encode(MULTIMEDIA_DELETE_ACTION, &req, &count, &buf)) {
		fprintf(stderr, "cli_to_serv_encode failed\n");
		return;
	}

	send(sock, buf, count, 0);
	free(buf);
}

void delete_resp(SOCKET sock)
{
	char buf[8096] = "\0";
	int count = recv(sock, buf, sizeof(buf), 0);

	/* 客户端: 返回连接请求 解码 */
	smsg_t *smsg = serv_to_cli_decode(buf, count);
	if (NULL == smsg) {
		fprintf(stderr, "serv_to_cli_decode failed\n");
		return;
	}

	printf("\n%s()\n", __func__);
	printf("type: 0x%x\n", smsg->type);
	printf("sn: %d\n", smsg->sn);
	printf("udp: 0x%x\n", smsg->updflags);
	printf("resp: %d\n", smsg->respcode);

	DeleteActionResp *tmp = (DeleteActionResp *)smsg->data;
	printf("state: %d\n", tmp->state);
	smsg_free(smsg);
}

void page_req(SOCKET sock)
{
	char *buf = NULL;
	int count = 0;

	/* 客户端: 连接请求 编码 */
	SearchActionByPageReq req = {12, 3};

	if (NULL == cli_to_serv_encode(MULTIMEDIA_SEARCH_ACTION_BY_PAGE, &req, &count, &buf)) {
		fprintf(stderr, "cli_to_serv_encode failed\n");
		return;
	}

	send(sock, buf, count, 0);
	free(buf);
}

void page_resp(SOCKET sock)
{
	char buf[8096] = "\0";
	int count = recv(sock, buf, sizeof(buf), 0);

	/* 客户端: 返回连接请求 解码 */
	smsg_t *smsg = serv_to_cli_decode(buf, count);
	if (NULL == smsg) {
		fprintf(stderr, "serv_to_cli_decode failed\n");
		return;
	}

	printf("\n%s()\n", __func__);
	printf("type: 0x%x\n", smsg->type);
	printf("sn: %d\n", smsg->sn);
	printf("udp: 0x%x\n", smsg->updflags);
	printf("resp: %d\n", smsg->respcode);

	SearchActionByPageResp *tmp = (SearchActionByPageResp *)smsg->data;
	printf("array_count: %d\n", tmp->action_array_count);
	int i;
	for (i = 0; i < tmp->action_array_count; i++) {
		printf("id: %lld\n", tmp->action_array[i].action_id);
		printf("name: %s\n", tmp->action_array[i].action_name);
		printf("order: 0x%llx\n", tmp->action_array[i].execute_order);
		printf("time: %lld\n", tmp->action_array[i].execute_time);
		printf("args: %s\n", tmp->action_array[i].arguments);
		printf("-------------------------------------\n");
	}
	smsg_free(smsg);
}

void getorders_req(SOCKET sock)
{
	char *buf = NULL;
	int count = 0;

	if (NULL == cli_to_serv_encode(MULTIMEDIA_GET_ORDERS, NULL, &count, &buf)) {
		fprintf(stderr, "cli_to_serv_encode failed\n");
		return;
	}

	send(sock, buf, count, 0);
	free(buf);
}

void getorders_resp(SOCKET sock)
{
	char buf[8096] = "\0";
	int count = recv(sock, buf, sizeof(buf), 0);

	/* 客户端: 返回连接请求 解码 */
	smsg_t *smsg = serv_to_cli_decode(buf, count);
	if (NULL == smsg) {
		fprintf(stderr, "serv_to_cli_decode failed\n");
		return;
	}

	printf("\n%s()\n", __func__);
	printf("type: 0x%x\n", smsg->type);
	printf("sn: %d\n", smsg->sn);
	printf("udp: 0x%x\n", smsg->updflags);
	printf("resp: %d\n", smsg->respcode);

	GetOrdersResp *tmp = (GetOrdersResp *)smsg->data;
	printf("array_count: %d\n", tmp->order_array_count);
	int i;
	for (i = 0; i < tmp->order_array_count; i++) {
		printf("order: 0x%llx\n", tmp->order_array[i]);
	}
	smsg_free(smsg);
}

void searchorder_req(SOCKET sock)
{
	char *buf = NULL;
	int count = 0;

	SearchActionByOrderReq req = {0xabcd000000000002};

	if (NULL == cli_to_serv_encode(MULTIMEDIA_SEARCH_ACTION_BY_ORDER, &req, &count, &buf)) {
		fprintf(stderr, "cli_to_serv_encode failed\n");
		return;
	}

	send(sock, buf, count, 0);
	free(buf);
}

void searchorder_resp(SOCKET sock)
{
	char buf[8096] = "\0";
	int count = recv(sock, buf, sizeof(buf), 0);

	/* 客户端: 返回连接请求 解码 */
	smsg_t *smsg = serv_to_cli_decode(buf, count);
	if (NULL == smsg) {
		fprintf(stderr, "serv_to_cli_decode failed\n");
		return;
	}

	printf("\n%s()\n", __func__);
	printf("type: 0x%x\n", smsg->type);
	printf("sn: %d\n", smsg->sn);
	printf("udp: 0x%x\n", smsg->updflags);
	printf("resp: %d\n", smsg->respcode);

	SearchActionByOrderResp *tmp = (SearchActionByOrderResp *)smsg->data;
	printf("array_count: %d\n", tmp->action_array_count);
	int i;
	for (i = 0; i < tmp->action_array_count; i++) {
		printf("id: %lld\n", tmp->action_array[i].action_id);
		printf("name: %s\n", tmp->action_array[i].action_name);
		printf("order: 0x%llx\n", tmp->action_array[i].execute_order);
		printf("time: %lld\n", tmp->action_array[i].execute_time);
		printf("args: %s\n", tmp->action_array[i].arguments);
		printf("-------------------------------------\n");
	}
	smsg_free(smsg);
}

void getnames_req(SOCKET sock)
{
	char *buf = NULL;
	int count = 0;

	if (NULL == cli_to_serv_encode(MULTIMEDIA_GET_ACTION_NAMES, NULL, &count, &buf)) {
		fprintf(stderr, "cli_to_serv_encode failed\n");
		return;
	}

	send(sock, buf, count, 0);
	free(buf);
}

void getnames_resp(SOCKET sock)
{
	char buf[8096] = "\0";
	int count = recv(sock, buf, sizeof(buf), 0);

	/* 客户端: 返回连接请求 解码 */
	smsg_t *smsg = serv_to_cli_decode(buf, count);
	if (NULL == smsg) {
		fprintf(stderr, "serv_to_cli_decode failed\n");
		return;
	}

	printf("\n%s()\n", __func__);
	printf("type: 0x%x\n", smsg->type);
	printf("sn: %d\n", smsg->sn);
	printf("udp: 0x%x\n", smsg->updflags);
	printf("resp: %d\n", smsg->respcode);

	GetActionNamesResp *tmp = (GetActionNamesResp *)smsg->data;
	printf("array_count: %d\n", tmp->name_array_count);
	int i;
	for (i = 0; i < tmp->name_array_count; i++) {
		printf("name: %s\n", tmp->name_array[i]);
	}
	smsg_free(smsg);
}

void searchname_req(SOCKET sock)
{
	char *buf = NULL;
	int count = 0;

	SearchActionByNameReq req = { "next" };

	if (NULL == cli_to_serv_encode(MULTIMEDIA_SEARCH_ACTION_BY_NAME, &req, &count, &buf)) {
		fprintf(stderr, "cli_to_serv_encode failed\n");
		return;
	}

	send(sock, buf, count, 0);
	free(buf);
}

void searchname_resp(SOCKET sock)
{
	char buf[8096] = "\0";
	int count = recv(sock, buf, sizeof(buf), 0);

	/* 客户端: 返回连接请求 解码 */
	smsg_t *smsg = serv_to_cli_decode(buf, count);
	if (NULL == smsg) {
		fprintf(stderr, "serv_to_cli_decode failed\n");
		return;
	}

	printf("\n%s()\n", __func__);
	printf("type: 0x%x\n", smsg->type);
	printf("sn: %d\n", smsg->sn);
	printf("udp: 0x%x\n", smsg->updflags);
	printf("resp: %d\n", smsg->respcode);

	SearchActionByNameResp *tmp = (SearchActionByNameResp *)smsg->data;
	printf("id: %lld\n", tmp->action_id);
	printf("name: %s\n", tmp->action_name);
	printf("order: 0x%llx\n", tmp->execute_order);
	printf("time: %lld\n", tmp->execute_time);
	printf("args: %s\n", tmp->arguments);
	printf("\n");

	smsg_free(smsg);
}

void upload_req(SOCKET sock)
{
	char *buf = NULL;
	int count = 0;

	if (NULL == cli_to_serv_encode(MULTIMEDIA_UPLOAD, NULL, &count, &buf)) {
		fprintf(stderr, "cli_to_serv_encode failed\n");
		return;
	}

	send(sock, buf, count, 0);
	free(buf);
}

void upload_resp(SOCKET sock)
{
	char buf[8096] = "\0";
	int count = recv(sock, buf, sizeof(buf), 0);

	/* 客户端: 返回连接请求 解码 */
	smsg_t *smsg = serv_to_cli_decode(buf, count);
	if (NULL == smsg) {
		fprintf(stderr, "serv_to_cli_decode failed\n");
		return;
	}

	printf("\n%s()\n", __func__);
	printf("type: 0x%x\n", smsg->type);
	printf("sn: %d\n", smsg->sn);
	printf("udp: 0x%x\n", smsg->updflags);
	printf("resp: %d\n", smsg->respcode);

	UploadResp *tmp = (UploadResp *)smsg->data;
	printf("state: %d\n", tmp->state);
	smsg_free(smsg);
}

void download_req(SOCKET sock)
{
	char *buf = NULL;
	int count = 0;

	if (NULL == cli_to_serv_encode(MULTIMEDIA_DOWNLOAD, NULL, &count, &buf)) {
		fprintf(stderr, "cli_to_serv_encode failed\n");
		return;
	}

	send(sock, buf, count, 0);
	free(buf);
}

void download_resp(SOCKET sock)
{
	char buf[8096] = "\0";
	int count = recv(sock, buf, sizeof(buf), 0);

	/* 客户端: 返回连接请求 解码 */
	smsg_t *smsg = serv_to_cli_decode(buf, count);
	if (NULL == smsg) {
		fprintf(stderr, "serv_to_cli_decode failed\n");
		return;
	}

	printf("\n%s()\n", __func__);
	printf("type: 0x%x\n", smsg->type);
	printf("sn: %d\n", smsg->sn);
	printf("udp: 0x%x\n", smsg->updflags);
	printf("resp: %d\n", smsg->respcode);

	DownloadResp *tmp = (DownloadResp *)smsg->data;
	printf("state: %d\n", tmp->state);
	smsg_free(smsg);
}

void activation_req(SOCKET sock)
{
	char *buf = NULL;
	int count = 0;

	ActivationReq req = {"xxxxxxxxx-234-xxa-adfasd"};

	if (NULL == cli_to_serv_encode(MULTIMEDIA_ACTIVATION, &req, &count, &buf)) {
		fprintf(stderr, "cli_to_serv_encode failed\n");
		return;
	}

	send(sock, buf, count, 0);
	free(buf);
}

void activation_resp(SOCKET sock)
{
	char buf[8096] = "\0";
	int count = recv(sock, buf, sizeof(buf), 0);

	/* 客户端: 返回连接请求 解码 */
	smsg_t *smsg = serv_to_cli_decode(buf, count);
	if (NULL == smsg) {
		fprintf(stderr, "serv_to_cli_decode failed\n");
		return;
	}

	printf("\n%s()\n", __func__);
	printf("type: 0x%x\n", smsg->type);
	printf("sn: %d\n", smsg->sn);
	printf("udp: 0x%x\n", smsg->updflags);
	printf("resp: %d\n", smsg->respcode);

	ActivationResp *tmp = (ActivationResp *)smsg->data;
	printf("state: %d\n", tmp->state);
	smsg_free(smsg);
}

void cmd_handler(int cmd, SOCKET sock)
{
	switch (cmd) {
	case 1:
		conn_req(sock);
		conn_resp(sock);
		break;
	case 2:
		disconn_req(sock);
		break;
	case 3:
		add_req(sock);
		add_resp(sock);
		break;
	case 4:
		modify_req(sock);
		modify_resp(sock);
		break;
	case 5:
		delete_req(sock);
		delete_resp(sock);
		break;
	case 6:
		page_req(sock);
		page_resp(sock);
		break;
	case 7:
		getorders_req(sock);
		getorders_resp(sock);
		break;
	case 8:
		searchorder_req(sock);
		searchorder_resp(sock);
		break;
	case 9:
		getnames_req(sock);
		getnames_resp(sock);
		break;
	case 10:
		searchname_req(sock);
		searchname_resp(sock);
		break;
	case 11:
		upload_req(sock);
		upload_resp(sock);
		break;
	case 12:
		download_req(sock);
		download_resp(sock);
		break;
	case 13:
		activation_req(sock);
		activation_resp(sock);
		break;
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

	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA initData;
	int err = WSAStartup(sockVersion, &initData);
	if (err != 0)
		cout << "init Failed!" << endl;
	//while (true)
	//{
		//创建一个套接字，参数列表（地址族TCP、UDP；套接字协议类型TCP；套接字使用的特定协议0自动指定)  
		SOCKET mysocket = socket(AF_INET, SOCK_STREAM, 0);
		if (mysocket == INVALID_SOCKET)
		{
			cout << "create socket failed!" << endl;
			return 0;
		}

		sockaddr_in hostAddr;
		hostAddr.sin_family = AF_INET;
		hostAddr.sin_port = htons(port);
		in_addr addr;
		inet_pton(AF_INET, ip, (void*)&addr);
		hostAddr.sin_addr = addr;
		cout << "ip:" << addr.S_un.S_addr << endl;

		SOCKET conSock = socket(AF_INET, SOCK_STREAM, 0);
		if (conSock == INVALID_SOCKET)
		{
			cout << "conSock failed" << endl;
			system("pause");
			return 0;
		}

		err = connect(conSock, (sockaddr*)&hostAddr, sizeof(sockaddr));
		if (err == INVALID_SOCKET)
		{
			cout << "connect failed!" << endl;
			system("pause");
			return 0;
		}

		while (true) {
			int cmd = -1;
			cout << "\nselect num:\n";
			cout << "1.  建立连接\n";
			cout << "2.  断开连接\n";
			cout << "3.  增加\n";
			cout << "4.  修改\n";
			cout << "5.  删除\n";
			cout << "6.  分页查询\n";
			cout << "7.  获取指令类型\n";
			cout << "8.  按指令类型查询\n";
			cout << "9.  获取动作名称\n";
			cout << "10. 按动作名称查询\n";
			cout << "11. 上传\n";
			cout << "12. 下载\n";
			cout << "13. 激活\n";
			cout << "0. 退出\n\n";
			cin >> cmd;

			if (cmd == 0)
				break;

			cmd_handler(cmd, conSock);

		}
		if (closesocket(conSock) != 0)
		{
			cout << "closesocket failed!" << endl;
			system("pause");
			return 0;
		}
	//}


	WSACleanup();

	system("pause");
	return 0;
}