#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <fcntl.h>

#include "order_socket.h"

//��������,��ʱ�ȴ�
int sys_socket_sendable_timeo(int sockfd, int sec)
{
    struct timeval TimeOut;
    fd_set sfds;

    FD_ZERO(&sfds); 
    FD_SET(sockfd, &sfds); 
    
    TimeOut.tv_sec = sec;
    TimeOut.tv_usec = 0;

    return (select(sockfd+1 , NULL ,&sfds , NULL , &TimeOut));
}

//��ȡ����,��ʱ�ȴ�
int sys_socket_recvable_timeo(int sockfd, int sec)
{
    struct timeval TimeOut;
    fd_set rfds;

    FD_ZERO(&rfds); 
    FD_SET(sockfd, &rfds); 
    
    TimeOut.tv_sec = sec;
    TimeOut.tv_usec = 0;

    return (select(sockfd+1 , &rfds , NULL , NULL , &TimeOut));
}

int sys_socket_recv (int sockfd, char *buf, int buflen)
{
    int ret = 0;
    
    ret = recv(sockfd,buf,buflen,MSG_NOSIGNAL);//MSG_NOSIGNAL
    
    return ret;
}

int sys_socket_send (int sockfd, char *buf, int len)
{
    int ret = 0;
    
    ret = send(sockfd,buf,len,MSG_NOSIGNAL);//MSG_NOSIGNAL
    
    return ret;
}


int sys_socket_close (int sockfd)
{
    if(sockfd == 0)
    {
        return -1;
    }
    
    close(sockfd);
    printf(" sys_socket_close , sockfd = [%d] \n\n" , sockfd);

    return 0;
}


/*
szUsrInetAddr Ŀ���ip��ַ
nUsrPort Ŀ����˿�
����tcp���Ӿ��
����TCP�ͻ���
*/
int sys_socket_create(char* szUsrInetAddr,int nUsrPort){
	int ulLinkHandle;
	int    retval = 0 ;
	struct sockaddr_in remote_addr; //�������������ַ�ṹ��  
	memset(&remote_addr,0,sizeof(remote_addr)); //���ݳ�ʼ��--����  
    	remote_addr.sin_family=AF_INET; //����ΪIPͨ��  
    	remote_addr.sin_addr.s_addr=inet_addr(szUsrInetAddr);//������IP��ַ  
    	remote_addr.sin_port=htons((u_short ) nUsrPort); //�������˿ں�  

	printf( "sys_socket_create \n");
	ulLinkHandle = socket( AF_INET , SOCK_STREAM , IPPROTO_TCP ) ;
	if ( ulLinkHandle < 0 )
	{
		printf( "Socket error[%s] " , strerror(errno) );
		return -1 ;
	}
	printf( "ulLinkHandle = %d \n",ulLinkHandle);
	retval = connect( ulLinkHandle , (struct sockaddr *)&remote_addr , sizeof(struct sockaddr) );
	if ( retval  < 0 )
	{
		printf( "connect retval = %d,ulLinkHandle=%d  errno = [%d]\n",retval,ulLinkHandle,errno);
		return ulLinkHandle ;
	}

	return ulLinkHandle;


}