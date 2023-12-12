#include <stdio.h>
#include <unistd.h>
#include <softPwm.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#define UC unsigned char

#define R_PIO 24
#define G_PIO 23
#define B_PIO 22

void ledinit(void);
void ledColorSet(UC,UC,UC);
void RGBled(UC,UC,UC);