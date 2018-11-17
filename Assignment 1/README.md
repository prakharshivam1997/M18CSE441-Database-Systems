# M18 CSE441 Database Systems Assignment 1
Shady ad hoc Python3 implementation of a Mini MySQL Engine that handles the following queries (also specified in Assignment1.pdf.
Not handled any aliasing.
#### To run
Install sqlparse
```
pip3 install sqlparse
```
Run
```
python3 MySQLEngine.py "<Query>"
```

#### Runable Queries
```
SELECT * FROM <table_name>;
```
Returns all columns and rows from table, error handling done for table existence
```
SELECT <max/min/sum/avg> <column_name> FROM <table_name>;
```
Returns aggregate of trivially integer column, no error checking done
```
SELECT <col1>, <col2> FROM <table_1>, <table_2> ...;
```
Returns selected columns from any number of tables, no alias resolving done, if a column is present in both tables, it's printed twice.
```
SELECT DISTINCT <col1>, <col2> FROM <table1> <table2>;
```
Returns distinct pairs of col1 and col2
```
SELECT <col1>, <col2> FROM <table1>, <table2> 
WHERE <col1> < = / >= / <= / > / < > <val1> <AND/OR> <col2> < = / >= / <= / > / < > <val2>;
```
Where there is atmost one AND / OR
```
SELECT < * / <col1>, <col2> > FROM <table1>, <table2> 
WHERE <col1> = <col2>;
```
Join condition implemented without resolving which columns belong to which table, bug
