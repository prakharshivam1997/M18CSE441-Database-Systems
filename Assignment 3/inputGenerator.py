#Input Generator
import sys
import random

def main():
    if len(sys.argv) < 4:
        print("Error: Usage: python3 inputGenerator.py <fileSize> <MB/GB> <numOfAttributes> <percentage of repetition>")
        return

    fileSize = int(sys.argv[1])
    fileSize = fileSize * 1024 * 1024 #size in bytes
    if sys.argv[2] == 'GB':
        fileSize = fileSize * 1024

    numOfAttributes = int(sys.argv[3])
    r = int(sys.argv[4])
    #Input - 3digit numbers 
    recordSize = 5* numOfAttributes
    numberOfRecords = fileSize // recordSize
    numberOfRecords = numberOfRecords - numberOfRecords % (100 + r)
    f = open(sys.argv[1]+sys.argv[2]+"_"+sys.argv[4]+"Percent_"+sys.argv[3]+"_"+str(numberOfRecords)+"_"+str(recordSize)+".txt","w+")
    rArray = ""
    for i in range(0,int(numberOfRecords/(100+r))):
        rArray = ""
        for j in range(0,100):
            record = ""
            for k in range(0,numOfAttributes-1):
                record = record + str(random.randint(100,999)) + ", "
            record = record + str(random.randint(100,999)) + "\n"
            f.write(record)
            if(j < r):
                rArray = rArray + record
        f.write(rArray)
    f.close() 

if __name__ == '__main__':
    main()
    
    