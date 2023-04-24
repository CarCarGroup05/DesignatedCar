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
  if(BT.available()){
    BT.readBytes(treasureMap, 10);
    BT.write("Received!\n");
    for(int i = 0; i < strlen(treasureMap); i++)
      Serial.print(treasureMap[i]);
    received = true;
    return true;
  }
  return false;
}

bool askStart(){
  if(BT.available()){
      BT.readBytes(inptemp, 10);
      if(inptemp[0] == 'Q'){ // if receive 'Q' from python
        return true;         // , then the car start running
      }
      else
        return false;
  }
  return false;
}

// send UID back through Serial1(bluetooth serial)
void send_byte(byte *id, byte& idSize, bool newlyFound){
  if(newlyFound){
    BT.write("Sending...");
    for (byte i = 0; i < 4; i++) {  // Send UID consequently.
      // Serial.print("Success!");

        BT.write(id[i]); // send id to python
        Serial.print(id[i],HEX); // print id in arduino serail monitor 
    }
    Serial.println();
    newlyFound = false;
  }
}
#endif
