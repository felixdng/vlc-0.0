/*
* date: 2018/1/25
* file: protocol.c
* author: dengzhiqin
* describe: 封装通讯协议接口, 包括protobuf编解码和自定义协议编解码
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "protocol.h"
#include "msg_handler.h"
#include "util.h"


#define C_SESSION_ID_SIZE 36
#define C_MSG_TYPE_SIZE   4
#define C_SN_SIZE         4
#define C_LENGTH_SIZE     4
#define C_ADLER32_SIZE    8
#define C_HEAD_INFO_SIZE \
	(C_SESSION_ID_SIZE) + (C_MSG_TYPE_SIZE) + (C_SN_SIZE) + \
	(C_LENGTH_SIZE) + (C_ADLER32_SIZE) + (C_MSG_TYPE_SIZE)

#define S_MSG_TYPE_SIZE   4
#define S_SN_SIZE         4
#define S_UPD_FLAGS_SIZE  4
#define S_RESP_CODE_SIZE  2
#define S_LENGTH_SIZE     2
#define S_ADLER32_SIZE    8
#define S_HEAD_INFO_SIZE \
	(S_MSG_TYPE_SIZE) + (S_SN_SIZE) + (S_UPD_FLAGS_SIZE) + \
	(S_RESP_CODE_SIZE) + (S_LENGTH_SIZE) + (S_ADLER32_SIZE) + (S_MSG_TYPE_SIZE)

static char prv_session_id[37] = "57d99d89-caab-482a-a0e9-a0a803eed3ba";
static int prv_sn = 0;


/*
* @func: 释放结构体cmsg_t指针内存
* @param: arg [in] 结构体cmsg_t指针
*/
void cmsg_free(cmsg_t *arg)
{
	if (arg) {
		if (arg->data) {
			free(arg->data);
			arg->data = NULL;
		}
		free(arg);
		arg = NULL;
	}
}

/*
* @func: 释放结构体smsg_t指针内存
* @param: arg [in] 结构体smsg_t指针
*/
void smsg_free(smsg_t *arg)
{
	if (arg) {
		if (arg->data) {
			free(arg->data);
			arg->data = NULL;
		}
		free(arg);
		arg = NULL;
	}
}

/*
* @func: 客户端发往服务器的消息 编码
* @param: type [in] 消息类型,32位整数
* @param: args [in] 原始数据,以结构体指针的形式传参
* @param: count [out] 编码后字节流的字节数
* @param: buf [out] 编码后的字节流
* @return: 成功返回编码之后的字节流指针,该指针必须要用free()释放内存空间,
*          失败返回NULL.
*/
char *cli_to_serv_encode(int type, void *args, int *count, char **buf)
{
	msg_handler_t hdl;
	if (0 != msghdl_array_get(type, &hdl)) {
		fprintf(stderr, "No handler for type [0x%x]\n", type);
		return NULL;
	}

	char *pb_buf = NULL;
	int pb_size = 0;
	if (hdl.req_encode && args) {
		if (NULL == hdl.req_encode(args, &pb_buf, &pb_size))
			return NULL;
	}

	int destsize = pb_size + C_HEAD_INFO_SIZE;
	char *dest = (char *)malloc(destsize * sizeof(char));
	if (NULL == dest) {
		fprintf(stderr, "malloc failed\n");
		goto exit_1;
	}

	char type_bytes[C_MSG_TYPE_SIZE];
	int2bytes(type, type_bytes);

	char sn_bytes[C_SN_SIZE];
	int2bytes(++prv_sn, sn_bytes);

	char length_bytes[C_LENGTH_SIZE];
	int2bytes(pb_size, length_bytes);

	char adler32_bytes[C_ADLER32_SIZE];
	if (pb_buf)
		long2bytes(get_adler32(pb_buf, pb_size), adler32_bytes);
	else 
		long2bytes(0, adler32_bytes);

	memcpy(dest, prv_session_id, C_SESSION_ID_SIZE);
	memcpy(dest + C_SESSION_ID_SIZE, type_bytes, C_MSG_TYPE_SIZE);
	memcpy(dest + C_SESSION_ID_SIZE + C_MSG_TYPE_SIZE, sn_bytes, C_SN_SIZE);
	memcpy(dest + C_SESSION_ID_SIZE + C_MSG_TYPE_SIZE + C_SN_SIZE, length_bytes, C_LENGTH_SIZE);
	if (pb_buf)
		memcpy(dest + C_SESSION_ID_SIZE + C_MSG_TYPE_SIZE + C_SN_SIZE + C_LENGTH_SIZE, pb_buf, pb_size);
	memcpy(dest + C_SESSION_ID_SIZE + C_MSG_TYPE_SIZE + C_SN_SIZE + C_LENGTH_SIZE + pb_size, 
			adler32_bytes, C_ADLER32_SIZE);
	memcpy(dest + C_SESSION_ID_SIZE + C_MSG_TYPE_SIZE + C_SN_SIZE + C_LENGTH_SIZE + pb_size + C_ADLER32_SIZE, 
			type_bytes, C_MSG_TYPE_SIZE);

	if (pb_buf)
		free(pb_buf);
	*count = destsize;
	*buf = dest;
	return dest;

exit_1:
	if (pb_buf)
		free(pb_buf);
	return NULL;
}

/*
* @func: 客户端发往服务器的消息 解码
* @param: buf [in] 需要解码的字节流
* @param: size [in] buf的字节数
* @return: 成功返回解码后的cmsg_t结构体指针,该指针必须用cmsg_free()释放内存空间,
		   失败返回NULL.
*/
cmsg_t *cli_to_serv_decode(const char *buf, int size)
{
	char session_id_bytes[C_SESSION_ID_SIZE+1];
	memcpy(session_id_bytes, buf, C_SESSION_ID_SIZE);
	session_id_bytes[C_SESSION_ID_SIZE] = '\0';

	char type_bytes[C_MSG_TYPE_SIZE];
	memcpy(type_bytes, buf + C_SESSION_ID_SIZE, C_MSG_TYPE_SIZE);
	int type = recoverNetworkBytesOrderInt(type_bytes, 0);

	char sn_bytes[C_SN_SIZE];
	memcpy(sn_bytes, buf + C_SESSION_ID_SIZE + C_MSG_TYPE_SIZE, C_SN_SIZE);
	int sn = recoverNetworkBytesOrderInt(sn_bytes, 0);

	char length_bytes[C_LENGTH_SIZE];
	memcpy(length_bytes, buf + C_SESSION_ID_SIZE + C_MSG_TYPE_SIZE + C_SN_SIZE, C_LENGTH_SIZE);
	int length = recoverNetworkBytesOrderInt(length_bytes, 0);

	char *msg_bytes = NULL;
	if (length > 0) {
		msg_bytes = (char *)malloc(length * sizeof(char));
		if (NULL == msg_bytes)
			return NULL;
		memcpy(msg_bytes, buf + C_SESSION_ID_SIZE + C_MSG_TYPE_SIZE + C_SN_SIZE + C_LENGTH_SIZE, length);

		char adler32_bytes[C_ADLER32_SIZE];
		memcpy(adler32_bytes, buf + C_SESSION_ID_SIZE + C_MSG_TYPE_SIZE + C_SN_SIZE + C_LENGTH_SIZE + length, C_ADLER32_SIZE);
		unsigned long long adl = recoverNetworkBytesOrderLong(adler32_bytes, 0);
		unsigned long long verify_adl = get_adler32(msg_bytes, length);
		if (adl != verify_adl) {
			fprintf(stderr, "adler32 failed\n");
			goto exit_1;
		}
	}

	cmsg_t *relt = (cmsg_t *)malloc(sizeof(cmsg_t));
	if (!relt) {
		fprintf(stderr, "cmsg_t: malloc failed\n");
		goto exit_1;
	}

	memcpy(relt->sess_id, session_id_bytes, C_SESSION_ID_SIZE+1);
	relt->type = type;
	relt->sn = sn;
	relt->data = NULL;

	/* protobuf解码 */
	if (msg_bytes) {
		msg_handler_t hdl;
		if (0 != msghdl_array_get(type, &hdl)) {
			fprintf(stderr, "No handler for type [0x%x]\n", type);
			goto exit_2;
		}
		if (hdl.req_decode) {
			relt->data = hdl.req_decode(msg_bytes, length);
		}
	}

	if (msg_bytes)
		free(msg_bytes);
	return relt;

exit_2:
	free(relt);
exit_1:
	if (msg_bytes)
		free(msg_bytes);
	return NULL;
}

/*
* @func: 服务器发往客户端的消息 编码
* @param: type [in] 消息类型,32位整数
* @param: sn [in] 消息序号,将客户端传过来的sn发送回去
* @param: upd [in] 更新标识组
* @param: resp [in] 响应码
* @param: args [in] 原始数据,以结构体指针的形式传参
* @param: count [out] 编码后字节流的字节数
* @param: buf [out] 编码后的字节流
* @return: 成功返回编码之后的字节流指针,该指针必须要用free()释放内存空间,
*          失败返回NULL.
*/
char *serv_to_cli_encode(int type, int sn, unsigned int upd, short resp, void *args, int *count, char **buf)
{
	msg_handler_t hdl;
	if (0 != msghdl_array_get(type, &hdl)) {
		fprintf(stderr, "No handler for type [0x%x]\n", type);
		return NULL;
	}

	char *pb_buf = NULL;
	int pb_size = 0;
	if (hdl.resp_encode && args) {
		if (NULL == hdl.resp_encode(args, &pb_buf, &pb_size))
			return NULL;
	}

	int destsize = pb_size + S_HEAD_INFO_SIZE;
	char *dest = (char *)malloc(destsize * sizeof(char));
	if (NULL == dest) {
		fprintf(stderr, "malloc failed\n");
		goto exit_1;
	}

	char type_bytes[S_MSG_TYPE_SIZE];
	int2bytes(type, type_bytes);

	char sn_bytes[S_SN_SIZE];
	int2bytes(sn, sn_bytes);

	char upd_bytes[S_UPD_FLAGS_SIZE];
	int2bytes(upd, upd_bytes);

	char resp_bytes[S_RESP_CODE_SIZE];
	short2bytes(resp, resp_bytes);

	char length_bytes[S_LENGTH_SIZE];
	short2bytes(pb_size, length_bytes);

	char adler32_bytes[S_ADLER32_SIZE];
	if (pb_buf)
		long2bytes(get_adler32(pb_buf, pb_size), adler32_bytes);
	else
		long2bytes(0, adler32_bytes);

	memcpy(dest, type_bytes, S_MSG_TYPE_SIZE);
	memcpy(dest + S_MSG_TYPE_SIZE, sn_bytes, S_SN_SIZE);
	memcpy(dest + S_MSG_TYPE_SIZE + S_SN_SIZE, upd_bytes, S_UPD_FLAGS_SIZE);
	memcpy(dest + S_MSG_TYPE_SIZE + S_SN_SIZE + S_UPD_FLAGS_SIZE, resp_bytes, S_RESP_CODE_SIZE);
	memcpy(dest + S_MSG_TYPE_SIZE + S_SN_SIZE + S_UPD_FLAGS_SIZE + S_RESP_CODE_SIZE, length_bytes, S_LENGTH_SIZE);
	if (pb_buf)
		memcpy(dest + S_MSG_TYPE_SIZE + S_SN_SIZE + S_UPD_FLAGS_SIZE + S_RESP_CODE_SIZE + S_LENGTH_SIZE, pb_buf, pb_size);
	memcpy(dest + S_MSG_TYPE_SIZE + S_SN_SIZE + S_UPD_FLAGS_SIZE + S_RESP_CODE_SIZE + S_LENGTH_SIZE + pb_size,
		adler32_bytes, S_ADLER32_SIZE);
	memcpy(dest + S_MSG_TYPE_SIZE + S_SN_SIZE + S_UPD_FLAGS_SIZE + S_RESP_CODE_SIZE + S_LENGTH_SIZE + pb_size + S_ADLER32_SIZE,
		type_bytes, S_MSG_TYPE_SIZE);

	if (pb_buf)
		free(pb_buf);
	*count = destsize;
	*buf = dest;
	return dest;

exit_1:
	if (pb_buf)
		free(pb_buf);
	return NULL;
}

/*
* @func: 服务器发往客户端的消息 解码
* @param: buf [in] 需要解码的字节流
* @param: size [in] buf的字节数
* @return: 成功返回解码后的smsg_t结构体指针,该指针必须用smsg_free()释放内存空间,
           失败返回NULL.
*/
smsg_t *serv_to_cli_decode(const char *buf, int size)
{
	char type_bytes[S_MSG_TYPE_SIZE];
	memcpy(type_bytes, buf, S_MSG_TYPE_SIZE);
	int type = recoverNetworkBytesOrderInt(type_bytes, 0);

	char sn_bytes[S_SN_SIZE];
	memcpy(sn_bytes, buf + S_MSG_TYPE_SIZE, S_SN_SIZE);
	int sn = recoverNetworkBytesOrderInt(sn_bytes, 0);

	char upd_bytes[S_UPD_FLAGS_SIZE];
	memcpy(upd_bytes, buf + S_MSG_TYPE_SIZE + S_SN_SIZE, S_UPD_FLAGS_SIZE);
	int upd = recoverNetworkBytesOrderInt(upd_bytes, 0);

	char resp_bytes[S_RESP_CODE_SIZE];
	memcpy(resp_bytes, buf + S_MSG_TYPE_SIZE + S_SN_SIZE + S_UPD_FLAGS_SIZE, S_RESP_CODE_SIZE);
	short resp = recoverNetworkBytesOrderShort(resp_bytes, 0);

	char length_bytes[S_LENGTH_SIZE];
	memcpy(length_bytes, buf + S_MSG_TYPE_SIZE + S_SN_SIZE + S_UPD_FLAGS_SIZE + S_RESP_CODE_SIZE, S_LENGTH_SIZE);
	short length = recoverNetworkBytesOrderShort(length_bytes, 0);

	char *msg_bytes = NULL;
	if (length > 0) {
		msg_bytes = (char *)malloc(length * sizeof(char));
		if (NULL == msg_bytes)
			return NULL;
		memcpy(msg_bytes, buf + S_MSG_TYPE_SIZE + S_SN_SIZE + S_UPD_FLAGS_SIZE + S_RESP_CODE_SIZE + S_LENGTH_SIZE, length);

		char adler32_bytes[S_ADLER32_SIZE];
		memcpy(adler32_bytes, buf + S_MSG_TYPE_SIZE + S_SN_SIZE + S_UPD_FLAGS_SIZE + S_RESP_CODE_SIZE + S_LENGTH_SIZE + length, S_ADLER32_SIZE);
		unsigned long long adl = recoverNetworkBytesOrderLong(adler32_bytes, 0);
		unsigned long long verify_adl = get_adler32(msg_bytes, length);
		if (adl != verify_adl) {
			fprintf(stderr, "adler32 failed\n");
			goto exit_1;
		}
	}

	smsg_t *relt = (smsg_t *)malloc(sizeof(smsg_t));
	if (!relt) {
		fprintf(stderr, "smsg_t: malloc failed\n");
		goto exit_1;
	}

	relt->type = type;
	relt->sn = sn;
	relt->updflags = upd;
	relt->respcode = resp;
	relt->data = NULL;

	if (msg_bytes) {
		/* protobuf解码 */
		msg_handler_t hdl;
		if (0 != msghdl_array_get(type, &hdl)) {
			fprintf(stderr, "No handler for type [0x%x]\n", type);
			goto exit_2;
		}
		if (hdl.resp_decode) {
			relt->data = hdl.resp_decode(msg_bytes, length);
		}
	}

	if (msg_bytes)
		free(msg_bytes);
	return relt;

exit_2:
	free(relt);
exit_1:
	if (msg_bytes)
		free(msg_bytes);
	return NULL;
}


