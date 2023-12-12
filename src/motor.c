#include <stdio.h>
#include <wiringPi.h>

#define MOTOR1_IN1_PIN 18   // DC 모터 1 
#define MOTOR1_IN2_PIN 19
#define MOTOR2_IN1_PIN 12   // DC 모터 2
#define MOTOR2_IN2_PIN 13

// 함수 선언
void initializeMotors();
void controlMotor1(int direction);
void controlMotor2(int direction);

int main() {
    // WiringPi 초기화
    if (wiringPiSetupGpio() == -1) {
        fprintf(stderr, "Failed to initialize WiringPi\n");
        return 1;
    }

    // DC 모터 초기화
    initializeMotors();

    // 사용자 입력 반복
    while (1) {
        // 사용자 입력 받기
        int userInput;
        printf("Enter 1 to activate Motor 1, 0 to activate Motor 2, 2 to stop all motors: ");
        scanf("%d", &userInput);

        // 입력에 따라 모터 제어
        if (userInput == 1) {
            controlMotor1(1);  // 모터1을 정방향으로 동작
        } else if (userInput == 0) {
            controlMotor2(1);  // 모터2를 정방향으로 동작
        } else if (userInput == 2) {
            // 모든 모터 정지
            controlMotor1(0);
            controlMotor2(0);
        } else {
            printf("Invalid input. Please enter 0, 1, or 2.\n");
        }
    }

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
