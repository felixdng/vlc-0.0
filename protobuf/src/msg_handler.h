/*
* date: 2018/1/25
* file: msg_handler.h
* author: dengzhiqin
* describe: msg_handler.c的头文件
*/
#ifndef __MSG_HANDLER_H
#define __MSG_HANDLER_H

#define MULTIMEDIA_CONNECT                 0xF010F001
#define MULTIMEDIA_DISCONNECT              0xF010F002
#define MULTIMEDIA_ADD_ACTION              0xF010F003
#define MULTIMEDIA_MODIFY_ACTION           0xF010F004
#define MULTIMEDIA_DELETE_ACTION           0xF010F005
#define MULTIMEDIA_SEARCH_ACTION_BY_PAGE   0xF010F006
#define MULTIMEDIA_GET_ORDERS              0xF010F007
#define MULTIMEDIA_SEARCH_ACTION_BY_ORDER  0xF010F008
#define MULTIMEDIA_GET_ACTION_NAMES        0xF010F009
#define MULTIMEDIA_SEARCH_ACTION_BY_NAME   0xF010F00A
#define MULTIMEDIA_UPLOAD                  0xF010F00B
#define MULTIMEDIA_DOWNLOAD                0xF010F00C
#define MULTIMEDIA_ACTIVATION              0xF010F00D

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _msg_handler {
	int type;

	/*
	* @func: protobuf 编码
	* @param: src [in] 原数据
	* @param: dest [out] 序列化之后的字符数组
	* @param: count [out] dest的字节数
	* @return: 成功返回dest,该指针必须用free()释放内存空间;失败返回NULL
	*/
	char *(*req_encode)(const void *src, char **dest, int *count);

	/*
	* @func: protobuf 解码
	* @param: buff [in] 序列化的字符数组
	* @param: len [in] buff的字节数
	* @return: 成功返回解码后的结构体指针,该指针必须用free()释放内存空间;失败返回NULL
	*/
	void *(*req_decode)(const char *buff, int len);

	/* 同req_encode */
	char *(*resp_encode)(const void *src, char **dest, int *count);

	/* 同req_decode */
	void *(*resp_decode)(const char *buff, int len);
} msg_handler_t;


#define MSGHDL_ARRSIZE(_arry) \
	( sizeof(_arry) / sizeof(_arry[0]) )

/*
* @func: 注册消息处理
* @param: hdls [in] msg_handler_t数组
* @param: size [in] msg_handler_t数组元素个数
*/
void msghdl_array_register(msg_handler_t *hdls, int size);

/*
* @func: 根据消息类型获取一个消息处理
* @param: type [in] 消息类型
* @param: hdl [out] 查询到的消息处理
* @rerurn: 成功返回0,失败返回-1 
*/
int msghdl_array_get(int type, msg_handler_t *hdl);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
