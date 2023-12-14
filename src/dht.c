#include "dht.h"

int readDHT11Data() {
    uint8_t laststate = HIGH;
    uint8_t counter = 0;
    uint8_t j = 0, i;

    int dht11_dat[5] = {0,};

    pinMode(DHTPIN, OUTPUT);
    digitalWrite(DHTPIN, LOW);
    delay(18);

    digitalWrite(DHTPIN, HIGH);
    delayMicroseconds(30);
    pinMode(DHTPIN, INPUT);

    for (i = 0; i < 83; i++) {
        counter = 0;
        while (digitalRead(DHTPIN) == laststate) {
            counter++;
            delayMicroseconds(1);
            if (counter == 255) break;
        }
        laststate = digitalRead(DHTPIN);
        if (counter == 255) break;

        if ((i >= 4) && (i % 2 == 0)) {
            dht11_dat[j / 8] <<= 1;
            if (counter > 16) dht11_dat[j / 8] |= 1;
            j++;
        }
    }

    if ((j >= 40) && (dht11_dat[4] == ((dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF))) {
        // 온도 값을 반환
        int temperature= dht11_dat[2];
        printf("Temperature=%d *c\n",temperature);
        return temperature;
    } else {
        printf("Data get failed\n");
        return -1;  // 실패한 경우 음수 값을 반환
    }
}

// 온습도 센서 제어 스레드
void* temperatureControlThread() {
    while (1) {
        if (door == 1) {
            // 문이 열려있을 때 온도 제어
            int temperature = readDHT11Data();
            if (temperature >= 25) {
                // 온도가 25 이상이면 모터1 작동
                controlMotor1(1);
            } else {
                // 온도가 25 미만이면 모터2 작동
                controlMotor2(1);
            }
        } else {
            controlMotor1(0);
            controlMotor2(0);
        }

        // 대기시간
        delay(1000);  // 예시: 1초 대기
    }
    return NULL;
}
