/***************************************************************************/
// File			  [node.h]
// Author		  [Erik Kuo, Joshua Lin]
// Synopsis		[Code for managing car movement when encounter a node]
// Functions  [/* add on your own! */]
// Modify		  [2020/03/027 Erik Kuo]
/***************************************************************************/

/*===========================import variable===========================*/
// int extern IRpin_LL,IRpin_L,IRpin_M,IRpin_R,IRpin_RR;
int extern _Tp;
int direction;
char temp[10];
/*===========================import variable===========================*/
int inNode(){
    BT.write("666"); //Notify the computer
    while(true){
        if(BT.available()){
            BT.readBytes(temp, 10);
            direction = atoi(temp);
            return direction;
        }
    }
}
// TODO: add some function to control your car when encounter a node
// here are something you can try: left_turn, right_turn... etc.
