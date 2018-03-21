/*
* date: 2018/1/29
* file: protob.c
* author: dengzhiqin
* describe: 实现protobuf消息的编码和解码
*/
#include <stdio.h>
#include <stdlib.h>

#include "pb_common.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "protob.h"
#include "msg_handler.h"

#define BUFSIZE 8096

/* protobuf编码 */
#define PB_ENCODE(_type) \
	do { \
		int ret; \
		char buf[BUFSIZE]; \
		int len; \
				\
		_type *msg = (_type *)src; \
		if (!msg) \
			return NULL; \
						\
		memset(buf, 0, sizeof(buf)); \
		pb_ostream_t stream = pb_ostream_from_buffer(buf, sizeof(buf)); \
																		\
		ret = pb_encode(&stream, _type##_fields, msg); \
		len = stream.bytes_written; \
		if (!ret) { \
			fprintf(stderr, "Encoding failed: %s\n", PB_GET_ERROR(&stream)); \
			return NULL; \
		} \
			\
		*dest = (char *)malloc(len * sizeof(char)); \
		if (!(*dest)) \
			return NULL; \
		memcpy((*dest), buf, len); \
		*count = len; \
						\
		return (*dest); \
	} while (0)

/* protobuf解码 */
#define PB_DECODE(_type) \
	do { \
		_type *msg = (_type *)malloc(sizeof(_type)); \
		if (!msg) \
			return NULL; \
						\
		pb_istream_t stream = pb_istream_from_buffer(buff, len); \
		if (!pb_decode(&stream, _type##_fields, msg)) { \
			fprintf(stderr, "Decoding failed: %s\n", PB_GET_ERROR(&stream)); \
			free(msg); \
			return NULL; \
		} \
		return msg; \
	} while (0)

/********************************************************
* 消息编解码的函数说明
* xxx_req_encode: 请求消息的编码
* xxx_req_decode: 请求消息的解码
*
* xxx_resp_encode: 返回消息的编码
* xxx_resp_decode: 返回消息的编码
********************************************************/


static char *conn_req_encode(const void *src, char **dest, int *count)
{
#if 0
	int ret;
	char buf[BUFSIZE];
	int len;

	ConnectReq *msg = (ConnectReq *)src;
	if (!msg)
		return NULL;

	memset(buf, 0, sizeof(buf));
	pb_ostream_t stream = pb_ostream_from_buffer(buf, sizeof(buf));

	ret = pb_encode(&stream, ConnectReq_fields, msg);
	len = stream.bytes_written;
	if (!ret) {
		fprintf(stderr, "Encoding failed: %s\n", PB_GET_ERROR(&stream));
		return NULL;
	}

	*dest = (char *)malloc(len * sizeof(char));
	if (!(*dest))
		return NULL;
	memcpy((*dest), buf, len);
	*count = len;

	return (*dest);
#endif
	PB_ENCODE(ConnectReq);
}

static void *conn_req_decode(const char *buff, int len)
{
#if 0
	ConnectReq *msg = (ConnectReq *)malloc(sizeof(ConnectReq));
	if (!msg)
		return NULL;

	pb_istream_t stream = pb_istream_from_buffer(buff, len);
	if (!pb_decode(&stream, ConnectReq_fields, msg)) {
		fprintf(stderr, "Decoding failed: %s\n", PB_GET_ERROR(&stream));
		free(msg);
		return NULL;
	}
	return msg;
#endif
	PB_DECODE(ConnectReq);
}

static char *conn_resp_encode(const void *src, char **dest, int *count)
{
	PB_ENCODE(ConnectResp);
}

static void *conn_resp_decode(const char *buff, int len)
{
	PB_DECODE(ConnectResp);
}

static char *disconn_req_encode(const void *src, char **dest, int *count)
{
	PB_ENCODE(DisconnectReq);
}

static void *disconn_req_decode(const char *buff, int len)
{
	PB_DECODE(DisconnectReq);
}

static char *add_req_encode(const void *src, char **dest, int *count)
{
	PB_ENCODE(AddActionReq);
}

static void *add_req_decode(const char *buff, int len)
{
	PB_DECODE(AddActionReq);
}

static char *add_resp_encode(const void *src, char **dest, int *count)
{
	PB_ENCODE(AddActionResp);
}

static void *add_resp_decode(const char *buff, int len)
{
	PB_DECODE(AddActionResp);
}

static char *modify_req_encode(const void *src, char **dest, int *count)
{
	PB_ENCODE(ModifyActionReq);
}

static void *modify_req_decode(const char *buff, int len)
{
	PB_DECODE(ModifyActionReq);
}

static char *modify_resp_encode(const void *src, char **dest, int *count)
{
	PB_ENCODE(ModifyActionResp);
}

static void *modify_resp_decode(const char *buff, int len)
{
	PB_DECODE(ModifyActionResp);
}

static char *del_req_encode(const void *src, char **dest, int *count)
{
	PB_ENCODE(DeleteActionReq);
}

static void *del_req_decode(const char *buff, int len)
{
	PB_DECODE(DeleteActionReq);
}

static char *del_resp_encode(const void *src, char **dest, int *count)
{
	PB_ENCODE(DeleteActionResp);
}

static void *del_resp_decode(const char *buff, int len)
{
	PB_DECODE(DeleteActionResp);
}

static char *search_page_req_encode(const void *src, char **dest, int *count)
{
	PB_ENCODE(SearchActionByPageReq);
}

static void *search_page_req_decode(const char *buff, int len)
{
	PB_DECODE(SearchActionByPageReq);
}

static char *search_page_resp_encode(const void *src, char **dest, int *count)
{
	PB_ENCODE(SearchActionByPageResp);
}

static void *search_page_resp_decode(const char *buff, int len)
{
	PB_DECODE(SearchActionByPageResp);
}

static char *get_order_resp_encode(const void *src, char **dest, int *count)
{
	PB_ENCODE(GetOrdersResp);
}

static void *get_order_resp_decode(const char *buff, int len)
{
	PB_DECODE(GetOrdersResp);
}

static char *search_order_req_encode(const void *src, char **dest, int *count)
{
	PB_ENCODE(SearchActionByOrderReq);
}

static void *search_order_req_decode(const char *buff, int len)
{
	PB_DECODE(SearchActionByOrderReq);
}

static char *search_order_resp_encode(const void *src, char **dest, int *count)
{
	PB_ENCODE(SearchActionByOrderResp);
}

static void *search_order_resp_decode(const char *buff, int len)
{
	PB_DECODE(SearchActionByOrderResp);
}

static char *get_name_resp_encode(const void *src, char **dest, int *count)
{
	PB_ENCODE(GetActionNamesResp);
}

static void *get_name_resp_decode(const char *buff, int len)
{
	PB_DECODE(GetActionNamesResp);
}

static char *search_name_req_encode(const void *src, char **dest, int *count)
{
	PB_ENCODE(SearchActionByNameReq);
}

static void *search_name_req_decode(const char *buff, int len)
{
	PB_DECODE(SearchActionByNameReq);
}

static char *search_name_resp_encode(const void *src, char **dest, int *count)
{
	PB_ENCODE(SearchActionByNameResp);
}

static void *search_name_resp_decode(const char *buff, int len)
{
	PB_DECODE(SearchActionByNameResp);
}

static char *upload_resp_encode(const void *src, char **dest, int *count)
{
	PB_ENCODE(UploadResp);
}

static void *upload_resp_decode(const char *buff, int len)
{
	PB_DECODE(UploadResp);
}

static char *download_resp_encode(const void *src, char **dest, int *count)
{
	PB_ENCODE(DownloadResp);
}

static void *download_resp_decode(const char *buff, int len)
{
	PB_DECODE(DownloadResp);
}

static char *activation_req_encode(const void *src, char **dest, int *count)
{
	PB_ENCODE(ActivationReq);
}

static void *activation_req_decode(const char *buff, int len)
{
	PB_DECODE(ActivationReq);
}

static char *activation_resp_encode(const void *src, char **dest, int *count)
{
	PB_ENCODE(ActivationResp);
}

static void *activation_resp_decode(const char *buff, int len)
{
	PB_DECODE(ActivationResp);
}

static msg_handler_t msghdl[] = {
	{MULTIMEDIA_CONNECT, conn_req_encode, conn_req_decode, conn_resp_encode, conn_resp_decode},
	{MULTIMEDIA_DISCONNECT, disconn_req_encode, disconn_req_decode, NULL, NULL},
	{MULTIMEDIA_ADD_ACTION, add_req_encode, add_req_decode, add_resp_encode, add_resp_decode},
	{MULTIMEDIA_MODIFY_ACTION, modify_req_encode, modify_req_decode, modify_resp_encode, modify_resp_decode},
	{MULTIMEDIA_DELETE_ACTION, del_req_encode, del_req_decode, del_resp_encode, del_resp_decode},
	{MULTIMEDIA_SEARCH_ACTION_BY_PAGE, search_page_req_encode, search_page_req_decode, search_page_resp_encode, search_page_resp_decode},
	{MULTIMEDIA_GET_ORDERS, NULL, NULL, get_order_resp_encode, get_order_resp_decode},
	{MULTIMEDIA_SEARCH_ACTION_BY_ORDER, search_order_req_encode, search_order_req_decode, search_order_resp_encode, search_order_resp_decode},
	{MULTIMEDIA_GET_ACTION_NAMES, NULL, NULL, get_name_resp_encode, get_name_resp_decode},
	{MULTIMEDIA_SEARCH_ACTION_BY_NAME, search_name_req_encode, search_name_req_decode, search_name_resp_encode, search_name_resp_decode},
	{MULTIMEDIA_UPLOAD, NULL, NULL, upload_resp_encode, upload_resp_decode},
	{MULTIMEDIA_DOWNLOAD, NULL, NULL, download_resp_encode, download_resp_decode},
	{MULTIMEDIA_ACTIVATION, activation_req_encode, activation_req_decode, activation_resp_encode, activation_resp_decode},
};

/* protobuf初始化 */
void protob_init(void)
{
	msghdl_array_register(msghdl, MSGHDL_ARRSIZE(msghdl));
}