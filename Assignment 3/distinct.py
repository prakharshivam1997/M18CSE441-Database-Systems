#!/usr/bin/env python
# coding: utf-8

# In[9]:


from btree import *
import itertools
import sys, time


# In[10]:


hashmap = {}
INT_SIZE = sys.getsizeof(int())
typeOFIndex = 0


# In[11]:


def getNext(line, outputBuffer, outputBufferLength, btree):
    key = hash(line)
    if not typeOfIndex:
        if not btree.search(key):
            outputBuffer = outputBuffer + line
            outputBufferLength = outputBufferLength + 1
            btree.insert(key)
    else:
        try:
            val = hashmap[key]
        except:
            hashmap[key] = line
            outputBuffer = outputBuffer + line
            outputBufferLength = outputBufferLength + 1

    return outputBuffer, outputBufferLength


# In[12]:


def Open(filename, numberOfAttributes, numberOfBuffers, blockSize):
    inputBuffer = []
    outputBuffer = ""
    outputBufferLength = 0
    blockSize = int(blockSize)
    numberOfAttributes = int(numberOfAttributes)
    numberOfBuffers = int(numberOfBuffers)

    if not typeOfIndex:
        t = blockSize//INT_SIZE - 1
        btree = BTree(t)
    else:
        btree = None

    if numberOfBuffers <= 1:
        print("Error: Number of buffers should be greater than 2")
        return

    records_per_block = blockSize//(5*numberOfAttributes)

    N = (numberOfBuffers - 1) * records_per_block

    fout = open(filename[:-4]+'_output_'+str(typeOfIndex)+'.txt', 'w+')
    with open(filename, 'r') as f:
        for inputBuffer in iter(lambda: list(itertools.islice(f, N)), []):
            for line in inputBuffer:
                if len(line.strip().strip('\n').split(',')) != numberOfAttributes:
                    print("All rows do not contain same number of attributes")
                    return
                outputBuffer, outputBufferLength = getNext(line, outputBuffer,outputBufferLength, btree)

                if outputBufferLength == records_per_block:
                    fout.write(outputBuffer)
                    outputBufferLength = 0
                    outputBuffer = ""

        if outputBufferLength:
            fout.write(outputBuffer)
    fout.close()
    


# In[14]:


def main():
    global typeOfIndex
    if len(sys.argv) < 6:
        print("Usage: python3 distinct.py <filename> <numAttributes> <numBuffers> <blockSize> <typeOfIndex>")
        return
    elif sys.argv[5] == "btree":
        typeOfIndex = 0
    elif sys.argv[5] == "hash":
        typeOfIndex = 1
    startTime = time.time()
    Open(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4])
    print("%s sec" % (time.time() - startTime))
    return


# In[ ]:

if __name__ == '__main__':
    main()




