/****************************2012-2014, NJUT, Edu.****************************** 
FileName: porttcp.c 
Author:  �ﶬ÷       Version :  1.0        Date: 2014.10.30
Description:   Modbus TCP�������������� ������LWIP��д��
1.��ʼ��,����502�˿ڣ���������ӵ������������ӣ������ӽ�������.
2.���������ں���xMBTCPPortGetRequest�а�Ǩ��FreeModbus.
3.Ҫ���͵�������xMBTCPPortSendResponse�д�FreeModbus��Ǩ����������TCP/IP�з��ͳ�ȥ��

Version:         1.0 
History:         
      <author>  <time>   <version >   <desc> 
      Sundm    14/10/30     1.0     �ļ�����   
********************************************************************************/ 
/* -------------------------- LWIP includes ---------------------------------*/
#include <lwip/api.h>
#include <finsh.h> 

/* ----------------------- System includes ----------------------------------*/
#include <stdio.h>
#include <string.h>
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- Defines  -----------------------------------------*/
#define MB_TCP_DEFAULT_PORT  502          /* TCP listening port. */
#define MB_TCP_BUF_SIZE     ( 256 + 7 )   /* Must hold a complete Modbus TCP frame. */

/* ----------------------- Prototypes ---------------------------------------*/
static  UCHAR    ucTCPRequestFrame[MB_TCP_BUF_SIZE];
static  USHORT   ucTCPRequestLen;

static  UCHAR    ucTCPResponseFrame[MB_TCP_BUF_SIZE];
static  USHORT   ucTCPResponseLen;

/* ----------------------- Static variables ---------------------------------*/
static  USHORT usPort;

/* ----------------- Net Server  Defines & variables-------------------------*/
static struct netconn* newconn = RT_NULL;

#define NW_RX      0x01
#define NW_TX      0x02
#define NW_CLOSED   0x04
#define NW_MASK      (NW_RX | NW_TX | NW_CLOSED)

#define ETH_LINK_ERR -1;
#define ETH_LINK_OK 0;

struct rt_event nw_event;

/* ----------------------- Static functions ---------------------------------*/

/*******************************************************************************
* Functions Name : rx_callback; process_rx_data; nw_thread
* Description    : �������ݽ��ջص�����; ������յ�����; ���������̺߳���
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
/*  �������ݽ��ջص����� */
void rx_callback(struct netconn *conn, enum netconn_evt evt, rt_uint16_t len)
{
   if (evt == NETCONN_EVT_RCVPLUS)
   {
      rt_event_send(&nw_event, NW_RX);
   }
}

/*  ������յ����� */
void process_rx_data(struct netbuf *buffer)
{
    rt_uint8_t *data;
    rt_uint16_t length;
    int i;    

    /* get data */
    netbuf_data(buffer, (void**)&data, &length);
    rt_kprintf("\r\n Modbus TCP receive len = %d, rx data:\n",length);
    for( i=0; i<length; i++)
    {
      rt_kprintf("0x%02X ",data[i]);
    }
    rt_kprintf("\r\n ");
    
    /* ���modbus���� */
    memcpy(ucTCPRequestFrame, data, length );
    ucTCPRequestLen = length;
    
    /* �� modbus poll������Ϣ */
    xMBPortEventPost( EV_FRAME_RECEIVED );
}

/*  �������ݷ���,����,�ر��߳� */
void nw_thread(void* parameter)
{
   struct netbuf *buf;
   int i;
   rt_err_t result;
   rt_uint32_t event;
   rt_err_t net_rev_result;

   /* set network rx call back */
   newconn->callback = rx_callback;

  /*  ѭ������ֱ���̹߳ر�*/
   while (1)
   {
      /* receive network event */
      result = rt_event_recv(&nw_event, 
      NW_MASK, RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, 
      RT_WAITING_FOREVER, &event);
      if (result == RT_EOK)
      {
         /* get event successfully */
         if (event & NW_RX)
         {
            /* �������ݲ����� */
            net_rev_result = netconn_recv(newconn, &buf);
            if (buf != RT_NULL)
            {
               process_rx_data(buf);
            }
            netbuf_delete(buf);
         }
         
         if (event & NW_TX)
         {
            /* LWIP����ModbusӦ�����ݰ� */
            netconn_write(newconn,  ucTCPResponseFrame , ucTCPResponseLen, NETCONN_NOCOPY );
            rt_kprintf("\r\n Modbus TCP send len = %d, send data:\n",ucTCPResponseLen);
            for( i=0; i<ucTCPResponseLen; i++)
            {
              rt_kprintf("0x%02X ",ucTCPResponseFrame[i]);
            }
            rt_kprintf("\r\n ");
         }
         
         if (event & NW_CLOSED)
         {
            /* connection is closed */
            netconn_close(newconn);
            netconn_delete(newconn);
            rt_event_detach(&nw_event);
            return;
         }
      }
   }
}


/*******************************************************************************
* Functions Name : checkethlink; mbtcp_srv_thread
* Description    : ����������ѯ����״̬; modbus tcp �������߳�
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
/*����������ѯ����״̬*/
static int checkethlink(void)
{
    int temp;
    extern uint16_t ETH_ReadPHYRegister(uint16_t PHYAddress, uint16_t PHYReg);
    int eth_state = ETH_LINK_ERR;
    temp = ETH_ReadPHYRegister(1, 1);
    temp = ETH_ReadPHYRegister(1, 1);
    if(temp&0x04)//���BMSR�ĵ�2λ link״̬��Ϊ1ʱ���ӣ�Ϊ0ʱ�Ͽ�
    {
      eth_state = ETH_LINK_OK;
    }
    return eth_state;
}

/* modbus tcp �������߳� */
void mbtcp_srv_thread(void* paramter)
{
  rt_err_t net_acp_result;
  struct netconn *conn;
  rt_thread_t thread;

  /*  ����һ���µ�TCP���Ӿ�� */
  conn = netconn_new(NETCONN_TCP);

  /*  �����Ӱ�������ı���IP��ַ��502�˿��� */
  netconn_bind(conn, NULL, usPort);

  /*  ���ӽ������״̬ */
  netconn_listen(conn);

  /*  ѭ������ */
  while(1)
  {
    /*  �����µ��������� �������� */
    net_acp_result = netconn_accept(conn,&newconn);

    rt_kprintf("connect error code: %d\n", net_acp_result);

   if (net_acp_result == 0)
   {
      rt_kprintf("Modbus Tcp server OK, startup rx thread\n");
      
      /* init event */
      rt_event_init(&nw_event, "nw_event", RT_IPC_FLAG_FIFO);

      /* create a new thread */
      thread = rt_thread_create("rx", nw_thread, RT_NULL,
         0x800, 20, 20);
      if (thread != RT_NULL)
         rt_thread_startup(thread);
   }

     /*�����������Ǽ�⵽�������ӶϿ��������߰ε������ڴ�ѭ��  */
    while( !((netconn_err(newconn) != ERR_OK )||((checkethlink()!= ERR_OK))))
    {
      rt_thread_delay(RT_TICK_PER_SECOND);
    }

    /*�Ͽ����� �رղ�ɾ������ ׼�������µ�����*/
    rt_event_send(&nw_event, NW_CLOSED); 
   
    rt_kprintf("Modbus Tcp server closeed! Wait for new connect !\n");
  }
}

/* ----------------------- Begin implementation -----------------------------*/
BOOL
xMBTCPPortInit( USHORT usTCPPort )
{
  BOOL bOkay = FALSE;
  rt_thread_t tid;

  if( usTCPPort == 0 )
  {
      usPort = MB_TCP_DEFAULT_PORT;
  }
  else
  {
      usPort = (USHORT)usTCPPort;
  }

  tid = rt_thread_create("mbtcpsrv", mbtcp_srv_thread, RT_NULL,
          0x800, 25, 5);
  if (tid != RT_NULL) 
  {
    rt_thread_startup(tid);
    bOkay = TRUE;
  }
  return bOkay;    
}

void 
vMBTCPPortClose(  )
{
    
}

void
vMBTCPPortDisable( void )
{
    
}

BOOL
xMBTCPPortGetRequest( UCHAR ** ppucMBTCPFrame, USHORT * usTCPLength )
{
    *ppucMBTCPFrame = &ucTCPRequestFrame[0];
    *usTCPLength = ucTCPRequestLen;
    
    /* Reset the buffer. */
    ucTCPRequestLen = 0;
    return TRUE;
}

BOOL
xMBTCPPortSendResponse( const UCHAR * pucMBTCPFrame, USHORT usTCPLength )
{
    memcpy( ucTCPResponseFrame , pucMBTCPFrame , usTCPLength);
    ucTCPResponseLen = usTCPLength;
    
    /* Send data via nw_thread */
    rt_event_send(&nw_event, NW_TX);      
    return TRUE;
}

