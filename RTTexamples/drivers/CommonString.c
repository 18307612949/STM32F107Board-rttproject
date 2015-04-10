/*********************2012-2013, NJUT, Edu.********************* 
FileName: CommonString.c 
Author:  �ﶬ÷       Version :  1.0        Date: 2013.06.10
Description:    ͨ���ַ�������     
Version:         1.0 
History:         
      <author>  <time>   <version >   <desc> 
      Sundm    13/04/10     1.0     �ļ�����   
***************************************************************/ 
#include "CommonString.h"


/*******************************************************************************
* Function Name  : strcut
* Description    : ɾ���ַ����е�ĳ���ַ�
* Input          : p �����ַ��� c ��ɾ�����ַ�
* Output         : p 
* Return         : None
*******************************************************************************/
void strcut(char*p,char c)
{
    	while(*p!=0)    //û�н�����ѭ��
	{
		if(*p==c)  //�����ַ�c����
		{
			char *q=p;  //���ַ�c����ʼ
			while(*q!=0) //ֱ��ĩβ�������ַ�
			{
				*q=*(q+1); //���ǰ��
				q++;      //ÿ��һ���ַ���ָ���1��׼������һ���ַ�
			}
		}  
		else        //��ǰ�ַ����ǿո�
		{
			p++;  //ָ����ƣ�ָ����������ַ�
		}
	}
}

/*******************************************************************************
* Function Name  : DeleteString
* Description    : ���ַ���ɾ��ָ�������ַ�
* Input          : beginaddr ��ʼɾ����λ�ã�dellen ɾ���ַ������ȡ�
* Output         : p 
* Return         : None
*******************************************************************************/
void DeleteString(unsigned char* array, unsigned int arraylen, unsigned int beginaddr, unsigned int dellen)
{
  int length;
	int i;
  length = arraylen - beginaddr - dellen;
  array = array + beginaddr;
  if( beginaddr < arraylen && length >=0 )
  {
    for( i = 0; i < length; i++)
    {
      *array = *(array + dellen ); 
      array++;
    }
    for( i = length; i < arraylen; i++)
    {
      *array = 0x00; 
      array++;
    }
  }
}

/*******************************************************************************
* Function Name  : insert
* Description    : ��һ���ַ������뵽��һ���ַ������ƶ�λ��
* Input          : s1Ŀ���ַ��� s2Դ�ַ��� f������λ�� 
* Output         : None 
* Return         : None
*******************************************************************************/
void insert(char *s1,char *s2,int f)
{
 int i,l1=0,l2=0;
 char *p;
 for(p=s1;*p!=0;p++)
  l1++;
 for(p=s2;*p!=0;p++)
  l2++;
 for(i=l1+l2;i>=f+l2;i--)
 {
  *(s1+i)=*(s1+l1);
  *(s1+l1)=*(s2+l2-1);
  l1--;
  l2--;
 }
}
//��һ���ַ���s2�ӵ���һ���ַ���s1���棬����0x00
void insertstr(char *s1,char *s2,int l1,int l2)
{
 int i;
 for (i=0;i<l2;i++)
 {
   *(s1+l1+i)=*(s2+i);
 }
}

void clrbuf(char*buf,int num)
{
  memset(buf,0,num);
}
//���س��ͻ��ж����ɿո�
void strrep(char*p)
{
    	while(*p!=0)    //û�н�����ѭ��
	{
		if((*p==0x0a)||(*p==0x0d))  //�����ո���
		{
                  *p = 0x20;
		}
		p++;  //ָ����ƣ�ָ����������ַ�
	}
}


//����ַ����Ƿ�Ϊ��ĸ��������
char isstring(char*p)
{
    	while(*p!=0)    //û�н�����ѭ��
	{
		if( ! isalnum (*p) )  //����ַ�Ϊ����ĸ�������֣�����0
		{
                  return RT_FALSE;
		}
		p++;  //ָ����ƣ�ָ����������ַ�
	}
        return RT_TRUE;
}
//ȥ���ַ����з���ĸ�������ֵ��ַ�
void strcutnochar(char* p)
{
    	while(*p!=0)    //û�н�����ѭ��
	{
		if( ! isalnum (*p) )  //����ַ�Ϊ����ĸ�������֣�����0
		{
                  strcut( p,*p);
                  p--;
		}
		p++;  //ָ����ƣ�ָ����������ַ�
	}
}
//��16�����ַ���ת����16�������磺"12" ת����0x12 ����˫�ַ��Ĳ�0
void ASCIItoHEX(unsigned char* arrayin, unsigned char *arrayout)
{
    unsigned char  temp = 0,n = 0;
    while(*arrayin!=0)    //û�н�����ѭ��
    {
      if(*arrayin >= 'A'&& *arrayin <= 'F') //ʮ�����ƻ�Ҫ�ж����ǲ�����A-F����a-f֮��a=10����
         n = *arrayin - 'A' + 10;
       else if(*arrayin >= 'a'&& *arrayin <= 'f')
         n = *arrayin - 'a' + 10;
       else n = *arrayin - '0';
        arrayin++;
      if(*arrayin!=0)  
      {
        if(*arrayin >= 'A'&& *arrayin <= 'F') //ʮ�����ƻ�Ҫ�ж����ǲ�����A-F����a-f֮��a=10����
           temp = *arrayin - 'A' + 10;
         else if(*arrayin >= 'a'&& *arrayin <= 'f')
           temp = *arrayin - 'a' + 10;
         else temp = *arrayin - '0';
      }
         
        temp = temp + n*16;
        *arrayout = temp;
        arrayin++;
        arrayout++;
        temp = 0;n = 0;
    }
}
//��16������ת����16�����ַ����磺12 ת����"12"
void  HEXtoASCII(unsigned char* arrayin, unsigned char *arrayout)
{
    unsigned char  temph = 0,templ = 0;
    while(*arrayin!=0)    //û�н�����ѭ��
    {
      temph = (*arrayin &0xF0) >>4;
      templ = *arrayin & 0x0F;
      
      if(temph <=0x09)
      *arrayout = temph + 0x30;
      else if(temph >= 0x0A && temph <=0x0F)
      *arrayout = temph -0x0A + 'A';
      else 
      *arrayout = 0x30;
      arrayout++;
      
      if(templ <=0x09)
      *arrayout = templ + 0x30;
      else if(templ >= 0x0A && templ <=0x0F)
      *arrayout = templ -0x0A + 'A';
      else 
      *arrayout = 0x30;
      arrayout++;
      arrayin++;
    }
}
//��16������ת����16�����ַ����磺0x12 ת����"12" lenΪ����16�������ݳ���
void  HEXtonASCII(unsigned char* arrayin, unsigned char *arrayout, unsigned char len)
{
    unsigned char  temph = 0,templ = 0;
    while(len)    //û�н�����ѭ��
    {
      temph = (*arrayin &0xF0) >>4;
      templ = *arrayin & 0x0F;
      
      if(temph <=0x09)
      *arrayout = temph + 0x30;
      else if(temph >= 0x0A && temph <=0x0F)
      *arrayout = temph -0x0A + 'A';
      else 
      *arrayout = 0x30;
      arrayout++;
      
      if(templ <=0x09)
      *arrayout = templ + 0x30;
      else if(templ >= 0x0A && templ <=0x0F)
      *arrayout = templ -0x0A + 'A';
      else 
      *arrayout = 0x30;
      arrayout++;
      arrayin++;
      len--;
    }
}

//���ַ���ӡ
void printchar(char *buffer,int len)
{
  	int i;    
	for( i=0;i<len;i++)
      {
        rt_kprintf("%c",buffer[i]);
      }
}

//�ַ����Ƚϣ�������
char  compstr(char *buffer1,char *buffer2,int len)
{
	int i;
	if(strlen(buffer1)==len)
      {
          for( i=0;i<len;i++)
          {
            if(*buffer1 != *buffer2)
            {
              return 0;
            }
            buffer1++;buffer2++;
          }
      }     
      else
        return 0;
      return 1;
}



