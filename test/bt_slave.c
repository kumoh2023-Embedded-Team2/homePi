// **************************
// ****** 데이터 받는 코드*****
// **************************
#include <stdio.h> 
#include <string.h> 
#include <unistd.h> 
#include <wiringPi.h> 
#include <wiringSerial.h>
#include <termios.h>  // tcflush 함수 사용을 위한 헤더
#include <pthread.h>

#define BAUD_RATE   115200
static const char* UART2_DEV = "/dev/ttyAMA1";
pthread_mutex_t mutex;
unsigned char door;

unsigned char serialRead(const int fd);//1Byte 데이터를 수신하는 함수 
unsigned char getData(const int fd);
int fd_serial; //UART2 파일 서술자
unsigned char dat;  //1Byte 데이터를 수신하는 함수

unsigned char serialRead(const int fd) {
    unsigned char x;
    if(read (fd, &x, 1) != 1) //read 함수를 통해 1바이트 읽어옴
        return -1;
    return x; //읽어온 데이터 반환
}

void accessSharedData(unsigned char data) {
    pthread_mutex_lock(&mutex); //잠그고 
    door = data; //값 바꾸고

    printf("Received Data: %c\n", door);

    pthread_mutex_unlock(&mutex); //풀고

}

unsigned char getData(const int fd) {
    unsigned char tmp = serialDataAvail(fd_serial); //읽을 데이터가 있는지 확인,
    if(tmp){    //읽을 데이터가 있다면 
        dat = serialRead (fd_serial); //버퍼에서 1바이트 값을 읽음
        fflush (stdout); 
        delay (10); 
        
        tcflush(fd, TCIFLUSH); //버퍼 비우기
        return dat;
    }else {
        return 0;
    }
}

int main () 
{
    wiringPiSetupGpio();
    if ((fd_serial = serialOpen (UART2_DEV, BAUD_RATE)) < 0){ //UART2 포트 오픈
        printf ("Unable to open serial device.\n") ;
        return 1 ;
    }

    while(1){
        unsigned char tmp = getData(fd_serial); //데이터 받는 코드
        if( tmp != 0)
            accessSharedData(tmp);  //데이터 수정하는 코드
        }
}