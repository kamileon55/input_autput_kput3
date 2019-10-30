#!/usr/bin/python3

from sys import stdin, stderr, stdout
from enum import Enum

class Direction(Enum):
    LEFT = 'l'
    RIGHT = 'r'

class Data:
    def __init__(self, it):
        self.currRouter = int(next(it))
        self.currStoreId = int(next(it))
        self.dataIndex = int(next(it))
        self.messageId = int(next(it))
        self.fromRouter = int(next(it))
        self.toRouter = int(next(it))
        self.direction = Direction(next(it)[0])
        

class MessagePiece:
    def __init__(self, it):
        self.index = int(next(it))
        self.message = next(it)


class Reader:
    def __init__(self):
        self.data = [0]*3
        self.previous = ""
        self.routerBits = [[False]*10 for i in range(14)]
        self.dataArray = []
        self.receivedPieces = []
        self.hasEnd = False

def readData(reader):
    reader.dataArray.clear()
    reader.receivedPieces.clear()

    while True:
        line = stdin.readline().rstrip()
        if len(line) == 0:
            break
        it = iter(line.split(" "))
        command = next(it)
        if command == ".":
            return
            
        if command == "WRONG" or command == "SCORE" or command == "TICK":
            reader.hasEnd = True
            reader.previous = line
        elif command == "REQUEST":
            for i in range(3):
                reader.data[i] = int(next(it))
        elif command == "PREVIOUS":
            reader.previous = line.substr(9)
        elif command == "ROUTER":
            routerIndex = int(next(it))
            bits = next(it)
            for i in range(10):
                reader.routerBits[routerIndex][i] = bits[i] == '1'
        elif command == "DATA":
            reader.dataArray.append(Data(it))
        elif command == "MESSAGE":
            reader.receivedPieces.append(MessagePiece(it))
        else:
            stderr.write("READER ERROR HAPPENED: unrecognized command line: {}\n".format(line))
            reader.hasEnd = True
            return

    stderr.write("Unexpected input end.\n")
    reader.hasEnd = True

if __name__== "__main__":
    teamToken = "tqEzVLvbq6wz_uWr6HS1"
    # seed = 0

    stdout.write("START " + teamToken 
    #    + " " + str(seed)
        + "\n")
        
    reader = Reader()
    command = "CREATE 0 0"
    while True:
        readData(reader)
        
        if reader.hasEnd:
            break
            
        # TODO logika jobb mint a semmitteves
        
        
        # Ha szeretnetek debug uzenetet kuldeni, akkor megtehetitek.
        # Vigyazzatok, mert maximalisan csak 1024 * 1024 bajtot kaptok vissza
        stderr.write("Send {}\n".format(command))
        command = "PASS"
        
        # standard out-ra meg mehet ki a megoldas! Mas ne irodjon ide ki ;)
        stdout.write("{} {} {} {}\n".format(reader.data[0], reader.data[1], reader.data[2], command))

    stderr.write("END (latest message): {}\n".format(reader.previous))
