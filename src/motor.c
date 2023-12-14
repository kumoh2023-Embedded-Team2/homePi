#include "motor.h"

void initializeMotors() {
    pinMode(MOTOR1_IN1_PIN, OUTPUT);
    pinMode(MOTOR1_IN2_PIN, OUTPUT);

    pinMode(MOTOR2_IN1_PIN, OUTPUT);
    pinMode(MOTOR2_IN2_PIN, OUTPUT);
}

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
