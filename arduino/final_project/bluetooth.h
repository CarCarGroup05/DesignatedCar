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

bool ask_BT(char treasureMap[]){ // get command from python
  if(BT.available()){
    BT.readBytes(treasureMap, 10);
    BT.write("Received!");
    Serial.println(treasureMap);
    received = true;
    return true;
  }
  return false;
}

bool askStart(){
  if(BT.available()){
      BT.readBytes(inp, 10);
      if(inptemp[0] == 'Q')
        return true;
      else
        return false;
  }
  return false;
}

// send msg back through Serial1(bluetooth serial)
// can use send_byte alternatively to send msg back
// (but need to convert to byte type)

void gsend_ms(const char& msg) // send command to python
{
     // TODO:
    //  if (Serial.available()) {
    //   BT.write(Serial.read());
    //  }

}// send_msg

// send UID back through Serial1(bluetooth serial)
void send_byte(byte *id/*, byte& idSize*/) {
  for (byte i = 0; i < 4; i++) {  // Send UID consequently.
    // Serial.print("Success!");
      BT.write(id[i]);
      Serial.print(id[i],HEX);
  }
  Serial.println();
  return;
  // #ifdef DEBUG
  // Serial.print("Sent id: ");
  // for (byte i = 0; i < idSize; i++) {  // Show UID consequently.
  //   Serial.print(id[i], HEX);
  // }
  // Serial.println();

  // #endif
}// send_byte
#endif