#ifdef __cplusplus
#if __cplusplus
    extern "C"{
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#ifndef __ORDER_MAIN_H__
#define __ORDER_MAIN_H__


#define SERVICE_IP "192.9.100.166"
#define SERVICE_PORT 30999



#define MSGKEY 1024
struct msg_struct  {  
    int msgType;  
    int len;  
    char msgText[2048];   
}; 


#define VERSION "2018102920181030"


//ָ���ϱ�Ĭ������-��
#define ORDER_PEROID 	10
//���̳߳�ʱʱ��-��
#define ORDER_RECEIVE_TIMEOUT  600


typedef enum order_message_type
{
	MESSAGE_STATUS_SEND 		= 0x1001,// ״̬�ϱ�
	MESSAGE_STATUS_RECEIVE 	= 0x2001,// ״̬�ϱ��ظ�
	
	MESSAGE_PRINT_CONTROL_SEND  = 0x1002,//��ӡ����
	MESSAGE_PRINT_CONTROL_RECEIVE = 0x2002,//��ӡ���ƻظ�

	MESSAGE_PRINT_RES_SEND  = 0x1003,// ��ӡ����ϱ�
	MESSAGE_PRINT_RES_RECEIVE = 0x2003,//��ӡ����ϱ��ظ�

	MESSAGE_PERIOD_SET_SEND  = 0x1004,//��������
	MESSAGE_PERIOD_SET_RECEIVE = 0x2004,//�������ûظ�

}ORDER_MESSAGE_TYPE;



//����
void order_send_thread(void);
void order_receive_thread(void);
void add_heart_thread(void);


#endif

#ifdef __cplusplus
#if __cplusplus
	}
#endif  /* __cplusplus */
#endif  /* __cplusplus */
/* EOF*/