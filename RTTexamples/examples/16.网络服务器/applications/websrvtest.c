/****************************2012-2013, NJUT, Edu.****************************** 
FileName: clienttest.c 
Author:  �ﶬ÷       Version :  1.0        Date: 2014.07.30
Description:    ʹ����С�޶�APIʵ�ֵ�һ���򵥵�HTTP/1.0������  

Version:         1.0 
History:         
      <author>  <time>   <version >   <desc> 
      Sundm    14/07/30     1.0     �ļ�����   
********************************************************************************/ 
#include <lwip/api.h>
#include <finsh.h> 

/*  ʵ�ʵ�webҳ�����ݡ��󲿷ֵı������Ὣ��Щ���ݷ���ROM�� */
ALIGN(4)
const static char indexdata[] = "<html> \
	<head><title>A test page</title></head> \
	<body> \
	This is a small test page.  \
         Made by sundm75.    \
           zdh.njut.edu.cn  \
	</body> \
	</html>";
ALIGN(4)
const static char http_html_hdr[] = "Content-type: text/html\r\n\r\n";

/*  ������������ */
static void process_connection(struct netconn *conn)
{
	struct netbuf *inbuf;
	char *rq;
	rt_uint16_t len;
        rt_err_t net_rev_result;

	/*  ��������Ӷ�ȡ���ݵ�inbuf�����Ǽٶ������netbuf�а������������� */
        net_rev_result = netconn_recv(conn, &inbuf);

	/*  ��ȡָ��netbuf�е�һ������Ƭ�ϵ�ָ�룬���������Ƭ��������ϣ������������� */
	netbuf_data(inbuf, (void**)&rq, &len);

	/*  �����������ǲ���HTTP "GET /\r\n"  */
	if( rq[0] == 'G' &&
		rq[1] == 'E' &&
		rq[2] == 'T' &&
		rq[3] == ' ')
	{
		/*  ����ͷ������ */
		netconn_write(conn, http_html_hdr, sizeof(http_html_hdr), NETCONN_NOCOPY);
		/*  ����ʵ�ʵ�webҳ�� */
		netconn_write(conn, indexdata, sizeof(indexdata), NETCONN_NOCOPY);

		/*  �ر����� */
		netconn_close(conn);
	}
	netbuf_delete(inbuf);
}

/* �߳���� */
void lw_thread(void* paramter)
{
	struct netconn *conn, *newconn;
        rt_err_t net_acp_result;

	/*  ����һ���µ�TCP���Ӿ�� */
	conn = netconn_new(NETCONN_TCP);

	/*  �����Ӱ�������ı���IP��ַ��80�˿��� */
	netconn_bind(conn, NULL, 80);

	/*  ���ӽ������״̬ */
	netconn_listen(conn);

	/*  ѭ������ */
	while(1)
	{
		/*  �����µ��������� */
		net_acp_result = netconn_accept(conn,&newconn);

		/*  ������������ */
		process_connection(newconn);

		/*  ɾ�����Ӿ�� */
		netconn_delete(newconn);
                rt_kprintf("TCP/IP counted!\n");//�߳�����

	}
}

void test_websrv(void)
{
	rt_thread_t tid;

	tid = rt_thread_create("websrv", lw_thread, RT_NULL,
		1024, 25, 5);
	if (tid != RT_NULL) rt_thread_startup(tid);
}


FINSH_FUNCTION_EXPORT(test_websrv, startup a simple web server e.g.test_websrv()); 