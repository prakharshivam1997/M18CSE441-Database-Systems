#!/bin/bash
./generate R1.txt 50000
./generate S1.txt 50000
./join R1.txt S1.txt sort 50
./join R1.txt S1.txt sort 60
./join R1.txt S1.txt sort 70
./join R1.txt S1.txt sort 80
./join R1.txt S1.txt sort 90
./join R1.txt S1.txt sort 100
./join R1.txt S1.txt hash 50
./join R1.txt S1.txt hash 60
./join R1.txt S1.txt hash 80
./join R1.txt S1.txt hash 90
./join R1.txt S1.txt hash 100
wc -l R1_S1_sortjoin.txt
wc -l R1_S1_sortjoin.txt
comm -13 <(sort -u R1_S1_sortjoin.txt) <(sort -u R1_S1_hashjoin.txt)
comm -13 <(sort -u R1_S1_hashjoin.txt) <(sort -u R1_S1_sortjoin.txt)

