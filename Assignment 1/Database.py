import csv

class Table:
    def __init__(self, name, columns):
        self.name = name
        self.columns = columns
        self.data = []
        with open(name+".csv", "r") as FObj:
            reader = csv.reader(FObj)
            for row in reader:
                row = list(map(int, row))
                self.data.append(row)

    def printTable(self):
        print(self.columns)
        print(self.data)

class Database:
    def __init__(self):
        self.tables = []
        self.tableNames = []
        tableColumns = []
        with open("metadata.txt","r") as FObj:
            readBeginTable = False
            readTableName = False
            for row in FObj:
                row = row.strip()
                if row == "<begin_table>":
                    readBeginTable = True
                    continue
                if readBeginTable == True:
                    if readTableName == False:
                        readTableName = True
                        self.tableNames.append(row)
                        continue 
                    else:
                        if row == "<end_table>":
                            readBeginTable = False
                            readTableName = False
                            t = Table(self.tableNames[-1], tableColumns)
                            self.tables.append(t)
                            tableColumns = []
                        else:
                            tableColumns.append(row)
    
    def printDatabase(self):
        print(self.tableNames)
        for t in self.tables:
            t.printTable()

if __name__ == "__main__":
    D = Database()
    D.printDatabase()
            

