#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <wiringPi.h>
#include <unistd.h>
#include <softPwm.h>
#include <wiringPiI2C.h>
#include <stdint.h>

#define UC unsigned char

#define R_PIO 24
#define G_PIO 23
#define B_PIO 22

#define MOTOR1_IN1_PIN 18
#define MOTOR1_IN2_PIN 19
#define MOTOR2_IN1_PIN 12
#define MOTOR2_IN2_PIN 13

#define DHTPIN 7

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

// 온습도 센서 관련 함수
void readDHT11Data();
void* dhtThread(void* arg);

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
    pthread_t ledThreadId, temperatureThreadId, dhtThreadId;

    // 각 스레드 실행
    pthread_create(&ledThreadId, NULL, ledThread, NULL);
    pthread_create(&temperatureThreadId, NULL, temperatureControlThread, NULL);
    pthread_create(&dhtThreadId, NULL, dhtThread, NULL);

    // 메인 스레드에서는 door 값을 변경하여 동작 테스트
    while (1) {
        // 사용자 입력 받기
        int userInput;
        printf("Enter 1 to open the door, 0 to close the door: ");
        scanf("%d", &userInput);

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
    pthread_join(dhtThreadId, NULL);

    return 0;
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
            // 여기에 온도에 따른 제어 로직 추가
        }
        pthread_mutex_unlock(&doorMutex);

        // 대기시간
        delay(1000);  // 예시: 1초 대기
    }
    return NULL;
}

// 온습도 센서 읽기 스레드
void* dhtThread(void* arg) {
    while (1) {
        pthread_mutex_lock(&doorMutex);
        if (door == 1) {
            // 문이 열려있을 때 온습도 센서 읽기
            readDHT11Data();
        }
        pthread_mutex_unlock(&doorMutex);

        // 대기시간
        delay(2000);  // 예시: 2초 대기
    }
    return NULL;
}

// 온습도 센서 읽기 함수
void readDHT11Data() {
    uint8_t laststate = HIGH;
    uint8_t counter = 0;
    uint8_t j = 0, i;
    uint8_t flag = HIGH;
    uint8_t state = 0;
    float f;

    int dht11_dat[5] = {0,};

    pinMode(DHTPIN, OUTPUT);
    digitalWrite(DHTPIN, LOW);
    delay(18);

    digitalWrite(DHTPIN, HIGH);
    delayMicroseconds(30);
    pinMode(DHTPIN, INPUT);

    for (i = 0; i < 83; i++) {
        counter = 0;
        while (digitalRead(DHTPIN) == laststate) {
            counter++;
            delayMicroseconds(1);
            if (counter == 255) break;
        }
        laststate = digitalRead(DHTPIN);
        if (counter == 255) break;

        if ((i >= 4) && (i % 2 == 0)) {
            dht11_dat[j / 8] <<= 1;
            if (counter > 16) dht11_dat[j / 8] |= 1;
            j++;
        }
    }

    if ((j >= 40) && (dht11_dat[4] == ((dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF))) {
        printf("Temperature = %d.%d *C, Humidity = %d.%d %%\n", dht11_dat[2], dht11_dat[3], dht11_dat[0], dht11_dat[1]);
    } else {
        printf("Data get failed\n");
    }
}
