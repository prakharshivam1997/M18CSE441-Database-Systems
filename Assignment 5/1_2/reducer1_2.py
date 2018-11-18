#Reducer.py
#!/usr/bin/python

import sys
count = 0
oldKey = None
for line in sys.stdin:
    data = line.strip().split("\t")
    if len(data) != 2:
        continue
    thisKey, value = data
    value = int(value)
    if oldKey and oldKey != thisKey:
        print oldKey, "\t", count
        oldKey = thisKey
        count = 0
    oldKey = thisKey
    count += value

if oldKey!=None:
    print oldKey,"\t",count
