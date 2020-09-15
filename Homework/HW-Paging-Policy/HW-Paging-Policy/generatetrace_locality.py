import random
from operator import itemgetter

random.seed(0)
numaddrs = 10
maxpage = 10
count = {}
addrList = []

while(1):
    for i in range(numaddrs):
        # genererate random number
        n = int(maxpage * random.random())
        # print("this is n:")
        # print(n)
        # print("\n")
        addrList.append(n)
        if n in count:
            count[n] += 1
        else:
            count[n] = 1
    # print("this is count:")
    # print(count)

    sortedCount = sorted(count.items(), key=itemgetter(1), reverse=True)
    # print("this is sortedCount:")
    # print(sortedCount)
    countSum = 0
    # sum the first to count access 
    for k in range(int(0.2 * numaddrs)):
        countSum += sortedCount[k][1]
        # print(sortedCount[k][1])
    # print("this is countSum")
    # print(countSum)
    # if it is greater than 0.8 this code is access frequently
    if countSum / numaddrs >= 0.8:
        break
    # clean 
    else:
        count = {}
        addrList = []
print(addrList)

