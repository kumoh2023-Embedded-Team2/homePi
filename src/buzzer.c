#include "buzzer.h"

int notes1[TOTAL_NOTES] = { // 웰컴 사운드 악보
    SOL, SOL, SOL, SOL, RE, RE, SOL, SOL,
    PA, PA, MI, RE, MI, MI, RE, DO,
    SOL, SOL, SOL, SOL, RE, RE, SOL, SOL,
    PA, PA, MI, RE, MI, MI, RE, DO,
    SOL, SOL, SOL, SOL, RE, RE, SOL, SOL,
    PA, PA, MI, RE, MI, MI, RE, DO,
    U_SI, DO, RE, RE, DO, DO, U_SI, U_RA,
    U_SOL, U_SOL, U_SOL, U_SOL, U_SOL, U_SOL, U_SOL, U_SOL
    };

int notes2[TOTAL_NOTES] = { // 굿바이 사운드 악보
    U_PA, DO, DO, RE, RE, RE, RE, U_SI, DO, RE,
    MI, RE, DO, U_SI, DO, RE, RE, RE, DO, U_SI, 
    U_SI, U_SI, U_RA, U_SI, PA, MI, MI, MI, U_SI, PA, 
    PA, PA, MI, MI, MI, RE, DO, RE, MI, MI, 
    MI, MI, U_SI, DO, DO, MI, MI, RE, RE, U_SI, 
    DO, RE, MI, RE, DO, U_SI, DO, RE, RE, DO,
    U_SI, U_SI, U_RA, U_SI
};

void initializeBuzzer() {
    softToneCreate(BUZZER_PIN);
}

void* buzzerThread() {
    while (1) {

        // door 값이 변경되었을 때만 부저를 재생
        if (door != prevDoor) {
            int playBuzzer = door;  // door가 1이면 1, 아니면 0
            prevDoor = door;  // 이전 door 값 업데이트

            if (playBuzzer == 73) {
                 for (int i = 0; i < TOTAL_NOTES; i++)
                { // 웰컴 사운드
                    softToneWrite(BUZZER_PIN, notes1[i]);
                    delay(100);
                }
                softToneWrite(BUZZER_PIN, 0);  // 부저 끄기
            } else {
                for (int i = 0; i < TOTAL_NOTES; i++)
                { // 굿바이 사운드
                    softToneWrite(BUZZER_PIN, notes2[i]);
                    delay(200);
                }
                softToneWrite(BUZZER_PIN, 0);  // 부저 끄기
            }
        } 
        // 대기시간
        delay(100);  // 예시: 0.1초 대기
    }
    return NULL;
}
