#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <memory.h>

#include <sys/ipc.h>   
#include <sys/msg.h> 
#include <errno.h>

#include "order_socket.h"
#include "order_main.h"

//socket���
int nLinkHandle = 0;
//��Ϣ���о��
int msgid = 0; 


//��Ϣ�����߳�
void order_send_thread(void){
	int len = 0;
	int ret = 0;
	while(1){
		//1000������һ�ζ���
		sleep(1);

		//���ж�socket�����Ƿ����
		if(nLinkHandle <= 0){
			//�ȴ����ӻָ�
			printf("connect exception ,waitting .........\n");
			continue;
		}


		 struct msg_struct data = {0};
		 len=msgrcv(msgid,&data,sizeof(struct msg_struct),0,0);
		 if(len >0){
			printf("msgtype =[%02x] len=[%02x]\n",data.msgType,data.len);
			ret = sys_socket_send(nLinkHandle,data.msgText,data.len);
			if(ret< 0 ){
				printf("sys_socket_send error ret = [%02x]\n",ret);
			}else{
				printf("sys_socket_send ok \n");
			}
			
		 }else if (len == 0){
			printf("msg hava no data\n");

		 }else{
			printf("error receive !!! len=[%x]\n",len);
		 }


		 
		
	
	}

}



//��Ϣ�����߳�
void order_receive_thread(void){
	printf("order_receive_thread ,start !!!\n");
	fd_set  	rfds ;
	int 	  retval = 0;
	
	//��������
	char   szBuffer[4096]={0};
	//�������ݳ���
	int     nLength = 0 ;     
	//���������ݳ���
	int     packetLen= 0;
	//λ��
	int     offset = 0;
	//ָ������
	int     msgType = 0;
	time_t timep;
	struct timeval tv;

	while(1){
		printf("socket create \n");

		//��ֹ��������Ƶ��,�ӳ�һ��
		sleep(1);
		nLinkHandle = sys_socket_create(SERVICE_IP, SERVICE_PORT);
		if(nLinkHandle == -1){
			printf("socket create error");
			sys_socket_close(nLinkHandle);
			goto RELEASE_TCP_LINK;
		}


		while(1){
		
			FD_ZERO(&rfds); /* ��select����֮ǰ�ȰѼ������� */
			FD_SET(nLinkHandle, &rfds); /* ��Ҫ���ľ��socket���뵽������ */

			//��ʱʱ��
			tv.tv_sec = 600;
			tv.tv_usec = 0;	
			retval = select ( nLinkHandle+1 , &rfds , NULL , NULL , &tv );		
			switch ( retval )
			{
				case  -1:
					printf("select error \n");
					goto RELEASE_TCP_LINK;
					break;
				case 0 :	
					time (&timep);
					printf("select data null @@@@ time=%s\n",asctime(gmtime(&timep)));
					goto RELEASE_TCP_LINK;
					break; 
				default:/*���շ����ָ��*/
					 if (FD_ISSET(nLinkHandle , &rfds) ){
				 		memset(szBuffer, 0, sizeof(szBuffer));
						usleep(1000); // 1ms
						nLength = recv ( nLinkHandle , szBuffer , 4096 , 0 );
						time (&timep);
						if (nLength<=0)
						{   		
							printf("recv errornLength=%d, time=%s \n",nLength,asctime(gmtime(&timep)));
							goto RELEASE_TCP_LINK;
							break;      
						}
						printf("recieve order_message nLength = %d time=%s\n",nLength,asctime(gmtime(&timep)));

				 	 }
					 break;      
					 
			}

			//1.�˴�des����

			//2.�˴�crc У��

			//3����ָ��
			offset = 0;
			
			//3.1 ������ͷ
			if((szBuffer[offset] &0xff)!=0x8a ||( szBuffer[offset+1]&0xff) !=0x8a){
				printf("message head error[%x][%x]\n",szBuffer[offset],szBuffer[offset+1]);
				continue;
			}
			offset+=2;


			//3.2 �����������ж��Ƿ��ʵ�ʽ������
			packetLen = 0;
			packetLen = (szBuffer[offset]&0xff) <<8 |(szBuffer[offset+1] &0xff);
			printf("packet len  [%x][%x]\n",packetLen,nLength);
			if(packetLen != nLength -4){
				printf("packet len error \n");
				continue;
			}
			offset+=2;

			//3.3��������
			msgType = 0;
			msgType = (szBuffer[offset]&0xff) <<8 |(szBuffer[offset+1] &0xff);
			offset+=2;

			switch(msgType){
				case MESSAGE_STATUS_RECEIVE:{
					// ״̬�ϱ��ظ�
					printf("MESSAGE_STATUS_RECEIVE  \n");
					printf("<-deviced Id = [%02x][%02x][%02x][%02x]->  \n",
						szBuffer[offset],szBuffer[offset+1],szBuffer[offset+2],szBuffer[offset+3]);
											
					break;
				}

				case MESSAGE_PRINT_CONTROL_SEND:{
						printf("MESSAGE_PRINT_CONTROL_SEND \n");
						break;
				}

			
				default:{
					printf("error msgType [%x] \n",msgType);

				}

			}
			
		}
		//�Ͽ�TCP����
		RELEASE_TCP_LINK:
			close( nLinkHandle);
			//�����Ϣ��������

	}

	

}



//��ʱ���������߳�
void add_heart_thread(void){
	int offset = 0;
	char sendData[1024] = {0};
	
	while(1){
		sleep(10);
		//���ж�socket�����Ƿ����
		if(nLinkHandle <= 0){
			//�ȴ����ӻָ�
			printf("connect exception ,waitting .........\n");
			continue;
		}	

		offset = 0;
		memset(sendData,0x00,1024);

		
		//ʾ��ָ��-״̬�ϱ�
			//��ͷ
			sendData[offset] = 0x8a;
			sendData[offset+1] = 0x8a;
			offset+=2;

			//����
			sendData[offset] = 0x00;
			sendData[offset+1] = 0x14;
			offset+=2;
			
			//ָ������
			sendData[offset] = 0x10;
			sendData[offset+1] = 0x01;
			offset+=2;

			//����
			sendData[offset] = 0x11;
			sendData[offset+1] = 0x22;
			sendData[offset+2] = 0x33;
			sendData[offset+3] = 0x44;
			offset+=4;

			memcpy(&sendData[offset],VERSION,8);
			offset+=8;

			sendData[offset] = 0;
			sendData[offset+1] = 1;
			offset+=2;

			//����������des����
			
			//У��,����crc
			offset+=2;

			//��β
			sendData[offset] = 0xa8;
			sendData[offset+1] = 0xa8;
			offset+=2;

		struct msg_struct data = {0};
		int len = offset;
		data.msgType = MESSAGE_STATUS_SEND;
		data.len = len;
		memcpy(&data.msgText[0],sendData,len);
	
		//����з�������
		if(msgsnd(msgid, (void*)&data, sizeof(struct msg_struct), 0) == -1)
		{
			printf("msgsnd failed\n");
		}else{
			printf("add heart msg ok !\n");
		}
		
	}


}


int main(){
	
      	printf("order_service start !!!! \n");

	//������Ϣ����
     	msgid = msgget(MSGKEY,IPC_CREAT|0666);
     	if(msgid <0){  
     		printf("failed to create msq | errno=%d [%s]\n",errno,strerror(errno));  
     		return 0;
     	}  
	

	pthread_t orderReceiveTskid;            
	if(pthread_create(&orderReceiveTskid,NULL ,(void *) order_receive_thread, NULL) !=0){
		printf("order_receive_thread create failed \n");
	}

	pthread_t orderSendTskid;            
	if(pthread_create(&orderSendTskid,NULL ,(void *) order_send_thread, NULL) !=0){
		printf("order_send_thread create failed \n");
	}

	pthread_t addHeartTskid;            
	if(pthread_create(&addHeartTskid,NULL ,(void *) add_heart_thread, NULL) !=0){
		printf("add_heart_thread create failed \n");
	}
	

	pthread_join(orderReceiveTskid, NULL);
	pthread_join(orderSendTskid, NULL);
	pthread_join(addHeartTskid, NULL);


	 printf("order_service end !!!! \n");
	return 0;
}






