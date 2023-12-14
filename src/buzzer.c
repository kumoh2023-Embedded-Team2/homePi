#include "buzzer.h"

void initializeBuzzer() {
    softToneCreate(BUZZER_PIN);
}

void* buzzerThread(void* arg) {
    while (1) {

        // door 값이 변경되었을 때만 부저를 재생
        if (door != prevDoor) {
            int playBuzzer = (door == 1);  // door가 1이면 1, 아니면 0
            prevDoor = door;  // 이전 door 값 업데이트

            if (playBuzzer) {
                // door가 1이면 도레미 소리
                softToneWrite(BUZZER_PIN, 523);  // 도
                delay(1000);  // 1초 동안 소리 재생
                softToneWrite(BUZZER_PIN, 587);  // 레
                delay(1000); 
                softToneWrite(BUZZER_PIN, 659);  // 미
                delay(1000);
                softToneWrite(BUZZER_PIN, 0);  // 부저 끄기
            } else {
                // door가 0이면 솔라시 소리
                softToneWrite(BUZZER_PIN, 783);  // 솔
                delay(1000);  // 1초 동안 소리 재생
                softToneWrite(BUZZER_PIN, 880);  // 라
                delay(1000);  // 1초 동안 소리 재생
                softToneWrite(BUZZER_PIN, 987);  // 시
                delay(1000);
                softToneWrite(BUZZER_PIN, 0);  // 부저 끄기
            }
        } 
        // 대기시간
        delay(100);  // 예시: 0.1초 대기
    }
    return NULL;
}
