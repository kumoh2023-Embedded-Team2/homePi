#include<stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <softPwm.h>

#define UC unsigned char
#define R_PIO 24
#define G_PIO 22
#define B_PIO 23

extern int door;

void* ledThread();
void ledInit();
void ledColorSet(UC R_no, UC G_no, UC B_no);