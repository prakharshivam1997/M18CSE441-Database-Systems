import sys
import sqlparse
import re
from Database import Database, Table

def getNewData(columns, query, tables, data, dataColumns):
    newData = []
    if len(columns) == 1:
        if (not len(columns[0]) == 1) or (not columns[0].isdigit()):
            print("Err: column with no comparision")
            exit()
        elif int(columns[0]):
            return data
        else:
            return newData
    columns = [column.strip() for column in columns]
    columns = [i.split(".") for i in columns]
    columns = [i[::-1] for i in columns]
    #print(columns)
    columnIndex = []
    for column in columns:
        if len(column) == 2:
            tableInTables = False
            for table in tables:
                if column[1] == table.name:
                    tableInTables = True
            if not tableInTables:
                print("Err: "+column[1]+" not in FROM clause")
                exit()
            if not column[0] in DB.tables[DB.tableNames.index(column[1])].columns:
                print("Err: Attribute "+column[0]+" doesn't exist in table "+column[1])
                exit()
        if not column[0].isdigit() and not column[0] in dataColumns:
            print("Err: No such attribute exists: "+column[0])
            exit()
        if column[0].isdigit():
            columnIndex = columnIndex + [-1]
        else:
            indices = [i for i, x in enumerate(dataColumns) if x == column[0]]
            #print(indices)
            if len(indices) == 1:
                columnIndex = columnIndex + [indices[0]]
            else:
                if len(column) == 1:
                    print("Err: Ambiguous attribute")
                    exit()
                else:
                    k = 0
                    for table in tables:
                        if column[0] in table.columns:
                            if not table.name == column[1]:
                                k= k+1
                            else:
                                break
                    columnIndex = columnIndex + [indices[k]]
    #print(columnIndex)
    if (not columnIndex[0] == -1) and (not columnIndex[1] == -1) and "=" in query:
        global JOIN
        JOIN = True
        global joinIndices
        joinIndices = columnIndex
        #print(joinIndices)
    #print(columns)
    for row in data:
        if columnIndex[0] == -1:
            val1 = int(columns[0][0])
        else:
            val1 = row[columnIndex[0]]
        if columnIndex[1] == -1:
            val2 = int(columns[1][0])
            #print(val2)
        else:
            val2 = row[columnIndex[1]]

        if ">=" in query:
            if val1 >= val2:
                newData.append(row)
        elif "<=" in query:
            if val1 <= val2:
                newData.append(row)
        elif "<" in query:
            if val1 < val2:
                newData.append(row)
        elif ">" in query:
            if val1 > val2:
                newData.append(row)
        elif "=" in query:
            if val1 == val2:
                #print("Reached here")
                newData.append(row)
        else:
            print("Err: Invalid condition in WHERE")
            exit()
    return newData
    #print(newData)
    #print(len(newData))
    #print(columnIndex)

def handleWhere(query, tables, data, dataColumns):
    query = query.split('\n')
    query = [i.strip() for i in query]
    OR = False
    AND = False
    singleCondition = True
    cond1 = []
    cond2 = []
    data2 = []
    data1 = []
    newData = []
    if len(query) > 2:
        print("Err: Expected only one AND or OR in WHERE clause")
        exit()
    if len(query) == 2:
        singleCondition = False
        if "OR" in query[1]:
            OR = True
            query[1] = query[1][3:]
        elif "AND" in query[1]:
            AND = True
            query[1] = query[1][4:]
        else:
            print("Err: Only single AND or OR allowed")
            exit()
        cond2 = re.split(r'[=><]+',query[1])
        data2 = getNewData(cond2, query[1], tables, data, dataColumns)
        print (cond2)
    query[0] = query[0][6:]
    cond1 = re.split(r'[=><]+',query[0])
    data1 = getNewData(cond1, query[0], tables, data, dataColumns)
    #print(data1)
    #print (cond1)
    if singleCondition:
        return data1
    for row in data:
        if AND:
            if row in data1 and row in data2:
                newData.append(row)
        elif OR:
            if row in data1 or row in data2:
                newData.append(row)
    return newData
    

def getData(tables):
    table1 = tables[0].data
    data = table1
    for t in range(1,len(tables)):
        table2 = tables[t].data
        data = []
        for i in range(len(table1)):
            for j in range(len(table2)):
                data.append(table1[i]+table2[j])
        table1 = data
    return data

def getTables(fromTables):
    tables= []
    tablesNames = fromTables.strip()
    tablesNames = tablesNames.split(',\n')
    tablesNames = [x.strip() for x in tablesNames]
    #print(tablesNames)
    for table in tablesNames:
        if not table in DB.tableNames:
            print("Err: "+table+" not in Database")
            exit()
        tables.append(DB.tables[DB.tableNames.index(table)])
    return tables

def getDataColumns(tables):
    dataColumns = []
    for table in tables:
        dataColumns = dataColumns + table.columns
    return dataColumns

def printer(DISTINCT, query, data, dataColumns):
    global JOIN
    global joinIndices
    printIndices = []
    newData = []
    if query[0] == '*':
        query = dataColumns
    for q in query:
        if not q in dataColumns:
            print("Err: out of scope")
            exit()
        indices = [i for i, x in enumerate(dataColumns) if x == q]
        printIndices = printIndices + indices
    for i in range(len(query)):
        if (not JOIN or (JOIN and not i == joinIndices[1])):
            print(query[i]+", ",end = " ")
    print()
    for row in data:
        row1 = []
        for i in range(len(row)):
            if i in printIndices and ((not JOIN) or (JOIN and not i == joinIndices[1])):
                #print(joinIndices)
                row1 = row1 + [row[i]]
        if DISTINCT:
            if not row1 in newData:
                newData.append(row1)
        else:
            newData.append(row1)
    for row in newData:
        for i in range(len(row1)):
            print(str(row[i])+", ",end=" ")
        print()
        
    return

def project(DISTINCT, query, tables, data, dataColumns):
    query = query.split(",")
    query = [x.strip() for x in query]
    # if len(query) == 1 and query[0] == '*':
    #     printer(DISTINCT, ["*"], data, dataColumns)
    if len(query) == 1:
        query = re.split(r'[\ \(\)]+',query[0])
        if "sum" == query[0].lower():
            if not query[1] in dataColumns:
                print("Err: column out of scope")
                exit()
            else:
                i = dataColumns.index(query[1])
                sum = 0
                for row in data:
                    sum = sum + row[i]
                print("Sum = "+str(sum))
                exit()
        elif "max" == query[0].lower():
            if not query[1] in dataColumns:
                print("Err: column out of scope")
                exit()
            else:
                i = dataColumns.index(query[1])
                max = -1
                for row in data:
                    if row[i] > max:
                        max = row[i]
                print("Max = "+str(max))
                exit()
        elif "min" == query[0].lower():
            if not query[1] in dataColumns:
                print("Err: column out of scope")
                exit()
            else:
                i = dataColumns.index(query[1])
                min = 10000000000
                for row in data:
                    if row[i] < min:
                        min = row[i]
                print("Min = "+str(min))
                exit()
        if "avg" == query[0].lower():
            if not query[1] in dataColumns:
                print("Err: column out of scope")
                exit()
            else:
                i = dataColumns.index(query[1])
                sum = 0
                count = 0
                for row in data:
                    sum = sum + row[i]
                    count = count + 1
                avg = sum/count
                print("Avg = "+str(avg))
                exit()
    printer(DISTINCT, query, data, dataColumns)

def SQLQueryParser(query):
    WHERE = False
    DISTINCT = False
    if(not (query[-1]==';')):
        print("Syntax Err: Expected ';' at the end")
        return
    query = query[:-1]
    query = sqlparse.format(query, reindent=True, keyword_case = 'upper')
    query1 = sqlparse.sql.IdentifierList(sqlparse.parse(query)[0].tokens).get_identifiers()
    #print(query1)
    query = []
    for command in query1:
        query.append(str(command))
    #print(query)
    if len(query) < 4:
        print("Err: Expected command like SELECT __ FROM __;")
        return
    if not query[0] == 'SELECT':
        print("Err: Expected SELECT at the begining")
        return
    if not (query[3] == 'FROM' or query[2] == 'FROM'):
        print("Err: Expected a FROM clause or of form SELECT DISTINCT __ FROM __;")
        return
    if query[1] == 'DISTINCT':
        DISTINCT = True
        tables = getTables(query[4])
        if len(query) >= 6:
            WHERE = True
    else:
        tables = getTables(query[3])
        if len(query) >= 5:
            WHERE = True
    data = getData(tables)
    dataColumns = getDataColumns(tables)
    #print(dataColumns)
    #print(data)
    if WHERE == True:
        data = handleWhere(query[-1], tables, data, dataColumns)
    if DISTINCT:
        project(DISTINCT, query[2], tables, data, dataColumns)
    else:
        project(DISTINCT, query[1], tables, data, dataColumns)
    #print(query)
    #print(data)
    


if __name__ == "__main__":
    DB = Database()
    #DB.printDatabase()
    query = sys.argv[1]
    joinIndices = []
    JOIN = False
    SQLQueryParser(query)
    
