import pandas
import numpy as np 
import requests
import socketio
import sys
import _thread

# ================================================================
# ScoreboardFake
#   add_UID(UID_str)
#     UID_str : UID string ("0x" excluded)
#   getCurrentScore()
#     return current score (int)
# ================================================================

class ScoreboardFake:
    '''
    Fake scoreboard. Check uid with fakeUID.csv
    '''
    def __init__(self, teamName, filepath):
        raw_data = np.array(pandas.read_csv(filepath))#.values

        self.totalScore = 0
        self.team = teamName

        print ("{} is playing game!".format(self.team))

        self.cardList = [int(a, 16) for a in raw_data.T[0]]

        # data member specific for Game: self.cardValue, self.visitList
        self.visitList = list()
        self.cardValue = dict()
        for i in range(len(raw_data)):
            self.cardValue[self.cardList[i]] = raw_data[i][1]
        print ("Successfully read the UID file!")

    def add_UID(self, UID_str):
        UID = int(UID_str,16)	# hex to dec
		
        if UID not in self.cardList:
            print("This UID doesn't exist in the UID list file:", hex(UID))
        else:
            if UID in self.visitList:
                print("This UID is already visited:", hex(UID))
            else:
                point = self.cardValue[UID]
                self.totalScore += point
                print("A treasure is found! You got " + str(point) + " points.")
                print("Current score: "+ str(self.totalScore))
                print("")
                self.visitList.append(UID)

    def getCurrentScore(self):
        return int(self.totalScore)


# ================================================================
# Scoreboard
#   __init__(teamName, host="http://140.112.175.15:3000")
#     teamName: specify your teamName, this will be shown on the server
#     host: default http://140.112.175.15:3000, open this link to check if the server is on
#   add_UID(UID_str)
#     UID_str : UID string ("0x" excluded)
#   getCurrentScore()
#     return current score (int)
# ================================================================


class Scoreboard:
    '''
    Connects to the server and updates score by add_UID().
    '''
    def __init__(self, teamName, host="http://140.112.175.15:3000"):
    
        # INIT VARIABLES
        self.totalScore = 0
        self.team = teamName
        self.game = 0
        self.ip = host

        print(f"{self.team} wants to play!")
        print(f"connecting to server......{self.ip}")

        # CONNECT WITH SERVER SOCKET

        # create socket.io instance and connect to server
        self.socket = Socket(self.ip)

        # GET INFORMATION FROM SERVER
        print("Game is starting.....")
        self.socket.start_game({ 
                'gamemode': self.game, 
                'team': self.team })

    def add_UID(self, UID_str):
        '''Send {UID_str} to server to update score. Returns nothing.'''
        UID_type = type(UID_str)
        UID_len = len(UID_str)
        print("In add_UID, UID = {}".format(UID_str))
        if(UID_type != type('')):
            print("    UID type error! (Your type: {})".format(UID_type.__name__))
        if(UID_len != 8):
            print("    UID length error! (Your length: {})".format(UID_len))
        self.socket.add_UID({'uid_str': UID_str})

    def getCurrentScore(self):
        try:
            r = requests.get(self.ip+"/current_score?id="+self.socket.get_id())
            return r.json()['current_score']
        except:
            print("Failed to fetch current score")
            return None


# ================================================================
# Socket interface
#   mySocket = Socket(ip)
# ================================================================


class Socket(socketio.ClientNamespace):
    sio = socketio.Client()

    def __init__(self, ip):
        #***********************#
        # pass in the namespace '/'
        # https://python-socketio.readthedocs.io/en/latest/client.html#class-based-namespaces
        #***********************#
        super().__init__('/')
        self.ip = ip
        try:
            self.sio.connect(self.ip)
        except Exception as e:
            print('Failed to connect to server, ', e)
            sys.exit(1)
        self.sio.register_namespace(self)
        self.team = None

    def on_connect(self):
        print("connected")
    
    def on_invalid_mode(self):
        print("Error:invalid gamemode!!")

    def on_game_end(self, data):
        if data["id"]!=self.get_id(): return
        print("game_end")
        self.disconnect()

    def on_game_started(self, data):
        if data["id"]!=self.get_id(): return
        print("Game started!")
        print("Playing game as\nTeam: {}\nMode: {}".format(
            data['current_team'], data['gamemode']))
        print("Please checkout {} for more information.".format(self.ip))
    
    def on_game_already_started(self, data):
        self.disconnect()
        print("{} is already playing.\nPlease wait {} seconds for retry.".format(data['current_team'],data['time_remain']))
        _thread.interrupt_main()
    
    def on_UID_added(self, message):
        if type(message) != str: return
        print(message)

    def get_id(self):
        return self.sio.get_sid(namespace='/')

    def start_game(self, gamenum):
        self.emit("start_game", gamenum)

    def add_UID(self, UID_str):
        self.emit("add_UID", UID_str)


if __name__ == '__main__':
    import time
    try:
        myScoreboard = Scoreboard("TeamName2","http://localhost:3000")
        # myScoreboard = ScoreboardFake("TeamName","data/fakeUID.csv")
        time.sleep(6)
        myScoreboard.add_UID("61C9931C")
        myScoreboard.add_UID("D1874019")
        myScoreboard.add_UID("12346578")
        print("score:",myScoreboard.getCurrentScore())
    except KeyboardInterrupt:
        sys.exit(1)