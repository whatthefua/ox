import feedforwardNeuralNetwork
import time

'''
fpI = open('train-images.idx3-ubyte','rb')
fpL = open('train-labels.idx1-ubyte','rb')

fpO = open('training-set.txt','w')

fpO.write('60000\n')

fpI.read(16)
fpL.read(8)

for i in range(60000):
    print i

    fpO.write('0.01\n')

    for j in range(784):
        fpO.write('%lf ' % (float(int(fpI.read(1).encode('hex'),16)) / 256))

    fpO.write('\n')

    c = int(fpL.read(1).encode('hex'),16)

    for j in range(10):
        if(j == c):
            fpO.write('1 ')
        else:
            fpO.write('0 ')

    fpO.write('\n')

fpI.close()
fpL.close()
fpO.close()
'''

tic = time.clock()
fNN = feedforwardNeuralNetwork.feedforwardNeuralNetwork(3,[784,300,10],'fNN')
fNN.learn('training-set.txt')
fNN.exportWeight('fNN-Weight1.txt')
toc = time.clock()

print toc - tic