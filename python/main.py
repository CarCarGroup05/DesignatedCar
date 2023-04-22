import maze as mz
from score import ScoreboardFake, Scoreboard
from BTinterface import BTinterface
import keyboard
import numpy as np
import pandas
import time
import sys
import os

def main():
    maze = mz.Maze("data/small_maze.csv")
    
    #point = Scoreboard("123456789", "http://140.112.175.15:3000")
    # point = ScoreboardFake("your team name", "data/fakeUID.csv")
    
    '''
    interf = BTinterface()
    interf.start
    '''
    # TODO : Initialize necessary variables
    s = int(input("start:"))
    e = int(input("end:"))
    maze.BFS_2(s,e)
    maze.getActions_2()
    interf = BTinterface()
    interf.send_action(maze.actions_to_str_2())
    print(maze.actions_to_str_2())
    
    while True:
        if keyboard.read_key() == "q":
            interf.ser.SerialWriteString("Q")
        interf.ser.SerialReadString()
'''
    if (sys.argv[1] == '0'):
        print("Mode 0: for treasure-hunting")
        # TODO : for treasure-hunting, which encourages you to hunt as many scores as possible
        
    elif (sys.argv[1] == '1'):
        print("Mode 1: Self-testing mode.")
        # TODO: You can write your code to test specific function.
'''
if __name__ == '__main__':
    main()

