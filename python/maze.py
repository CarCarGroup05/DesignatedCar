from node import *
import numpy as np
import csv
import pandas
from enum import IntEnum
import math


class Action(IntEnum):
    ADVANCE = 1
    U_TURN = 2
    TURN_RIGHT = 3
    TURN_LEFT = 4
    HALT = 5


class Maze:
    def __init__(self, filepath):
        # TODO : read file and implement a data structure you like
        # For example, when parsing raw_data, you may create several Node objects.
        # Then you can store these objects into self.nodes.
        # Finally, add to nd_dict by {key(index): value(corresponding node)}
        self.raw_data = pandas.read_csv(filepath).values
        # print(self.raw_data)

        self.nodes = []
        self.nd_dict = dict()  # key: index, value: the correspond node
        self.deadend = []

        for Nodes in self.raw_data:
            correspond_node = list(
                filter(lambda x: str(x) != 'nan', Nodes[1:5]))
            correspond_node = [int(x) for x in correspond_node]
            self.nd_dict[int(Nodes[0])] = correspond_node

        for Nodes in self.nd_dict:
            if len(self.nd_dict[Nodes]) == 1:
                self.deadend.append(Nodes)

        # print(self.nd_dict)
        # print(self.deadend)

        # BFS所需
        self.queue = []
        self.next_node = []
        self.father = []

        # BFS_continuous所需
        self.all_route = []

        # BFS2所需
        self.queue_2 = []
        self.next_node_2 = []
        self.father_2 = []

        # get actions
        self.shortest_route = []
        self.shortest_route_2 = []
        self.distance = 0
        # 存成Node
        self.All_nodes = []
        for nodes in self.raw_data:
            temp = Node(int(nodes[0]))
            for i in range(1, 5):    # NORTH = 1,EAST = 2,SOUTH = 3,WEST  = 4
                if str(nodes[i]) != "nan":
                    if i == 1:
                        temp.Successors.append(
                            (int(nodes[i]), 1, int(nodes[i+4])))
                    elif i == 2:
                        temp.Successors.append(
                            (int(nodes[i]), 3, int(nodes[i+4])))
                    elif i == 3:
                        temp.Successors.append(
                            (int(nodes[i]), 4, int(nodes[i+4])))
                    elif i == 4:
                        temp.Successors.append(
                            (int(nodes[i]), 2, int(nodes[i+4])))
            self.All_nodes.append(temp)

    def getStartPoint(self):
        if (len(self.nd_dict) < 2):
            print("Error: the start point is not included.")
            return 0
        return self.nd_dict

    def getNodeDict(self):
        return self.nd_dict

    def BFS(self, nd):
        if nd not in self.queue:
            self.queue.append(nd)
        if nd in self.deadend:
            self.deadend.remove(nd)

        while self.queue:
            now = self.queue.pop(0)
            for next in self.nd_dict[now]:
                if next in self.father:
                    continue
                self.queue.append(next)
                self.next_node.append(next)
                self.father.append(now)
                if next in self.deadend:
                    self.deadend.remove(next)
                    return 1
        print("fail")
        return 0

        # TODO : design your data structure here for your algorithm
        # Tips : return a sequence of nodes from the node to the nearest unexplored deadend

    def BFS_continuous(self, nd, distance_limit):
        self.BFS(nd)
        self.getActions()
        self.all_route.extend(self.shortest_route[:-1])
        if len(self.deadend) == 0:
            self.shortest_route = self.all_route
            return 0
        if len(self.all_route) <= int(distance_limit):
            self.father = []
            self.queue = []
            self.next_node = []
            return self.BFS_continuous(self.shortest_route[-1], distance_limit)
        else:
            self.shortest_route = self.all_route
            return 0

    def BFS_2(self, nd_from, nd_to):

        if nd_from not in self.queue_2:
            self.queue_2.append(nd_from)

        while self.queue_2:
            now = self.queue_2.pop(0)
            for next in self.nd_dict[now]:
                if next in self.father_2:
                    continue
                self.queue_2.append(next)
                self.next_node_2.append(next)
                self.father_2.append(now)
                if next == nd_to:
                    return 1
        print("fail")
        return 0

        # TODO : similar to BFS but with fixed start point and end point
        # Tips : return a sequence of nodes of the shortest path
        return None

    def getAction(self, car_dir, nd_from, nd_to):
        # TODO : get the car action
        # Tips : return an action and the next direction of the car if the nd_to is the Successor of nd_to
        # If not, print error message and return 0
        return None

    def getActions(self):
        self.shortest_route = [self.next_node[-1], self.father[-1]]
        for i in range(len(self.next_node)-1, -1, -1):
            if self.next_node[i] == self.shortest_route[-1]:
                self.shortest_route.append(self.father[i])
        self.shortest_route.reverse()
        return self.shortest_route

    def getActions_2(self):
        self.shortest_route_2 = [self.next_node_2[-1], self.father_2[-1]]
        for i in range(len(self.next_node_2)-1, -1, -1):
            if self.next_node_2[i] == self.shortest_route_2[-1]:
                self.shortest_route_2.append(self.father_2[i])
        self.shortest_route_2.reverse()
        return self.shortest_route_2

        # TODO : given a sequence of nodes, return the corresponding action sequence
        # Tips : iterate through the nodes and use getAction() in each iteration
        return None

    def actions_to_str(self):
        # cmds should be a string sequence like "fbrl....", use it as the input of BFS checklist #1

        previous = 1  # 起始朝北
        next = 0
        cmd = ""
        n = 0
        for i in self.shortest_route[0:-1]:
            n += 1
            for successor in self.All_nodes[i-1].Successors:
                if successor[0] == self.shortest_route[n]:
                    next = successor[1]
                    break

            if next - previous == 0:
                cmd += 'M'
            elif (next - previous) % 4 == 1:
                cmd += 'R'
            elif (next - previous) % 4 == 2:
                cmd += 'B'
            elif (next - previous) % 4 == 3:
                cmd += 'L'
            previous = next

        cmd += 'S'

        return cmd[1:]

        '''
        cmd = "fbrls"
        cmds = ""
        for action in actions: cmds += cmd[action-1]
        print(cmds)
        return cmds
        '''

    def actions_to_str_2(self):
        # cmds should be a string sequence like "fbrl....", use it as the input of BFS checklist #1
        previous = 1  # 起始朝北
        next = 0
        cmd = ""

        for i in self.shortest_route_2[0:-1]:
            for successor in self.All_nodes[i-1].Successors:
                if successor[0] == self.shortest_route_2[self.shortest_route_2.index(i)+1]:
                    next = successor[1]
                    break

            if next - previous == 0:
                cmd += 'M'
            elif (next - previous) % 4 == 1:
                cmd += 'R'
            elif (next - previous) % 4 == 2:
                cmd += 'B'
            elif (next - previous) % 4 == 3:
                cmd += 'L'
            previous = next
        cmd += 'S'

        return cmd[1:]

    def strategy(self, nd):
        return self.BFS(nd)

    def strategy_2(self, nd_from, nd_to):
        return self.BFS_2(nd_from, nd_to)


if __name__ == '__main__':
    test = Maze("data/maze_1.csv")
    '''
    test.BFS_2(1,48)
    print(test.getActions_2())
    print(test.actions_to_str_2())

    test.BFS(14)
    print(test.getActions())
    print(test.actions_to_str())
    '''
    '''
    test.BFS_2(1,6)
    test.getActions_2()
    print(test.actions_to_str_2())

    '''
    test.BFS_continuous(1, 100)
    print(test.shortest_route)
    print(len(test.shortest_route))
    print(test.actions_to_str())

    #
