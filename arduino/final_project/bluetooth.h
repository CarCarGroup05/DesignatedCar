/***************************************************************************/
// File			  [bluetooth.h]
// Author		  [Erik Kuo]
// Synopsis		[Code for bluetooth communication]
// Functions  [ask_BT, send_msg, send_byte]
// Modify		  [2020/03/27 Erik Kuo]
/***************************************************************************/

#ifndef bluetooth_h
#define bluetooth_h

char inptemp[10]; // 儲存起始指令的 buffer
extern bool received;

bool ask_BT(char treasureMap[]){ // get command from python
  // successs();
  if(BT.available()){
    BT.readBytes(treasureMap, 10);
    BT.write("Received!\n");
    for(int i = 0; i < strlen(treasureMap); i++)
      Serial.println(treasureMap[i]);
    received = true;
    return true;
  }
  return false;
}

bool askStart(){
  if(BT.available()){

      BT.readBytes(inptemp, 10);
      if(inptemp[0] == 'Q'){
        return true;
      }
      else
        return false;

  }
  return false;
}

// send UID back through Serial1(bluetooth serial)
void send_byte(byte *id, byte& idSize, bool newlyFound){
  if(newlyFound){
    for (byte i = 0; i < 4; i++) {  // Send UID consequently.
      // Serial.print("Success!");
        BT.write(id[i]);
        Serial.print(id[i],HEX);
    }
    Serial.println();
    newlyFound = false;
  }
}
// enum BT_CMD {
//   S, // stop
//   M, // move on 
//   R, // righ turn 
//   L, // left turn 
//   B, // back turn
//   // TODO: add your own command type here
// };
// #ifdef DEBUG
// Serial.print("Sent id: ");
// for (byte i = 0; i < idSize; i++) {  // Show UID consequently.
//   Serial.print(id[i], HEX);
// }
// Serial.println();
// #endif
// send_byte
#endif
