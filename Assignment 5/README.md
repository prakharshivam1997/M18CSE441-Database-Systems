# M18 CSE441 Database Systems Assignment 5
Exposed to MapReduce in Hadoop. Run on Oracle Virtual Machine.

#### Installation Instructions
Look through the Assignment 5 pdf for installation instructions.

#### To run 
```
hadoop jar /usr/lib/hadoop-0.20-mapreduce/contrib/streaming/hadoop-streaming-2.00-mr1-chd4.1.1.jar -mapper "python mapper.py" -reducer "python reducer.py" -file mapper.py -file reducer.py -input Input -output Output
```
```
mapper.py - mapper code in current local directory
reducer.py - reducer code in current local directory
Input - Directory in hadoop that contains the input
Output - Directory in hadoop to write the output (part-00000) to, shouldn't previously exist
```

#### Useful Links

https://classroom.udacity.com/courses/ud617
