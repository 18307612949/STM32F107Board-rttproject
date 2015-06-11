#ifndef COMMON_H
#define COMMON_H

#include <rtthread.h>
#include "ctype.h"
#include "string.h"
#if defined(RT_USING_DFS)
#include <dfs_posix.h>
#endif

//��16�����ַ���ת����16�������磺"12" ת����0x12
void  ASCIItoHEX(unsigned char* arrayin, unsigned char *arrayout);

//��16������ת����16�����ַ����磺12 ת����"12"
void  HEXtoASCII(unsigned char* arrayin, unsigned char *arrayout);
void  HEXtonASCII(unsigned char* arrayin, unsigned char *arrayout, unsigned char len);
//��һ���ַ���s2�ӵ���һ���ַ���s1���棬����0x00
void insertstr(char *s1,char *s2,int l1,int l2);

//����ַ����Ƿ�Ϊ��ĸ��������
char isstring(char* p);

//���ַ�����ĳ���ַ�ɾ��
void strcut(char*p,char c);

//���ַ���ɾ��ָ�������ַ�
void DeleteString(unsigned char* array, unsigned int arraylen, unsigned int beginaddr, unsigned int dellen);

//��һ���ַ������뵽��һ���ַ������ƶ�λ��
void insert(char *s1,char *s2,int f);

//ȥ���ַ����з���ĸ�������ֵ��ַ�
void strcutnochar(char*p);

//���㻺��
void clrbuf(char*buf,int num);
//���س����б�ɿո�

void strrep(char*p);

//���ַ���ӡ
void printchar(char *buffer,int len);

//�ַ����Ƚϣ�������
char  compstr(char *buffer1,char *buffer2,int len);



#endif
