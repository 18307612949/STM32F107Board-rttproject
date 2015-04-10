/****************************2012-2013, NJUT, Edu.****************************** 
FileName: filetest.c 
Author:  �ﶬ÷       Version :  1.0        Date: 2014.07.30
Description:    �ļ�ϵͳ���Գ��� 

Version:         1.0 
History:         
      <author>  <time>   <version >   <desc> 
      Sundm    14/07/30     1.0     �ļ�����   
********************************************************************************/ 
#include <dfs_posix.h> /* ����Ҫʹ���ļ�����ʱ����Ҫ�������ͷ�ļ�*/
/* �����ļ���������һ����������� */
void test_file()
{
	int fd;
	char s[] = "RT-Thread Programmer!", buffer[80];
	/* �� /text.txt ��д�룬������ļ��������������ļ� */
	fd = open("/text.txt", O_WRONLY | O_CREAT,0);
	if (fd >= 0)
	{
		write(fd, s, sizeof(s));
		close(fd);
	}
	/* �� /text.txt ׼������ȡ���� */
	fd = open("/text.txt", O_RDONLY,0);
	if (fd >= 0)
	{
		read(fd, buffer, sizeof(buffer));
		close(fd);
	}
	rt_kprintf("%s", buffer);
}

#include "finsh.h"
FINSH_FUNCTION_EXPORT(test_file, file system test e.g. test_file());

