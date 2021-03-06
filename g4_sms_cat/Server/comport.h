/********************************************************************************
 *      Copyright:  (C) 2020 LuXiaoyang<920916829@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  comport.h
 *    Description:  This head file of comport.c
 *
 *        Version:  2.0.0(11/07/20)
 *         Author:  LuXiaoyang <920916829@qq.com>
 *      ChangeLog:  1, Release initial version on "11/07/20 16:09:33"
 *                 
 ********************************************************************************/
#ifndef  _COMPORT_H_
#define  _COMPORT_H_

#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>

#define BUF_LEN  128
#define SERIALNAME_LEN 64

enum{
    OPEN_ERROR = -6,
    STAT_ATREADY,
    STAT_SIMREADY,
    STAT_SIMSIGNAL,
    STAT_CREGREADY,
    STAT_SIMIO,
    OK

};

typedef struct sim_states
{
    int         ready;          //AT 
    int         sim_signal;     //AT+CSQ
    int         sim_register;   //AT+CREG?
    int         sim_present;    //AT+CPIN?
	char        operater[32] ;  //AT+COPS?

}sim_state;

typedef struct _st_ComportAttr {

    int                     fd;        //串口文件描述符
    int                     BaudRate;  //波特率
    int                     DataBits;  //数据位
    char                    Parity;    //奇偶校验位
    int                     StopBits;  //停止位
    int                     mSend_Len; //单次最大发送长度
    char                    SerialName[SERIALNAME_LEN];  //串口名称
    struct termios          OldTermios;  //串口的原始属性
	sim_state		        simRegInfo; //注册信息
    //pthread_lock      sms_thread_lock;  //PPP线程不使用
    pthread_mutex_t         mutex;
    pthread_mutexattr_t     attr;

}ComportAttr;

typedef struct gsmd_conf_s
{
    unsigned char   sms_enable;
    unsigned short  sms_port;
    char            sms_tty[16];

    unsigned char   ppp_enable;
    char            ppp_tty[16];

    char            ping_network_card[16];

} gsmd_conf_t;

typedef struct gsmd_ctx_s
{
    ComportAttr            *comport[2];
    gsmd_conf_t             conf;

} gsmd_ctx_t;


/* Use the serial port name(comport->SerialName) to open and assign the returned file descriptor to comport->fd */
int comport_open(ComportAttr *comport);

/* Set the serial port's baud rate, data bits, parity and other attributes, and set the related Peugeot bits for serial communication */
int comport_init(ComportAttr *comport);

/* Send the serial port command in the parameter and can handle the command with too long length */
int comport_send(ComportAttr *comport,char *sbuf,int sbuf_len);

/* Receive serial data, you can use select multiplexing to specify the time of read according to the value of timeout */
int comport_recv(ComportAttr *comport,char *rbuf,int rbuf_len,int timeout);

/* Close the serial port, clear the serial port buffer, and release the memory */
int gsmd_ctx_free(gsmd_ctx_t *gsmd_ctx);



#endif   /* ----- #ifndef _COMPORT_H_  ----- */

