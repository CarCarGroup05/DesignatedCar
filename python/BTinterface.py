import BT

# hint: You may design additional functions to execute the input command, which will be helpful when debugging :)

class BTinterface:
    def __init__(self):
        print("")
        print("Arduino Bluetooth Connect Program.")
        print("")
        self.ser = BT.bluetooth()
        port = input("PC bluetooth port name: ")
        while(not self.ser.do_connect(port)):
            if(port == "quit"):
                self.ser.disconnect()
                quit()
            port = input("PC bluetooth port name: ")

    def start(self):
        input("Press enter to start.")
        self.ser.SerialWriteString('s')

    def get_UID(self):
        return self.ser.SerialReadByte()

    def send_action(self,dirc):
        self.ser.SerialWriteString(dirc)
        #print(123)
        
        
        

        # TODO : send the action to car
        return 0

    def end_process(self):
        
        self.ser.SerialWriteString('e')
        self.ser.disconnect()

if __name__ == '__main__':
    test = BTinterface()
    test.send_action("should be direction")
    while(True):
        test.ser.SerialReadString()
        #test.start()
        #test.send_action(1)
    
    #test.end_process()

'''
if __name__ == '__main__':
    test = BTinterface()
    test.start()
    test.end_process()
'''

