/*
* date: 2018/1/25
* file: util.c
* author: dengzhiqin
* describe: 通用工具接口
*/
#include <stdio.h>
#include <stdlib.h>


#define ADLER32_BASE 65521

/* 一个字节有8位 */
#define BITS_PER_BYTE 8

/* short型2个字节,int型4个字节,long型8个字节 */
#define SHORT_BYTES  2
#define INT_BYTES    4
#define LONG_BYTES   8

/*
* 短整型转为字节流
*/
void short2bytes(short num, char *bytes)
{
	int i;
	for (i = 0; i < SHORT_BYTES; i++) {
		bytes[i] = (num >> (BITS_PER_BYTE * (SHORT_BYTES - i - 1))) & 0xff;
	}
}

/*
* 整型转为字节流
*/
void int2bytes(int num, char *bytes)
{
	int i;
	for (i = 0; i < INT_BYTES; i++) {
		bytes[i] = (num >> (BITS_PER_BYTE * (INT_BYTES - i - 1))) & 0xff;
	}
}

/*
* 长整型转为字节流
*/
void long2bytes(unsigned long long num, char *bytes)
{
	int i;
	for (i = 0; i < LONG_BYTES; i++) {
		bytes[i] = (num >> (BITS_PER_BYTE * (LONG_BYTES - i - 1))) & 0xff;
	}
}

/*
* 字节流转为短整型
*/
short int recoverNetworkBytesOrderShort(const char *orderBytes, int begindex)
{
	short int sourceNumber = 0;
	int i;
	for (i = begindex; i < begindex + SHORT_BYTES; i++) {
		sourceNumber <<= BITS_PER_BYTE;
		sourceNumber |= (orderBytes[i] & 0xff);
	}
	return sourceNumber;
}

/*
* 字节流转为整型
*/
int recoverNetworkBytesOrderInt(const char *orderBytes, int begindex)
{
	int sourceNumber = 0;
	int i;
	for (i = begindex; i < begindex + INT_BYTES; i++) {
		sourceNumber <<= BITS_PER_BYTE;
		sourceNumber |= (orderBytes[i] & 0xff);
	}
	return sourceNumber;
}

/*
* 字节流转为长整型
*/
unsigned long long recoverNetworkBytesOrderLong(const char* orderBytes,
		int begindex)
{
	unsigned long long sourceNumber = 0;
	int i;
	for (i = begindex; i < begindex + LONG_BYTES; i++) {
		sourceNumber <<= BITS_PER_BYTE;
		sourceNumber |= orderBytes[i] & 0xff;
	}
	return sourceNumber;
}

/*
* 计算校验码
*/
unsigned long long get_adler32(unsigned char *buf, int len)
{
	unsigned long long adler = 1;
	unsigned long long s1 = adler & 0xffff;
	unsigned long long s2 = (adler >> 16) & 0xffff;

	int i;
	for (i = 0; i < len; i++) {
		s1 = (s1 + buf[i]) % ADLER32_BASE;
		s2 = (s2 + s1) % ADLER32_BASE;
	}
	return (s2 << 16) + s1;
}
