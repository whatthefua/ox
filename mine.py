import urllib2
import json

def computeShifts(pattern):
    shifts = [None] * (len(pattern) + 1)
    shift = 1

    for pos in range(len(pattern) + 1):
        while shift < pos and pattern[pos - 1] != pattern[pos - shift - 1]:
            shift += shifts[pos - shift - 1]

        shifts[pos] = shift
    return shifts

def kmpAllMatches(pattern, text):
    shift = computeShifts(pattern)
    startPos = 0
    matchLen = 0
    res = []
    for c in text:
        while matchLen >= 0 and pattern[matchLen] != c:
            startPos += shift[matchLen]
            matchLen -= shift[matchLen]

        matchLen += 1

        if matchLen == len(pattern):
            res.append(startPos)
            startPos += shift[matchLen]
            matchLen -= shift[matchLen]

    return res

def processTurn(fp, winner, obj):
    if obj["illegalMove"] != "":
        return

    data = obj["field"].split(",")

    for x in data:
        if int(x) & 12 != 0:
            return

    macroBoard = []
    board = []

    c = 0

    for x in data:
        if c % 3 == 0 and (c // 9 % 3) == 0 :
            macroBoard.append((int(x) & 48) >> 4)

        board.append(int(x) & 3)

        c += 1

    for x in macroBoard:
        fp.write("%d " % x)

    fp.write("\n")

    for x in board:
        fp.write("%d " % x)

    if 1 == int(winner[6]):
        fp.write("\n1\n")
    else:
        fp.write("\n0\n")

for I in range(1,236):
    response = urllib2.urlopen("http://theaigames.com/competitions/ultimate-tic-tac-toe/game-log/a/%d" % I)
    str = response.read()
    gameId = []
    prefixMatch = kmpAllMatches('href="http://theaigames.com/competitions/ultimate-tic-tac-toe/games/', str)

    for i in prefixMatch:
        tmp = ""
        c = i + 68

        while(str[c] != '"'):
            tmp += str[c]
            c += 1

        gameId.append(tmp)

    K = 1

    for url in gameId:
        response = urllib2.urlopen('http://theaigames.com/competitions/ultimate-tic-tac-toe/games/' + url + '/data')
        str = response.read()
        obj = json.loads(str)
        winner = obj["settings"]["winnerplayer"]
        fp = open('gamedata\\game-%d-%d.txt' % (I,K),'w')

        if len(winner) != 7:
            continue

        if len(obj["states"]) < 70:
            continue

        for turn in obj["states"]:
            processTurn(fp, winner, turn)

        fp.close()

        print "%d %d" % (I,K)

        K += 1