/*
 * ���� �ۼ���	:	���ؿ�
 * ���� �ۼ���	:	2019.12.28
 * ���� ������	:	2020.03.22
 * ����			:	�ڷ��� ���� �� �⺻�� �Լ� ��ȯ�� ��ġȭ
 * ���� �̷�	:	���ؿ�, 2019.12.28.(ver. 01)
					���ؿ�, 2019.12.28.(ver. 02)	:	��� �� ����, ����, ���� ���� ��ȯ�� ��ġȭ
					����, 2019.12.28.(ver. 03)	:	��� �ּ� ����
					���ؿ�, 2019.12.28.(ver. 04)	:	��ü�� �ּ� ����
					���ؿ�, 2020.02.02.(ver. 05)	:	time.h �߰�
					���ؿ�, 2020.02.06.(ver. 06)	:	windows������ �� ���� ���� �ذ�
					���ؿ�, 2020.02.07.(ver. 07)	:	false �� true ����
					���ؿ�, 2020.02.10.(ver. 08)	:	stdio.h �� string.h �߰�
					���ؿ�, 2020.02.15.(ver. 09)	:	���� ������ ����
					���ؿ�, 2020.02.17.(ver. 10)	:	�Ű������� IN, OUT ����
					���ؿ�, 2020.02.20.(ver. 11)	:	�� ���ǵ� �ڷ������� ��ȯ �� ���� ���� ���� ����
					���ؿ�, 2020.02.20.(ver. 12)	:	BITSTOBYTE ���� ��ġ ���� �� HEXNUMBITS, HEXCOUNT, CHARBITS ���Ӱ� ����
					���ؿ�, 2020.03.15.(ver. 13)	:	���ĪŰ ��ȣȭ���� �ʿ��� PADDING ���� ���� �߰�
					���ؿ�, 2020.03.22.(ver. 14)	:	BaseDefine.h���� ����(FAIL�� ERROR�� ����)
					���ؿ�, 2020.03.24.(ver. 15)	:	Linux���� ������� ���� ���� ó��
 * ���۱�		:	White
 */

#ifndef _BASE_DEFINE_H_
#define _BASE_DEFINE_H_

#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

#ifndef DATA_TYPE
#define DATA_TYPE
typedef int8_t						S_CHAR;		//signed char, 1Byte
typedef int16_t						S_SHORT;	//signed short,2Byte
typedef int32_t						S_INT;		//signed int,  4Byte
typedef int64_t						S_LONG;		//signed long long, 8Byte
typedef uint8_t						U_CHAR;		//unsigned char, 1Byte
typedef uint16_t					U_SHORT;	//unsigned short,2Byte
typedef uint32_t					U_INT;		//unsigned int, 4Byte
typedef uint64_t					U_LONG;		//unsigned long,8Byte

#define U_CHAR_MAX					(0xff)					//	unsigned char type�� �ִ� ũ��
#define U_SHORT_MAX					(0xffff)				//	unsigned short type�� �ִ� ũ��
#define U_INT_MAX					(0xffffffff)			//	unsigned int type�� �ִ� ũ��
#define U_LONG_MAX					(0xffffffffffffffff)	//	unsigned long type�� �ִ� ũ��
#define S_CHAR_MAX					((U_CHAR_MAX)	>> 1)	//	signed char type�� �ִ� ũ��
#define S_SHORT_MAX					((U_SHORT_MAX) >> 1)	//	signed short type�� �ִ� ũ��
#define S_INT_MAX					((U_INT_MAX) >> 1)		//	signed int type�� �ִ� ũ��
#define S_LONG_MAX					((U_LONG_MAX) >> 1)		//	signed long type�� �ִ� ũ��
#endif // !DATA_TYPE

#ifndef SUCCESS
#define SUCCESS						(1)						//	�Լ����� ��� ���� ���������� �������� ���
#endif // !SUCCESS

#ifndef ERROR
#define ERROR						(0)
#endif // !ERROR

#ifndef WARNINGS
#define WARNINGS					(((ERROR)) - (1))		//	������ �߻��Ͽ����� �������ᰡ �ʿ� ���� ���
#endif // !WARNINGS

#ifndef FAIL
#define FAIL						((ERROR))				//	�Լ������� ������ �߻��Ͽ��� �������ᰡ �ʿ��� ���
#endif // !FAIL

#ifndef _MINWINDEF_
#ifndef TURE_AND_FALSE
#define TRUE_AND_FALSE
#define TRUE						(1)						//	����(����)
#define FALSE						((TRUE) - (1))			//	�ٸ�(����)
#endif
#endif // !_MINWINDEF_

#ifndef BITS_TO_CHAR
#define BITS_TO_CHAR
#define BITS_TO_BYTE				(8)						//	1byte �� bit ��
#define HEX_NUM_BITS				(4)						//	hex���� ǥ���ϱ� ���ؼ� ���Ǵ� bit ��
#define HEX_COUNT					((BITS_TO_BYTE) / (HEX_NUM_BITS))	//	1byte�� ǥ���Ǵ� hex�� �ڸ���
#define CHAR_BITS					((sizeof(S_CHAR)) * (HEX_COUNT))	//	1 S_CHAR�� ǥ���Ǵ� hex�� �ڸ���
#endif // !BITS_TO_CHAR

#ifndef MESSAGE_LEN
#define MESSAGE_LEN
#define ASYM_PADDING_LEN			(11)					// PADDING ����
#define CRYPTO_MESSAGE_LEN			(256)					//	��ȣȭ�� �޼����� ����
#define PLAIN_TEXT_MESSAGE_LEN		(((CRYPTO_MESSAGE_LEN) - (ASYM_PADDING_LEN)) + 1)	//	��ȣȭ�� ������ ���� �޼��� �ִ� ����
#define PLAIN_TEXT_MESSAGE_LEN_HALF	((((PLAIN_TEXT_MESSAGE_LEN) % 2) == 0) ? (((PLAIN_TEXT_MESSAGE_LEN) >> 1) - 1) : (((PLAIN_TEXT_MESSAGE_LEN) - 1) >> 1))	//	��ȣȭ�� ������ ���� �޼��� �ִ� ����
#define SEND_MESSAGE_LEN			(((CRYPTO_MESSAGE_LEN) << 2) + 1)	//	���۵Ǵ� �޼����� �ִ� ũ��
#endif // !MESSAGE_LEN

#define IN							const					//	�Է¹޴� �Ű������� ���
#define OUT													//	��¹޴� �Ű������� ���

#endif // !_BASE_DEFINE_H_
