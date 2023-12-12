/*
참고 링크 :
1. 라즈베리파이 멀티스레드 : https://soheeeep.tistory.com/17
2. 온습도 센서 : https://blog.naver.com/PostView.nhn?blogId=emperonics&logNo=222092518468
3. dc 모터 : https://alisyabob.tistory.com/165 및 교안
4. 수동부저 : 교안 

sofPWM 사용해야함 (pwm gpio가 부족함)
쿨러 : 4,6(default)
온습도 센서 : 2,7,9 (5v,gpio04,gnd)
dc 모터 : 5V,GND,GPIO18(INA),GPIO19(INB)                                                                                                                                                                                                                     
dc 모터2 : 5V,GNX,GPIO12(INA),GPIO13(INB)
수동부저 : GPIO21, GND
LED : 5V,GPIIO 17,27,22(R,G,B)

데모 : 팀별로 자리에다가 세팅을 함 한명이 소개를함 , 다른 팀원들은 보고 평가 (1시간)
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <wiringPi.h>
#include <softTone.h>
#include <unistd.h>
#include <softPwm.h>

// GPIO 핀 정의
#define DC1PWM0 18   // DC 모터 1 
#define DC1PWM1 19
#define DC2PWM0 12 // DC 모터 2
#define DC2PWM1 13
#define LED_R 24
#define LED_G 23 
#define LED_B 22         
#define BUZZER_PIN 17      // 예시: 부저 핀
#define DHT_PIN 4        // 예시: DHT11 센서 핀
// 전역변수
int door = 0;  // 초기값 0 (문이 닫힌 상태)
pthread_mutex_t door_mutex = PTHREAD_MUTEX_INITIALIZER;  // 뮤텍스 초기화

// 함수 선언
void* doorMonitor(void* arg);
void activateActuators(int state);

void setup() {
    wiringPiSetupGpio();
    pinMode(DC1PWM0, OUTPUT);
    pinMode(DC2PWM0, OUTPUT);
    pinMode(LED_R, OUTPUT);
    pinMode(LED_G, OUTPUT);
    pinMode(LED_B, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(DHT_PIN, INPUT);
}

// 액츄에이터 함수
void actuatorFunction(int state) {
    if (state == 1) {
        // door가 1일 때 동작
        float humidity, temperature;
        humidity = temperature = 0.0;

        // DHT11 센서 읽기
        if (readDHT(&humidity, &temperature) == 0) {
            printf("Temperature: %.2f°C, Humidity: %.2f%%\n", temperature, humidity);

            // DC 모터 1 (냉방) 동작
            if (temperature >= 25) {
                digitalWrite(DC1PWM0, HIGH);
            }

            // DC 모터 2 (난방) 동작
            if (temperature <= 22) {
                digitalWrite(DC2PWM0, HIGH);
            }
        }

        // LED 켜기
        digitalWrite(LED_R, 0);
        digitalWrite(LED_G, 0);
        digitalWrite(LED_B, 0);

        // 부저 재생
        softToneWrite(BUZZER_PIN, 1000);  // 도
        usleep(500000);  // 0.5초 대기
        softToneWrite(BUZZER_PIN, 1175);  // 레
        usleep(500000);  // 0.5초 대기
        softToneWrite(BUZZER_PIN, 1319);  // 미
        usleep(500000);  // 0.5초 대기
    } else {
        // door가 0일 때 동작
        // DC 모터 1 (냉방) 중지
        digitalWrite(DC1PWM0, LOW);

        // DC 모터 2 (난방) 중지
        digitalWrite(DC2PWM0, LOW);

        // LED 끄기
        digitalWrite(LED_R, 255);
        digitalWrite(LED_R, 255);
        digitalWrite(LED_R, 255);

        // 부저 재생
        softToneWrite(BUZZER_PIN, 261.63);  // 솔
        usleep(500000);  // 0.5초 대기
        softToneWrite(BUZZER_PIN, 293.66);  // 라
        usleep(500000);  // 0.5초 대기
        softToneWrite(BUZZER_PIN, 329.63);  // 시
        usleep(500000);  // 0.5초 대기
        softToneWrite(BUZZER_PIN, 0);      // 부저 중지
    }
}

// DHT11 센서 읽기 함수
int readDHT(float* humidity, float* temperature) {
    return DHT_read(DHT_PIN, DHT11, humidity, temperature);
}

// 스레드 함수
void* doorMonitor(void* arg) {
    while (1) {
        pthread_mutex_lock(&door_mutex);
        if (door == 1) {
            actuatorFunction(door);
        }
        pthread_mutex_unlock(&door_mutex);
    }
}

int main() {
    // WiringPi 초기화
    setup();
    // DHT11 센서 초기화
    if (DHT_begin() != 0) {
        fprintf(stderr, "Failed to initialize DHT11 sensor\n");
        exit(EXIT_FAILURE);
    }

    // 스레드 생성
    pthread_t monitorThread;
    if (pthread_create(&monitorThread, NULL, doorMonitor, NULL) != 0) {
        perror("Error creating thread");
        exit(EXIT_FAILURE);
    }

    // 메인 스레드에서 door 값 변경 예시
    while (1) {
        scanf("%d",&door);
        pthread_mutex_lock(&door_mutex);
        // 여기에서 door 값을 변경하고 상태에 따라 actuatorFunction을 호출할 수 있음
        door = 1;  // 예시: 문을 열린 상태로 변경
        pthread_mutex_unlock(&door_mutex);

        // 기다리는 시간 (예시: 5초)
        sleep(5);

        pthread_mutex_lock(&door_mutex);
        door = 0;  // 예시: 문을 닫힌 상태로 변경
        pthread_mutex_unlock(&door_mutex);

        // 기다리는 시간 (예시: 5초)
        sleep(5);
    }

    // 스레드 종료
    pthread_join(monitorThread, NULL);

    return 0;
}
