/*
* date: 2018/1/25
* file: msg_handler.c
* author: dengzhiqin
* describe: protobuf消息的处理
*/
#include <stdio.h>
#include <stdlib.h>
#include "msg_handler.h"


#define HDL_ARR_SIZE 100
static msg_handler_t msg_hdl_arr[HDL_ARR_SIZE];
static int hdl_arr_count = 0;

/*
* @func: 增加一个消息处理
* @param: hdl [i] 消息处理
* @return: 成功返回0,失败返回-1
*/
static int msghdl_array_add(msg_handler_t hdl)
{
	if (hdl_arr_count >= HDL_ARR_SIZE)
		return -1;

	msg_hdl_arr[hdl_arr_count].type = hdl.type;
	msg_hdl_arr[hdl_arr_count].req_encode = hdl.req_encode;
	msg_hdl_arr[hdl_arr_count].req_decode = hdl.req_decode;
	msg_hdl_arr[hdl_arr_count].resp_encode = hdl.resp_encode;
	msg_hdl_arr[hdl_arr_count].resp_decode = hdl.resp_decode;
	hdl_arr_count++;
	return 0;
}

/*
* @func: 注册消息处理
* @param: hdls [in] msg_handler_t数组
* @param: size [in] msg_handler_t数组元素个数
*/
void msghdl_array_register(msg_handler_t *hdls, int size)
{
	int i;
	for (i = 0; i < size; i++) {
		if (0 != msghdl_array_add(hdls[i]))
			break;
	}
}

/*
* @func: 根据消息类型获取一个消息处理
* @param: type [in] 消息类型
* @param: hdl [out] 查询到的消息处理
* @rerurn: 成功返回0,失败返回-1 
*/
int msghdl_array_get(int type, msg_handler_t *hdl)
{
	int i;
	for (i = 0; i < hdl_arr_count; i++) {
		if (type == msg_hdl_arr[i].type) {
			hdl->type = msg_hdl_arr[i].type;
			hdl->req_encode = msg_hdl_arr[i].req_encode;
			hdl->req_decode = msg_hdl_arr[i].req_decode;
			hdl->resp_encode = msg_hdl_arr[i].resp_encode;
			hdl->resp_decode = msg_hdl_arr[i].resp_decode;
			return 0;
		}
	}
	return -1;
}
