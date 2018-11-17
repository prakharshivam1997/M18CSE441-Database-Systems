# M18 CSE441 Database Systems Assignment 4
C++ code that performs Sort Merge Join (Efficient) and Hash join (Grace Hash join) on 2 relations R(Y,X) and S(Y,Z) where Y is the joining attribute

#### Compilation
To compile the code run the following command
```
g++ InputGenerator.cpp -o generate
g++ code.cpp -o join
```

#### Input Generation
To generate input file that contains 2 attributes, with the joining attribute being the first attribute run the following command
```
./generate <input file name> <number of tuples>
```

#### Sort Merge Join
To perform sort merge join on 2 files, say R1 and R2, run the following command
```
./join R1.txt R2.txt sort <number of blocks>
```
The output gets saved in a file called R1_R2_sortjoin.txt
Tuples per block = 100

#### Hash Join
To perform hash join on 2 files, say R1 and R2, run the following command
```
./join R1.txt R2.txt hash <number of blocks>
```
The output gets saved in a file called R1_R2_sortjoin.txt
Tuples per block = 100

#### Comparision
To compare 2 files, file 1 and file 2 and to see what is in file1 but not in file 2, run the following command
```
comm -13 <(sort -u file2.txt) <(sort -u file1.txt)
```
