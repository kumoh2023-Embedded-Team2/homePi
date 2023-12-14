#include<stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <softTone.h>

#define UC unsigned char
#define BUZZER_PIN 21

extern int door;
extern int prevDoor;

void initializeBuzzer();
void* buzzerThread();