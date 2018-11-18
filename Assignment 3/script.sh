#!/bin/sh
python3 inputGenerator.py 1 GB 20 20
python3 distinct.py 1GB_20Percent_20_10737360_100.txt 20 20 100000 btree
python3 distinct.py 1GB_20Percent_20_10737360_100.txt 20 20 100000 hash
python3 distinct.py 1GB_20Percent_20_10737360_100.txt 20 40 100000 btree
python3 distinct.py 1GB_20Percent_20_10737360_100.txt 20 40 100000 hash
python3 distinct.py 1GB_20Percent_20_10737360_100.txt 20 60 100000 btree
python3 distinct.py 1GB_20Percent_20_10737360_100.txt 20 60 100000 hash
python3 distinct.py 1GB_20Percent_20_10737360_100.txt 20 80 100000 btree
python3 distinct.py 1GB_20Percent_20_10737360_100.txt 20 80 100000 hash
python3 distinct.py 1GB_20Percent_20_10737360_100.txt 20 100 100000 btree
python3 distinct.py 1GB_20Percent_20_10737360_100.txt 20 100 100000 hash
python3 distinct.py 1GB_20Percent_20_10737360_100.txt 20 120 100000 btree
python3 distinct.py 1GB_20Percent_20_10737360_100.txt 20 120 100000 hash
python3 distinct.py 1GB_20Percent_20_10737360_100.txt 20 150 100000 btree
python3 distinct.py 1GB_20Percent_20_10737360_100.txt 20 150 100000 hash