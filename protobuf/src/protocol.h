/*
* date: 2018/1/25
* file: protocol.h
* author: dengzhiqin
* describe: protocol.c的头文件
*/
#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _cmag {
	char sess_id[37];
	int type;
	int sn;
	void *data;
} cmsg_t ;

typedef struct _smsg {
	int type;
	int sn;
	int updflags;
	int respcode;
	void *data;
} smsg_t;

/*
* @func: 释放结构体cmsg_t指针内存
* @param: arg [in] 结构体cmsg_t指针
*/
void cmsg_free(cmsg_t *arg);

/*
* @func: 释放结构体smsg_t指针内存
* @param: arg [in] 结构体smsg_t指针
*/
void smsg_free(smsg_t *arg);

/*
* @func: 客户端发往服务器的消息 编码
* @param: type [in] 消息类型,32位整数
* @param: args [in] 原始数据,以结构体指针的形式传参
* @param: count [out] 编码后字节流的字节数
* @param: buf [out] 编码后的字节流
* @return: 成功返回编码之后的字节流指针,该指针必须要用free()释放内存空间,
*          失败返回NULL.
*/
char *cli_to_serv_encode(int type, void *args, int *count, char **buf);

/*
* @func: 客户端发往服务器的消息 解码
* @param: buf [in] 需要解码的字节流
* @param: size [in] buf的字节数
* @return: 成功返回解码后的cmsg_t结构体指针,该指针必须用cmsg_free()释放内存空间,
           失败返回NULL.
*/
cmsg_t *cli_to_serv_decode(const char *buf, int size);


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
char *serv_to_cli_encode(int type, int sn, unsigned int upd, short resp, void *args, int *count, char **buf);


/*
* @func: 服务器发往客户端的消息 解码
* @param: buf [in] 需要解码的字节流
* @param: size [in] buf的字节数
* @return: 成功返回解码后的smsg_t结构体指针,该指针必须用smsg_free()释放内存空间,
           失败返回NULL.
*/
smsg_t *serv_to_cli_decode(const char *buf, int size);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
