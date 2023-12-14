#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <termios.h>  // tcflush 함수 사용을 위한 헤더

#define IN_CHAR 'I'
#define OUT_CHAR 'O'

#define BAUD_RATE 115200 //블루투스의 보율이 바뀔 경우 이 값을 변경해야함 

extern int fd_serial;
extern unsigned char dat;
extern int door;
extern int prevDoor;

unsigned char serialRead(const int fd);//1Byte 데이터를 수신하는 함수 
unsigned char getBtData(const int fd);
int initializeBT();
