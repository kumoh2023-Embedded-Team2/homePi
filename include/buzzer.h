#include<stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <softTone.h>

#define UC unsigned char
#define TOTAL_NOTES 64
#define BUZZER_PIN 21
#define U_PA 174
#define U_SOL 196
#define U_RA 220
#define U_SI 246
#define DO 262
#define RE 294
#define MI 330
#define PA 350
#define SOL 392
#define RA 440
#define SI 494

extern int door;
extern int prevDoor;

void initializeBuzzer();
void* buzzerThread();