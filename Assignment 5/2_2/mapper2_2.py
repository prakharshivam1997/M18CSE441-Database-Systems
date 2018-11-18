#Mapper.py
#!/usr/bin/python

import sys

for line in sys.stdin:
    data = line.strip().split(', ')
    try:
        ID, typeOfAirport, name, latitude, longitude, country, region = data
        print ("{0}\t{1}".format(region,1))
    except:
        continue


