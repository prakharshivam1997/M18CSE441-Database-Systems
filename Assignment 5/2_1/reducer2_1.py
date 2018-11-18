#Reducer.py
#!/usr/bin/python

import sys
count = 0
max = 0
oldKey = None
d = {}
for line in sys.stdin:
    data = line.strip().split("\t")
    if len(data) != 2:
        continue
    thisKey, value = data
    value = int(value)
    if oldKey and oldKey != thisKey:
        d[oldKey] = count
        if count > max:
            max = count
        oldKey = thisKey
        count = 0
    oldKey = thisKey
    count += value

if oldKey!=None:
    d[oldKey] = count
    if count > max:
        max = count

for country in d:
    if d[country] == max:
        print country
