//Database Systems Assignment
//Assignment 2 - Two Phase Merge Sort
//Compilation - g++ -o sort sort.cpp

#include <iostream>
#include <string>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <ctime>

using namespace std;
string inputFile;//Holds name of Input File
string outputFile;//Holds name of Output File
int desc = 0;//set to 1 if desc else 0
int memSize = 0;//main memory size in bytes
int subListSize = 0;//Size of one sublist ~ number of records main memory can hold
int tupleSize = 0;//size of one record
int fileNo = 0;//stores number of sublists
int blockSize = 0;//Block size in no of records
map<string, string> minRecord;//holds min record while merging
map<string,int> metadata;//holds metadata information
vector<string> compColumns;//holds comparision columns
vector<map<string, string>> outputBuffer;//output buffer
map<string, string> record;//global record`
vector<map<string, string>> subList;//global sublist
int flag = 0;

//Function to Check if file exists
int fileExists(string fileName)
{
  struct stat buffer;
  if(stat(fileName.c_str(), &buffer)==0)
    return 1;
  return 0;
}

//Function to find metadata and size of a tuple
void getMetadata()
{
  ifstream myFile;
  myFile.open("metadata.txt", ios::in);
  if(!myFile.is_open())
  {
    cout<<"\nErr: Couldn't open the Metadata File\n";
    exit(0);
  }
  string line;
  string column;
  string size;
  while(getline(myFile, line))
  {
    stringstream linestream(line);
    getline(linestream, column, ',');
    getline(linestream, size, '\n');
    tupleSize += atoi(size.c_str());
    metadata.insert(pair<string, int> (column, atoi(size.c_str())));
  }
  myFile.close();
}

//Function to write to outputFile
void writeToOutput()
{
  map<string, string>::iterator itr;
  ofstream myFile;
  string line;
  myFile.open(outputFile, ios::app);
  for(int i = 0; i<outputBuffer.size(); i++)
  {
    itr = outputBuffer[i].begin();
    line = itr->second;
    ++itr;
    for(;itr != outputBuffer[i].end(); itr++)
      line = line +"  "+ itr->second;
    myFile << line.c_str()<<"\r"<<endl;
  }
  myFile.close();
}

//Function to write to Temporary file
void insertIntoFile()
{
  cout<<"\nTemp File Created";
  string fileName = "File" + to_string(fileNo) + ".txt";
  map<string, string>::iterator itr;
  ofstream myFile;
  string line;
  myFile.open(fileName, ios::out | ios::trunc);
  for(int i = 0; i<subList.size(); i++)
  {
    itr = subList[i].begin();
    line = itr->second;
    ++itr;
    for(;itr != subList[i].end(); itr++)
      line = line +"  "+ itr->second;
    myFile << line.c_str()<<"\r"<<endl;
  }
  myFile.close();
}

//Function to delete all temporary files
void deleteTemporaryFiles()
{
  string fileName = "";
  for(int i=1;i<=fileNo; i++)
  {
    fileName = "File"+to_string(i)+".txt";
    if(remove(fileName.c_str())!=0)
    {
      cout<<"\nErr: File "+fileName+" : No such file or directory exists";
      exit(0);
    }
  }
}

//Function to read a record
void readRecord(stringstream &linestream)
{
  record.clear();
  char ch;
  map<string, int>::iterator itr;
  for(itr = metadata.begin(); itr != metadata.end(); ++itr)
  {
    char *buffer = new char[itr->second + 1];
    linestream.read(buffer, itr->second);
    buffer[itr->second] = '\0';
    record.insert(pair<string, string> (itr->first, buffer));
    if(!linestream.eof())
    {
      linestream.get(ch);
      linestream.get(ch);
    }
  }
}


int main(int argc, char *argv[])
{
  //Error checking
  clock_t start;
  start = clock();
  if(argc < 6)
  {
    cout<<"Err: Give command line arguments\n";
    cout<<"Ex: ./sort input.txt output.txt 50 asc c0 c1\n";
    return(0);
  }

  //Parsing input arguments
  inputFile = argv[1];

  if(!fileExists(inputFile))
  {
    cout<<"\nErr: Input File Doesn't exist\n";
    return(0);
  }
  outputFile = argv[2];
  if(fileExists(outputFile))
  {
    if(remove(outputFile.c_str())!=0)
    {
      cout<<"\nErr: Output file with same name exists and cannot be deleted";
      exit(0);
    }
  }
  memSize = stoi(argv[3])*1024*1024;
  if(strcmp(argv[4],"desc")==0)
    desc = 1;
  else if(strcmp(argv[4],"asc")!=0)
  {
    cout<<"\nErr: Arguments should contain asc or desc only\n";
    return(0);
  }
  if(!fileExists("metadata.txt"))
  {
    cout<<"\nErr: Metadata File Doesn't Exist\n";
    return(0);
  }

  //Get metadata information
  getMetadata();

  //Checking if comparision columns present in file
  for(int i=5; i<argc; i++)
  {
    if(metadata.find(argv[i]) == metadata.end())
    {
      cout<<"\nErr: Column "<<argv[i]<<" not Present in Metadata File";
      return(0);
    }
    compColumns.push_back(argv[i]);
  }
  subListSize = memSize / tupleSize;

  //PHASE 1
  string line;
  map<string, int>::iterator itr;
  ifstream myFile1;
  myFile1.open(inputFile, ios::in);
  if(!myFile1.is_open())
  {
    cout<<"\nErr: Couldn't open input File\n";
    exit(0);
  }
  while(getline(myFile1, line))
  {
    stringstream linestream(line);
    readRecord(linestream);
    vector<map<string, string>>::iterator i;
    int foundPos = 0;
    for(i=subList.begin();i!=subList.end();i++)
    {
      foundPos = 0;
      for(int j=0; j<compColumns.size(); j++)
      {
        const char *a = ((record.find(compColumns[j]))->second).c_str();
        const char *b = (((*i).find(compColumns[j]))->second).c_str();
        int Len = (metadata.find(compColumns[j]))->second;
        if(memcmp(a,b,Len) > 0)
        {
          if(desc)
            foundPos = 1;
          break;
        }
        else if(memcmp(a,b,Len) == 0)
          continue;
        else if(memcmp(a,b,Len) < 0)
        {
          if(!desc)
            foundPos = 1;
          break;
        }
      }
      if(foundPos)
        break;
    }
    subList.emplace(i, record);
    if(subList.size()==subListSize)
    {
      ++fileNo;
      cout<<"\nCalled Insert into file";
      insertIntoFile();
      subList.clear();
    }
    record.clear();
  }
  if(!subList.empty())
  {
    ++fileNo;
    cout<<"\nCalled Insert into file";
    insertIntoFile();
    subList.clear();
  }
  myFile1.close();

  //PHASE 2
  blockSize = subListSize / (fileNo + 1);
  vector<map<string, string>> blocks[fileNo];
  ifstream myFile[fileNo];
  string fileName;
  int blockIndex;
  map<string, string>::iterator itr1;
  for(int i = 1; i<=fileNo; i++)
  {
    fileName = "File"+to_string(i)+".txt";
    myFile[i-1].open(fileName);
    if(!myFile[i-1].is_open())
    {
      cout<<"\nErr: Couldn't open "<<fileName;
      exit(0);
    }
  }

  while(1)
  {
    minRecord.clear();
    for(int i=0; i<fileNo; i++)
    {
      if(blocks[i].empty())
      {
        while(getline(myFile[i], line))
        {
          stringstream linestream(line);
          readRecord(linestream);
          blocks[i].emplace_back(record);
          if(blocks[i].size()==blockSize)
            break;
        }
      }
      if(minRecord.empty() && !blocks[i].empty())
      {
        minRecord = *((blocks[i]).begin());
        blockIndex = i;
      }

      else if(!blocks[i].empty() && blockIndex!= i)
      {
        for(int j=0; j<compColumns.size(); j++)
        {
          const char *a = ((minRecord.find(compColumns[j]))->second).c_str();
          record = *(blocks[i].begin());
          const char *b = ((record.find(compColumns[j]))->second).c_str();
          int Len = (metadata.find(compColumns[j]))->second;
          if(memcmp(a,b,Len) > 0)
          {
            if(!desc)
            {
              minRecord = record;
              blockIndex = i;
              break;
            }
          }
          else if(memcmp(a,b,Len) == 0)
            continue;
          else if(memcmp(a,b,Len) < 0)
          {
            if(desc)
            {
              minRecord = record;
              blockIndex = i;
              break;
            }
          }
        }
      }
    }
    blocks[blockIndex].erase(blocks[blockIndex].begin());
    outputBuffer.emplace_back(minRecord);
    minRecord.clear();
    if(outputBuffer.size()==blockSize)
    {
      writeToOutput();
      outputBuffer.clear();
    }

    flag = 0;
    for(int i = 0; i<fileNo; i++)
    {
      if(!myFile[i].eof() || !blocks[i].empty())
      {
        flag = 1;
        break;
      }
    }
    if(!flag)
      break;
  }
  if(!outputBuffer.empty())
  {
    writeToOutput();
    outputBuffer.clear();
  }

  for(int i = 0; i<fileNo; i++)
    myFile[i].close();
  deleteTemporaryFiles();
  double duration = (clock() - start)/double(CLOCKS_PER_SEC);
  cout<<"\n"<<duration<<"\n";
  return(0);
}
