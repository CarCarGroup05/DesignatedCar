/***************************************************************************/
// File       [final_project.ino]
// Author     [Erik Kuo]
// Synopsis   [Code for managing main process]
// Functions  [setup, loop, Search, getPath]
// Modify     [2020/03/27 Erik Kuo]
/***************************************************************************/

#define DEBUG  // debug flag
#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>
#include "node.h"

/*===========================define pin & create module object================================*/
// BlueTooth
// Mega          HC05
// Pin (Function) Pin
// 10    TX   ->   RX
// 11    RX   <-   TX
SoftwareSerial BT(10, 11);

// 馬達控制器(TB6612), 請按照自己車上的接線寫入腳位(左右不一定要跟註解寫的一樣)
#define MotorR_I1 2    //定義 A1 接腳（右）
#define MotorR_I2 3    //定義 A2 接腳（右）
#define MotorR_PWMR 9  //定義 ENA (PWM調速) 接腳
#define MotorL_I3 5    //定義 B1 接腳（左）
#define MotorL_I4 6    //定義 B2 接腳（左）
#define MotorL_PWML 8  //定義 ENB (PWM調速) 接腳

// 紅外線模組(IR模組), 請按照自己車上的接線寫入腳位
#define IRpin_LL 32
#define IRpin_L 34
#define IRpin_M 36
#define IRpin_R 38
#define IRpin_RR 40

// RFID, 請按照自己車上的接線寫入腳位
#define RST_PIN 7                  // 讀卡機的重置腳位
#define SS_PIN 53                  // 晶片選擇腳位
MFRC522 mfrc522(SS_PIN, RST_PIN);  // 建立MFRC522物件
/*===========================define pin & create module object===========================*/

/*============setup============*/
// MFRC522 *mfrc522;
// 宣告MFRC522指標
void setup(){
  //bluetooth initialization
  BT.begin(9600);
  //Serial window
  Serial.begin(9600);
  //RFID initial
  SPI.begin();
  mfrc522.PCD_Init();

  //TB6612(motor) pin
  pinMode(MotorR_I1, OUTPUT);
  pinMode(MotorR_I2, OUTPUT);
  pinMode(MotorL_I3, OUTPUT);
  pinMode(MotorL_I4, OUTPUT);
  pinMode(MotorL_PWML, OUTPUT);
  pinMode(MotorR_PWMR, OUTPUT);

  //tracking(IR) pin
  pinMode(IRpin_LL, INPUT);
  pinMode(IRpin_L, INPUT);
  pinMode(IRpin_M, INPUT);
  pinMode(IRpin_R, INPUT);
  pinMode(IRpin_RR, INPUT);
}
/*============setup============*/

/*=====Import header files=====*/
#include "RFID.h"
#include "track.h"
#include "bluetooth.h"
/*=====Import header files=====*/

/*===========================initialize variables===========================*/
int _Tp = 160;  // set your own value for motor power
int mapState = 0;
bool start = false;
bool received = false;
char treasureMap[256];
int irRead = 0;
byte idSize = 8;
bool newlyFound = false; // flag set as true when find a new TREASURE
/*===========================initialize variables===========================*/

/*===========================declare function prototypes===========================*/
void Search();  // search graph
void getPath(char tMap);
/*===========================declare function prototypes===========================*/

/*===========================define function===========================*/
void loop(){

  while(!start){
    while(!received){
      if(Serial.available())
        BT.write(Serial.read());
      received = ask_BT(treasureMap);
  send_byte(rfid(idSize, newlyFound), idSize, newlyFound);
    }
    if(askStart()){
      BT.write("Start~");
      start = true;
      motionSwitch(treasureMap[0]);
      done();
      mapState++;
    }
  }
  Search();
  // flag newlyFound determines whether to send UID
}

/*===========================define function===========================*/
void Search(){
  send_byte(rfid(idSize, newlyFound), idSize, newlyFound);
  if(tracking(treasureMap[mapState]))
    mapState++;
  if(mapState >= strlen(treasureMap)){
    MotorWriting(0, 0);
    BT.write("done!");
    successs();
    start = false;
  }
}

void getPath(char tMap){
  if(BT.available()) {
    BT.readBytes(&tMap, 256);
    Serial.println(tMap);
  }
}
/*===========================define function===========================*/