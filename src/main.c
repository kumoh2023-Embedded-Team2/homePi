#include "main.h"

int door = 0;
int prevDoor = 0;

int main() {
    // WiringPi 초기화
    if (wiringPiSetupGpio() == -1) {
        fprintf(stderr, "Failed to initialize WiringPi\n");
        return 1;
    }

    //BT 초기회
    initializeBT();

    // DC 모터 초기화
    initializeMotors();

    // LED 초기화
    ledInit();

    // 부저 초기화
    initializeBuzzer();

    // 스레드 생성
    pthread_t ledThreadId, temperatureThreadId, buzzerThreadId;

    // 각 스레드 실행
    pthread_create(&ledThreadId, NULL, ledThread, NULL);
    pthread_create(&temperatureThreadId, NULL, temperatureControlThread, NULL);
    pthread_create(&buzzerThreadId, NULL, buzzerThread, NULL);

    // 메인 스레드에서는 door 값을 변경하여 동작 테스트
    while (1) {
        // 사용자 입력 받기
        int userInput;
        printf("Enter 1 to open the door, 0 to close the door: ");
        scanf("%d", &userInput);
        //bt 값 수신
        // unsigned char temp = getBtData(fd_serial); //데이터 받는 코드
        // printf("receive data: %d\n", temp);
        // door 값 변경
        door = userInput;

        // 대기시간
        delay(2000);  // 예시: 2초 대기
    }

    // 스레드 종료 대기
    pthread_join(ledThreadId, NULL);
    pthread_join(temperatureThreadId, NULL);
    pthread_join(buzzerThreadId, NULL);

    return 0;
}
