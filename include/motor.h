#include<stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <softPwm.h>

#define MOTOR1_IN1_PIN 18
#define MOTOR1_IN2_PIN 19
#define MOTOR2_IN1_PIN 12
#define MOTOR2_IN2_PIN 13

void initializeMotors();
void controlMotor1(int direction);
void controlMotor2(int direction);