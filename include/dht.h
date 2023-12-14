#include<stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <softTone.h>
#include <unistd.h>
#include <stdint.h>
#include "motor.h"

#define DHTPIN 4

extern int door;

void* temperatureControlThread();
int readDHT11Data();