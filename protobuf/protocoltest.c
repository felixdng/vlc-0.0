/*
* date: 2018/1/25
* file: protocoltest.c
* author: dengzhiqin
* describe: 测试程序
*/
#include <stdio.h>
#include <stdlib.h>

#include "protocol.h"
#include "msg_handler.h"
#include "protob.h"


void conn_test(void)
{
	char *buf = NULL;
	int count = 0;

	/* 客户端: 连接请求 编码 */
	char serial[] = "123456789";
	ConnectReq req;
	strcpy(req.serial_num, serial);

	if (NULL == cli_to_serv_encode(MULTIMEDIA_CONNECT, &req, &count, &buf)) {
		fprintf(stderr, "cli_to_serv_encode failed\n");
		return;
	}

	/* 服务端: 连接请求 解码 */
	cmsg_t *msg = cli_to_serv_decode(buf, count);
	if (NULL == msg) {
		fprintf(stderr, "cli_to_serv_decode failed\n");
		free(buf);
		return;
	}
	free(buf);
	buf = NULL;
	
	/* 服务端: 返回连接请求 编码 */
	ConnectResp resp;
	strcpy(resp.serial_num, serial);
	resp.connect_state = 1;
	resp.version_code = 3;
	resp.state = 200;

	if (NULL == serv_to_cli_encode(msg->type, msg->sn, 0xf0001000, 200, &resp, &count, &buf)) {
		fprintf(stderr, "serv_to_cli_encode failed\n");
		cmsg_free(msg);
		return;
	}
	cmsg_free(msg);

	/* 客户端: 返回连接请求 解码 */
	smsg_t *smsg = serv_to_cli_decode(buf, count);
	if (NULL == smsg) {
		fprintf(stderr, "serv_to_cli_decode failed\n");
		free(buf);
		return;
	}
	free(buf);
	buf = NULL;

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
#if 1
int main(int argc, char **argv)
{
	protob_init();

	conn_test();

	while (1);
	return 0;
}
#endif
