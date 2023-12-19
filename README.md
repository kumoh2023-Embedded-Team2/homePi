# homePi

20210770 이강현 : DHC-11, RGB-LED, BUZZER, DCMOTOR1,2

20180060 권수열 : BT

# 개발배경

- 최근 1인 가구의 증가로 인해 현대 사회에서 편의성이 중요해지고 있습니다. 이에따라 라즈베리파이를 이용한 IOT 도어락/홈파이를 개발했습니다.
- 문이 열리고/닫힐때 각각의 액추에이터들이 스레드를 통해 동시에 작동되게하고, 꺼지게 한다. 
### 주요 기능
- 웰컴/굿바이 사운드 기능
  
1인 가구가 증가함에 따라 집안 분위기를 조성하는 것이 중요합니다. 따라서 도어락에서 문이 열릴때와 닫힐 때 홈파이에서 부저를 통해 웰컴/굿바이 사운드를 출력하여 적적한 분위기를 한결 나아지게 합니다.-
- 온도 측정 및 자동 냉난방 제어
  
문이 열릴 때 DHT-11 센서를 통해 실내 온도를 측정하여 온도에 따라서 냉/난방을 제어합니다. 이를 통해 에너지 소비를 최적화하고 편리함을 증진시킵니다.
- 자동 조명 제어
  
문이 열리고 닫힐 때 자동으로 LED 조명을 키고/끄는 기능을 도입하여 에너지 소비의 최적화와 편의성을 향상시켰습니다.

# 프로젝트 설명 및 구조

1. doorPi에서 키패드 혹은 NFC를 통해 문을열면, 블루투스 모듈을 통해 I/O (IN/OUT)라는 1바이트 크기의 데이터를 homePi에게 전달 합니다.
2. homePi는 블루투스를 통해 전달될 값을 대기하고있다가 I(IN) 혹은 O(OUT)값을 전달 받게 되면 각각의 작동을 하게됩니다.
3. 블루투스 모듈을 통해 homePi에 I(IN)이 들어왔을 경우에는 DHT-11(온습도 센서)와 LED(조명 역할), 부저(웰컴 사운드)가 스레드를 통해서 동시에 작동하게 됩니다.
4. DHT-11센서를 통해서 온도를 측정하고, 온도를 homePi에 전달하여 전달받은 값이 25도 이상일 경우에는 냉방 모터가, 25도 이하일 경우에는 난방 모터가 작동하게 됩니다.
5. 블루투스 모듈을 통해 homePi에 O(OUT)이 들어왔을 경우에는 냉/난방 모터, LED 가 작동을 정지하게 되고, 부저에서 굿바이 사운드가 출력하게 됩니다. 

![시스템구조](https://github.com/kumoh2023-Embedded-Team2/homePi/assets/127746054/a3950484-6aa2-4562-ac04-ef442b80b8c4)

# 사용모듈
- DHT-11 센서
![image](https://github.com/kumoh2023-Embedded-Team2/homePi/assets/127746054/19d5220b-1bb6-40b8-9629-87f92713cee7)
- 수동 부저
![image](https://github.com/kumoh2023-Embedded-Team2/homePi/assets/127746054/bed20c69-9669-4d82-a2fd-cb712f58fdc7)
- RGB_LED
![image](https://github.com/kumoh2023-Embedded-Team2/homePi/assets/127746054/7dc2111b-17cf-4a0d-9ad3-6d1babe46e13)
- 블루투스 모듈
![image](https://github.com/kumoh2023-Embedded-Team2/homePi/assets/127746054/7b669968-6d4a-4ba6-8088-a47c8df8ab30)
- DC 모터
![image](https://github.com/kumoh2023-Embedded-Team2/homePi/assets/127746054/072efeab-5905-4245-a720-74466eddece4)

# 실행방법 

$ make

$./homePi


# 간트 차트 

1. 초기 간트 차트
![image](https://github.com/kumoh2023-Embedded-Team2/homePi/assets/127746054/3bf77aad-dccf-4891-b261-14a3cdc510e9)

2. 실제 간트 차트
![image](https://github.com/kumoh2023-Embedded-Team2/homePi/assets/127746054/f9e7904c-8794-4816-9350-20c0e13603ed)

데모날짜가 미뤄짐에따라 코드개발에 조금더 신경을 쓸수 있었고, 오류해결 및 블루투스 연결 문제에 있어서 시간이 조금더 필요했기 때문에 기존 계획과는 조금 달라졌습니다.


