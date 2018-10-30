/*  *****************************************************************************
**	文件名称	: order_socket.h
**	描述说明	: 
**	修改记录	:
**  ****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
    extern "C"{
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#ifndef __ORDER_SOCKET_H__
#define __ORDER_SOCKET_H__

int sys_socket_sendable_timeo(int sockfd, int sec);
int sys_socket_recvable_timeo(int sockfd, int sec);
int sys_socket_recv (int sockfd, char *buf, int buflen);
int sys_socket_send (int sockfd, char *buf, int len);
int sys_socket_create(char* szUsrInetAddr,int nUsrPort);
int sys_socket_close(int sockfd);

#endif

#ifdef __cplusplus
#if __cplusplus
	}
#endif  /* __cplusplus */
#endif  /* __cplusplus */
/* EOF*/