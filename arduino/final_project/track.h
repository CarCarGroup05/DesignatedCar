/***************************************************************************/
// File			  [track.h]
// Author		  [Erik Kuo]
// Synopsis		[Code used for tracking]
// Functions  [MotorWriting, MotorInverter, tracking]
// Modify		  [2020/03/27 Erik Kuo]
/***************************************************************************/

/*if you have no idea how to start*/
/*check out what you have learned from week 1 & 6*/
/*feel free to add your own function for convenience*/

/*===========================import variable===========================*/
int extern _Tp;
/*===========================import variable===========================*/
// Write the voltage to motor.
void MotorWriting(double vL, double vR) {
  // TODO: use TB6612 to control motor voltage & direction
  analogWrite(MotorL_PWML, vL);
  analogWrite(MotorR_PWMR, vR);
}// MotorWriting

// Handle negative motor_PWMR value. 
void MotorInverter() {
  //Hint: the value of motor_PWMR must between 0~255, cannot write negative value.
  digitalWrite(MotorR_I1, 0);
  digitalWrite(MotorR_I2, 1);
  digitalWrite(MotorL_I3, 1);
  digitalWrite(MotorL_I4, 0);
  MotorWriting(_Tp, _Tp);
}// MotorInverter
void MotorMove(){
  digitalWrite(MotorR_I1, 1);
  digitalWrite(MotorR_I2, 0);
  digitalWrite(MotorL_I3, 1);
  digitalWrite(MotorL_I4, 0);
  MotorWriting(_Tp, _Tp);
}

void backTurn(){
  MotorInverter();
  delay(880);
  MotorMove();
}

void lrTurn(int n){
  MotorWriting(_Tp * (1 + (n%2)*2) / 2, _Tp * (1+((n+1)%2)*2) / 2);
  delay(800);
}

// P/PID control Tracking
bool tracking(char nextMo){ 
  int tempIR = 0;// 五個IR的讀值總和
  bool atNode = true;
  // find your own parameters!
  // double _w0; //
  // double _w1; // 
  // double _w2; //
  // double _Kp; // p term parameter 
  // double _Kd; // d term parameter (optional) 
  // double _Ki; // i term parameter (optional) (Hint: 不要調太大)
  // double error=l2*_w2+l1*_w1+m0*_w0+r1*(-_w1)+r2*(-_w2);
  // double vR, vL; // 馬達左右轉速原始值(從PID control 計算出來)。Between -255 to 255.
  // double adj_R=1, adj_L=1; // 馬達轉速修正係數。MotorWriting(_Tp,_Tp)如果歪掉就要用參數修正。
  // int dir = 0;
  // TODO: complete your P/PID tracking code
  for(int i = 0; i < 5; i++){
    if(!digitalRead(32 + 2 * i))
      atNode = false;
    tempIR += (i - 2)*digitalRead(32 + 2 * i);
  }
  MotorWriting(_Tp* (1 + tempIR * 0.15), _Tp * (1 - tempIR * 0.15));
  if(atNode){
    switch(nextMo){
      case 'S':
        MotorWriting(0, 0);
        delay(100000);
        break;
      case 'M':
        MotorWriting(_Tp, _Tp);
        delay(1000);
        break;
      case 'R':
        lrTurn(0);
        break;
      case 'L':
        lrTurn(1);
        break;
      case 'B':
        backTurn();
        break;
      default:
        break;
    }
  }
  return atNode;
  // end TODO
}// tracking
//   S, // stop
//   M, // move on 
//   R, // righ turn 
//   L, // left turn 
//   B, // back turn
