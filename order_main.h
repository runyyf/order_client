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


//指令上报默认周期-秒
#define ORDER_PEROID 	10
//收线程超时时间-秒
#define ORDER_RECEIVE_TIMEOUT  600


typedef enum order_message_type
{
	MESSAGE_STATUS_SEND 		= 0x1001,// 状态上报
	MESSAGE_STATUS_RECEIVE 	= 0x2001,// 状态上报回复
	
	MESSAGE_PRINT_CONTROL_SEND  = 0x1002,//打印控制
	MESSAGE_PRINT_CONTROL_RECEIVE = 0x2002,//打印控制回复

	MESSAGE_PRINT_RES_SEND  = 0x1003,// 打印结果上报
	MESSAGE_PRINT_RES_RECEIVE = 0x2003,//打印结果上报回复

	MESSAGE_PERIOD_SET_SEND  = 0x1004,//周期设置
	MESSAGE_PERIOD_SET_RECEIVE = 0x2004,//周期设置回复

}ORDER_MESSAGE_TYPE;



//声明
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