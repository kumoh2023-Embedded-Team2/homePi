#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <wiringPi.h>
#include <unistd.h>
#include <softPwm.h>
#include <wiringPiI2C.h>
#include <wiringSerial.h>
#include <termios.h>  // tcflush 함수 사용을 위한 헤더

#define UC unsigned char


#define MOTOR1_IN1_PIN 18
#define MOTOR1_IN2_PIN 19
#define MOTOR2_IN1_PIN 12
#define MOTOR2_IN2_PIN 13

#define BAUD_RATE   115200
static const char* UART2_DEV = "/dev/ttyAMA1";

pthread_mutex_t doorMutex = PTHREAD_MUTEX_INITIALIZER;
int door = 0;

// 함수 선언
void initializeMotors();
void controlMotor1(int direction);
void controlMotor2(int direction);
void ledInit();
void ledColorSet(UC R_no, UC G_no, UC B_no);
void* ledThread(void* arg);
void* temperatureControlThread(void* arg);
unsigned char serialRead(const int fd);//1Byte 데이터를 수신하는 함수 
unsigned char getBtData(const int fd);
int initBt();
int fd_serial; //UART2 파일 서술자
unsigned char dat;  //1Byte 데이터를 수신하는 함수


int main() {


    // WiringPi 초기화
    if (wiringPiSetupGpio() == -1) {
        fprintf(stderr, "Failed to initialize WiringPi\n");
        return 1;
    }

    // DC 모터 초기화
    initializeMotors();

    // LED 초기화
    ledInit();

    // 스레드 생성
    pthread_t ledThreadId, temperatureThreadId;

    // 각 스레드 실행
    pthread_create(&ledThreadId, NULL, ledThread, NULL);
    pthread_create(&temperatureThreadId, NULL, temperatureControlThread, NULL);

    // 메인 스레드에서는 door 값을 변경하여 동작 테스트
    while (1) {
        // 사용자 입력 받기
        int userInput;
        printf("Enter 1 to open the door, 0 to close the door: ");
        scanf("%d", &userInput);
        initBt();
        //bt 값 수신
        unsigned char temp = getBtData(fd_serial); //데이터 받는 코드
        printf("receive data: %c\n", temp);
        // door = tmp;
        // door 값 변경
        pthread_mutex_lock(&doorMutex);
        door = userInput;
        pthread_mutex_unlock(&doorMutex);

        // 대기시간
        delay(2000);  // 예시: 2초 대기
    }

    // 스레드 종료 대기
    pthread_join(ledThreadId, NULL);
    pthread_join(temperatureThreadId, NULL);

    return 0;
}

int initBt(){
    if ((fd_serial = serialOpen (UART2_DEV, BAUD_RATE)) < 0){ //UART2 포트 오픈
        printf ("Unable to open serial device.\n") ;
        return 1 ;
    }
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

// DC 모터 초기화 함수
void initializeMotors() {
    pinMode(MOTOR1_IN1_PIN, OUTPUT);
    pinMode(MOTOR1_IN2_PIN, OUTPUT);

    pinMode(MOTOR2_IN1_PIN, OUTPUT);
    pinMode(MOTOR2_IN2_PIN, OUTPUT);
}

// 모터1 제어 함수
void controlMotor1(int direction) {
    if (direction == 1) {
        // 정방향 동작
        digitalWrite(MOTOR1_IN1_PIN, HIGH);
        digitalWrite(MOTOR1_IN2_PIN, LOW);
    } else {
        // 정지
        digitalWrite(MOTOR1_IN1_PIN, LOW);
        digitalWrite(MOTOR1_IN2_PIN, LOW);
    }
}

// 모터2 제어 함수
void controlMotor2(int direction) {
    if (direction == 1) {
        // 정방향 동작
        digitalWrite(MOTOR2_IN1_PIN, HIGH);
        digitalWrite(MOTOR2_IN2_PIN, LOW);
    } else {
        // 정지
        digitalWrite(MOTOR2_IN1_PIN, LOW);
        digitalWrite(MOTOR2_IN2_PIN, LOW);
    }
}

// LED 초기화 함수
void ledInit() {
    softPwmCreate(R_PIO, 0, 255);
    softPwmCreate(G_PIO, 0, 255);
    softPwmCreate(B_PIO, 0, 255);
}

// LED 색상 설정 함수
void ledColorSet(UC R_no, UC G_no, UC B_no) {
    softPwmWrite(R_PIO, R_no);
    softPwmWrite(G_PIO, G_no);
    softPwmWrite(B_PIO, B_no);
}

// LED 스레드
void* ledThread(void* arg) {
    while (1) {
        pthread_mutex_lock(&doorMutex);
        if (door == 1) {
            // 문이 열려있을 때 LED 켜기 (빨간색)
            ledColorSet(255, 0, 0);
        } else {
            // 문이 닫혀있을 때 LED 끄기 (흰색)
            ledColorSet(255, 255, 255);
        }
        pthread_mutex_unlock(&doorMutex);

        // 대기시간
        delay(500);  // 예시: 0.5초 대기
    }
    return NULL;
}

// 온습도 센서 제어 스레드
void* temperatureControlThread(void* arg) {
    while (1) {
        pthread_mutex_lock(&doorMutex);
        if (door == 1) {
            // 문이 열려있을 때 온도 제어
            int temperature;
            printf("Enter the temperature value: ");
            scanf("%d", &temperature);

            if (temperature >= 25) {
                // 온도가 25 이상이면 모터1 작동
                controlMotor1(1);
            } else {
                // 온도가 25 미만이면 모터2 작동
                controlMotor2(1);
            }
        }
        else{
            controlMotor1(0);
            controlMotor2(0);
        }
        pthread_mutex_unlock(&doorMutex);

        // 대기시간
        delay(1000);  // 예시: 1초 대기
    }
    return NULL;
}
