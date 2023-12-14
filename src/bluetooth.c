#include "bluetooth.h"

int fd_serial;
unsigned char dat;
static const char* UART2_DEV = "/dev/ttyAMA1";

int initializeBT(){
    if ((fd_serial = serialOpen (UART2_DEV, BAUD_RATE)) < 0){ //UART2 포트 오픈
        printf ("Unable to open serial device.\n") ;
        return 1 ;
    }
    return 0;
}

unsigned char getBtData(const int fd) {
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
unsigned char serialRead(const int fd) {
    unsigned char x;
    if(read (fd, &x, 1) != 1) //read 함수를 통해 1바이트 읽어옴
        return -1;
    return x; //읽어온 데이터 반환
}