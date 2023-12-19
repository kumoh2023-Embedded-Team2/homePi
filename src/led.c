#include "led.h"

void ledInit() {
    softPwmCreate(R_PIO, 0, 255);
    softPwmCreate(G_PIO, 0, 255);
    softPwmCreate(B_PIO, 0, 255);
}

void ledColorSet(UC R_no, UC G_no, UC B_no) {
    softPwmWrite(R_PIO, R_no);
    softPwmWrite(G_PIO, G_no);
    softPwmWrite(B_PIO, B_no);
}
// LED 스레드
void* ledThread() {
    while (1) {
        if (door == 73) {
            // 문이 열려있을 때 LED 켜기 (파란색)
            ledColorSet(255, 0, 0);
        } else {
            // 문이 닫혀있을 때 LED 끄기 (흰색)
            ledColorSet(255, 255, 255);
        }

        // 대기시간
        delay(500);  // 예시: 0.5초 대기
    }
    return NULL;
}