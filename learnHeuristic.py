'''
import feedforwardNeuralNetwork
import time

tic = time.clock()
fNN = feedforwardNeuralNetwork.feedforwardNeuralNetwork(4,[30,20,5,1],'fNN-30-20-5-1-lim','fNN-30-20-5-1-Weight50-lim.txt')

toc = time.clock()
print toc - tic

fNN.learn('train_short_lim_fine.txt','',50)

toc = time.clock()
print toc - tic

fNN.exportWeight('fNN-30-20-5-1-Weight100-lim.txt')

toc = time.clock()
print toc - tic
'''

import feedforwardNeuralNetwork
import time

tic = time.clock()
fNN = feedforwardNeuralNetwork.feedforwardNeuralNetwork(4,[30,20,5,1],'fNN-30-20-5-1','fNN-30-20-5-1-Weight100-lim.txt')

toc = time.clock()
print toc - tic

fNN.feed('rawtest.txt','testout.txt')

toc = time.clock()
print toc - tic