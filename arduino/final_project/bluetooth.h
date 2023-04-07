/***************************************************************************/
// File			  [bluetooth.h]
// Author		  [Erik Kuo]
// Synopsis		[Code for bluetooth communication]
// Functions  [ask_BT, send_msg, send_byte]
// Modify		  [2020/03/27 Erik Kuo]
/***************************************************************************/

/*if you have no idea how to start*/
/*check out what you have learned from week 2*/

enum BT_CMD {
  S, // stop
  M, // move on 
  R, // righ turn 
  L, // left turn 
  B, // back turn
  // TODO: add your own command type here
};

BT_CMD ask_BT(){ // get command from python
    BT_CMD message = S;
    char cmd = '\0';
    if(BT.available()){
      
    //   Serial.write(BT.read());
      
      // TODO:
      // 1. get cmd from Serial1(bluetooth serial)
      // 2. link bluetooth message to your own command type

      #ifdef DEBUG
      Serial.print("cmd : ");
      Serial.println(cmd);
      #endif
   }
    return message;
}// ask_BT

// send msg back through Serial1(bluetooth serial)
// can use send_byte alternatively to send msg back
// (but need to convert to byte type)
void gsend_ms(const char& msg) // send command to python
{
     // TODO:
     if (Serial.available()) {
      
      BT.write(Serial.read());
     }

}// send_msg

// send UID back through Serial1(bluetooth serial)
void send_byte(byte *id, byte& idSize) {
  for (byte i = 0; i < idSize; i++) {  // Send UID consequently.
    BT.write(id[i]);
  }
  #ifdef DEBUG
  Serial.print("Sent id: ");
  for (byte i = 0; i < idSize; i++) {  // Show UID consequently.
    Serial.print(id[i], HEX);
  }
  Serial.println();
  #endif
}// send_byte
