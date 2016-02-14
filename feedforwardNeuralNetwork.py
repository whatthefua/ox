############################################################
# defines class feedforwardNeuralNetwork
#
# attributes: __layer - number of layers
#             __nodesN[i] - list containing number of nodes in layer i
#             __weight[i][j][k] - weights connecting node j of layer i to node k of layer i + 1
#             __weightBias[i][j] - bias weight of node j of layer i + 1
#             __label - label of instance, will be used to interact with exe, must be distinct
############################################################

import os
import random
import shutil
import subprocess

class feedforwardNeuralNetwork:
    def __init__(self, layer, nodesN, label, weight = [], weightBias = []):
        """
        initialises an instance of feedforwardNeuralNetwork
        :arguments: layer - number of layers
                    nodesN - list containing number of nodes in each layer, starting with layer 0
                    label - label of instance, must be distinct
                    weight - list containing weights of instance in the format defined in class header
                           - file containing weights and bias weights of instance
                    weightBias - list containing bias weights of instance in the format defined in class header

                    if weight and weightBias are not given, instance will be initialised with random weights in range [-1,1]
        :return: none
        """
        self.__layer = layer
        self.__nodesN = nodesN
        self.__label = label

        if(weight != []):
            if isinstance(weight,basestring):
                fp = open(weight,'r')

                str = fp.read()
                res = str.split()
                c = 0

                self.__weight = [[[0 for k in range(self.__nodesN[i + 1])] for j in range(self.__nodesN[i])] for i in range(self.__layer - 1)]
                self.__weightBias = [[0 for k in range(self.__nodesN[i + 1])] for i in range(self.__layer - 1)]

                self.__layer = int(res[c])
                c += 1

                for i in range(self.__layer):
                    self.__nodesN[i] = int(res[c])
                    c += 1

                for i in range(self.__layer - 1):
                    for j in range(self.__nodesN[i]):
                        for k in range(self.__nodesN[i + 1]):
                            self.__weight[i][j][k] = float(res[c])
                            c += 1

                    for j in range(self.__nodesN[i + 1]):
                        self.__weightBias[i][j] = float(res[c])
                        c += 1

                fp.close()
            elif type(weight) is list:
                self.__weight = weight
                self.__weightBias = weightBias
        else:
            self.__weight = [[[((random.random() * 2) - 1) for k in range(self.__nodesN[i + 1])] for j in range(self.__nodesN[i])] for i in range(self.__layer - 1)]
            self.__weightBias = [[((random.random() * 2) - 1) for k in range(self.__nodesN[i + 1])] for i in range(self.__layer - 1)]

        fp = open(self.__label + 'Info.txt','w')

        fp.write('%d\n' % (self.__layer))

        for i in range(self.__layer):
            fp.write('%d ' % (self.__nodesN[i]))

        fp.write('\n')

        for i in range(self.__layer - 1):
            for j in range(self.__nodesN[i]):
                for k in range(self.__nodesN[i + 1]):
                    fp.write('%lf ' % (self.__weight[i][j][k]))

            fp.write('\n')

            for j in range(self.__nodesN[i + 1]):
                fp.write('%lf ' % (self.__weightBias[i][j]))

            fp.write('\n')

        fp.close()

    def feed(self, inp, out = 'feedOut.txt'):
        """
        feeds an input to instance
        :arguments: inp - list containing input vector
                        - string containing file name of input data
                    out - string containing file name of output data

                    if out is not given, output will be written to feedOut.txt

        :return: list containing output vector
                 nothing in case inp is a file

        input file
        line 1: number of input sets (n)
        line 2: first input vector
        line 3: second input vector
        .
        .
        .
        line n + 1: n-th input vector

        output file
        input file
        line 1: number of output sets (n)
        line 2: first output vector
        line 3: second output vector
        .
        .
        .
        line n + 1: n-th output vector
        """
        if type(inp) is list:
            fp = open(self.__label + 'Feed.txt','w')

            for i in range(self.__nodesN[0]):
                fp.write('%lf ' % (inp[i]))

            fp.close()

            subprocess.call(['feedforwardNeuralNetworkFeed.exe',self.__label + 'Info.txt',self.__label + 'Feed.txt',self.__label + 'FeedOut.txt'])

            os.remove(self.__label + 'Feed.txt')

            res = [0] * self.__nodesN[self.__layer - 1]

            fp = open(self.__label + 'FeedOut.txt','r')
            str = fp.read()

            res = str.split();

            for i in range(self.__nodesN[self.__layer - 1]):
                res[i] = float(res[i])

            fp.close()

            os.remove(self.__label + 'FeedOut.txt')

            return res
        elif isinstance(inp, basestring):
            subprocess.call(['feedforwardNeuralNetworkFeedList.exe',self.__label + 'Info.txt',inp,out])

    def learn(self, inp, outp, loops, alpha = 0.01):
        """
        performs backpropagation on instance
        :arguments: inp - list containing input vector
                        - string containing file name of training data
                    outp - list containing desired output vector
                    alpha - alpha value used in learning
        :return: none

        training file
        line 1: number of training sets (n)
        line 2: alpha value for first training data
        line 3: input vector of first training data
        line 4: desired output vector of first training data
        line 5: alpha value for second training data
        line 6: input vector of second training data
        line 7: desired output vector of second training data
        .
        .
        .
        line 3n - 1: alpha value for last training data
        line 3n: input vector of last training data
        line 3n + 1: desired output vector of last training data
        """
        if type(inp) is list:
            fp = open(self.__label + 'Learn.txt','w')

            fp.write('%lf\n' % alpha)

            for i in range(self.__nodesN[0]):
                fp.write('%lf ' % (inp[i]))

            fp.write('\n')

            for i in range(self.__nodesN[self.__layer - 1]):
                fp.write('%lf ' % (outp[i]))

            fp.close()

            subprocess.call(['feedforwardNeuralNetworkLearn.exe',self.__label + 'Info.txt',self.__label + 'Learn.txt', '%d' % loops])

            os.remove(self.__label + 'Learn.txt')

            fp = open(self.__label + 'Info.txt','r')
            str = fp.read()

            res = str.split()
            c = 0

            self.__layer = int(res[c])
            c += 1

            for i in range(self.__layer):
                self.__nodesN[i] = int(res[c])
                c += 1

            for i in range(self.__layer - 1):
                for j in range(self.__nodesN[i]):
                    for k in range(self.__nodesN[i + 1]):
                        self.__weight[i][j][k] = float(res[c])
                        c += 1

                for j in range(self.__nodesN[i + 1]):
                    self.__weightBias[i][j] = float(res[c])
                    c += 1

            fp.close()
        elif isinstance(inp,basestring):
            subprocess.call(['feedforwardNeuralNetworkLearnList.exe',self.__label + 'Info.txt', inp, '%d' % loops])

            fp = open(self.__label + 'Info.txt','r')
            str = fp.read()

            res = str.split()
            c = 0

            self.__layer = int(res[c])
            c += 1

            for i in range(self.__layer):
                self.__nodesN[i] = int(res[c])
                c += 1

            for i in range(self.__layer - 1):
                for j in range(self.__nodesN[i]):
                    for k in range(self.__nodesN[i + 1]):
                        self.__weight[i][j][k] = float(res[c])
                        c += 1

                for j in range(self.__nodesN[i + 1]):
                    self.__weightBias[i][j] = float(res[c])
                    c += 1

            fp.close()

    def exportWeight(self, targ):
        """
        writes the weights of instance to file
        :arguments: targ - file name
        :return: none
        """
        fp = open(targ,'w')

        fp.write('%d\n' % (self.__layer))

        for i in range(self.__layer):
            fp.write('%d ' % (self.__nodesN[i]))

        fp.write('\n')

        for i in range(self.__layer - 1):
            for j in range(self.__nodesN[i]):
                for k in range(self.__nodesN[i + 1]):
                    fp.write('%lf ' % (self.__weight[i][j][k]))

            fp.write('\n')

            for j in range(self.__nodesN[i + 1]):
                fp.write('%lf ' % (self.__weightBias[i][j]))

            fp.write('\n')

        fp.close()

    def __del__(self):
        """
        destroys class and deletes files related to instance
        :arguments: none
        :return: none
        """
        os.remove(self.__label + 'Info.txt')