/*
* date: 2018/1/25
* file: util.h
* author: dengzhiqin
* describe: util.c的头文件
*/
#ifndef __UTIL_H
#define __UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

/*
* 短整型转为字节流
*/
void short2bytes(short num, char *bytes);

/*
* 整型转为字节流
*/
void int2bytes(int num, char *bytes);

/*
* 长整型转为字节流
*/
void long2bytes(unsigned long long num, char *bytes);

/*
* 字节流转为短整型
*/
short int recoverNetworkBytesOrderShort(const char *orderBytes, int begindex);

/*
* 字节流转为整型
*/
int recoverNetworkBytesOrderInt(const char *orderBytes, int begindex);

/*
* 字节流转为长整型
*/
unsigned long long recoverNetworkBytesOrderLong(const char* orderBytes, int begindex);

/*
* 计算校验码
*/
unsigned long long get_adler32(unsigned char *buf, int len);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
