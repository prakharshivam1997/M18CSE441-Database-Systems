/* for sort based
Condition: B(R) + B(S) <= M^2
for hash
Condition: Min(B(R),B(S)) <= M^2
*/

//Insert linear probbing
#include<math.h>
#include<iostream>
#include<string>
#include<cstring>
#include<fstream>
#include<algorithm>
#include<stdio.h>
#include <chrono>

using namespace std;

string pathToR = "";
string pathToS = "";
string RFile = "";
string SFile = "";
string outputFile = "";
int M = 0;
int Method = 0; //Default to Hashing

int RRecords = 0;
int SRecords = 0;

int blockSize = 100;
int Rnum = 0;
int Snum = 0;
int Rsize = 0;
int Ssize = 0;

int hash2(string line)
{
	int value = 0;
	for(int i=0; i<line.size()-1; i++)
	{
		value += (int)line[i];
	}
	return (value) % (M-1);
}

void getNextHash(int *RhashBucketSize, int *ShashBucketSize)
{
	string R = "";
	string S = "";
	ifstream Rread;
	string hashBuckets[M-1][100];
	int hashBucketsPointer[M-1];
	int hashKey = 0;
	string line = "";
	string Rkey = "";
	string Skey = "";
	string outputBuffer[blockSize];
	int outputBufferPointer=0;
	ofstream Rout(outputFile);
    int probbing[M-1][M-2];

	for (int i=0; i<M; i++)
	{
		for(int j=0; j<M-1; j++)
			hashBucketsPointer[j] = 0;

        for(int j=0; j<M-1; j++)
        {
            for(int k=0; k<M-2; k++)
            {
                probbing[j][k] = -1;
            }
        }

		if(!RhashBucketSize[i] || !ShashBucketSize[i])
			continue;
		if (RhashBucketSize[i] <= ShashBucketSize[i])
		{
			R = "R";
			S = "S";
		}
		else
		{
			R = "S";
			S = "R";
		}
		Rread.open(R+"File"+to_string(i)+".txt");
		while(getline(Rread,line))
		{
			Rkey = line.substr(0,line.find(", "));
			hashKey = hash2(Rkey);
			if(hashBucketsPointer[hashKey] == blockSize)
			{
				for(int j=0; j<M-2; j++)
                {
                    if (hashBucketsPointer[hashKey + 1 + j] != blockSize)
                    {
                        
                        if(probbing[hashKey][j] == -1)
                        {
                            probbing[hashKey][j] = hashBucketsPointer[hashKey+1+j];
                        }
                        hashKey = hashKey + 1 + j;
                        break;
                    }
                }
			}
			hashBuckets[hashKey][hashBucketsPointer[hashKey]] = line;
			hashBucketsPointer[hashKey] += 1;
		}
		Rread.close();
		Rread.open(S+"File"+to_string(i)+".txt");
		while(getline(Rread,line))
		{
			Skey = line.substr(0,line.find(", "));
			hashKey = hash2(Skey);
			for(int k=0; k<hashBucketsPointer[hashKey]; k++)
			{
				Rkey = hashBuckets[hashKey][k].substr(0,hashBuckets[hashKey][k].find(", "));
				if(Rkey.size() == Skey.size() && Rkey == Skey)
				{
					if(outputBufferPointer == blockSize)
					{
						for(int l=0; l<blockSize; l++)
						{
							Rout<<outputBuffer[l]<<endl;
						}
						outputBufferPointer = 0;
					}
					if(R == "R")
						outputBuffer[outputBufferPointer] = hashBuckets[hashKey][k] + ", "+line;
					else
						outputBuffer[outputBufferPointer] = line + ", " + hashBuckets[hashKey][k];
					outputBufferPointer += 1;
                    cout<<"\nRkey: "<<Rkey<<" Skey: "<<Skey;
				}
			}
            for(int l=0; l<M-2; l++)
            {
                if(probbing[hashKey][l]!=-1)
                {
                    int newKey = (hashKey+1+l)%(M-1);
                    for(int k=probbing[hashKey][l]; k<hashBucketsPointer[newKey]; k++)
                    {
                        Rkey = hashBuckets[newKey][k].substr(0,hashBuckets[newKey][k].find(", "));
                        if(Rkey.size() == Skey.size() && Rkey == Skey)
				        {
					        if(outputBufferPointer == blockSize)
					        {
						        for(int m=0; m<blockSize; m++)
						        {
							        Rout<<outputBuffer[m]<<endl;
						        }
						        outputBufferPointer = 0;
					        }
					        if(R == "R")
						        outputBuffer[outputBufferPointer] = hashBuckets[newKey][k] + ", "+line;
					        else
						        outputBuffer[outputBufferPointer] = line + ", " + hashBuckets[newKey][k];
					        outputBufferPointer += 1;
                            //cout<<"\nRkey: "<<Rkey<<" Skey: "<<Skey;
				        }
                    }
                }
            }
		}
		Rread.close();
	}
	for(int l=0; l<outputBufferPointer; l++)
		Rout<<outputBuffer[l]<<endl;
	Rout.close();
}

void closeHash()
{
	for(int i=0; i<M; i++)
	{
		remove(("RFile"+to_string(i)+".txt").c_str());
		remove(("SFile"+to_string(i)+".txt").c_str());
	}
}

int hash1(string line)
{
	int value = 0;
	for(int i=0; i<line.size(); i++)
	{
		value += (int)line[i];
	}
	return (value) % M;
}

int* createHashedSublists(string path, string R, int *hashBucketSize)
{
	ifstream Rread(path);
	ofstream Rout[M];
	string inputBuffer[blockSize];
	int inputBufferPointer = 0;
	string hashBuckets[M][100];
	int hashBucketsPointer[M];
	string line = "";
	string key = "";
	int hashKey = 0;

	for(int i=0; i<M; i++)
	{
		Rout[i].open(R+"File"+to_string(i)+".txt");
		hashBucketsPointer[i] = 0;
		hashBucketSize[i] = 0;
	}

	while(getline(Rread, line))
	{
		for(int i=0; i<M; i++)
		{
			if(hashBucketsPointer[i] == blockSize)
			{
				for(int j=0; j<blockSize; j++)
				{
					Rout[i]<<hashBuckets[i][j]<<endl;
				}
				hashBucketsPointer[i] = 0;
			}
		}

		key = line.substr(0,line.find(", "));
		hashKey = hash1(key);
		hashBuckets[hashKey][hashBucketsPointer[hashKey]] = line;
		hashBucketsPointer[hashKey] += 1;
		hashBucketSize[hashKey] += 1;
	}
	Rread.close();
	for(int i=0; i<M; i++)
	{
		for(int j=0; j<hashBucketsPointer[i]; j++)
		{
			Rout[i]<<hashBuckets[i][j]<<endl;
		}
		Rout[i].close();
	}
    for(int i=0; i<M; i++)
    {
        if(hashBucketSize[i] > (M-1)*blockSize)
        {
            closeHash();
            cout<<"\nErr: Use better Hash1 function";
            exit(0);
        }
    }
	return hashBucketSize;
}

void openHash()
{
	int RhashBucketSize[M];
	int ShashBucketSize[M];
	createHashedSublists(pathToR, "R", RhashBucketSize);
	createHashedSublists(pathToS, "S", ShashBucketSize);
	getNextHash(RhashBucketSize, ShashBucketSize);
	closeHash();
}

void createSortedSublists(string path, string R)
{
	ifstream Rread(path);
	string line1 = "";
	string line = ""; //temporary line read from file
	string memoryBuffer[M][blockSize];//memory buffer that stores original data in blocks
	string outputBuffer[blockSize];//output buffer - size one block
	int outputBufferPointer = 0;//To mkeep track of how full the output buffer is
	int memoryPointer[M];//To keep track of how full each buffer is
	int fileCounter = 0;//to keep track of which file I'm writing to
	int bufferPointer = 0;//to keep track which buffer is to be filled
	string key = "";//key currently read
	string minKey = "";//minKey read so far
	string minLine = "";//record corresponding to minKey
	int min;//buffer corresponding to having the minKey
	for(int i =0; i<M; i++)
		memoryPointer[i] = 0;//Initiallising all buffer position pointers to 0
	int mergePointer[M];//to track position of pointers during the merge phase
	while(getline(Rread, line))//while there exists records
	{
		int bflag = 0;
		//cout<<"\nFrom "<<path<<" read: "<<line;
		line1 = line;
		if(memoryPointer[M-1] == blockSize)//If last buffer block in memory if full
		{
			bflag = 1;
			//cout<<"\nMemory full: 2nd phase of sorting sublists";
			ofstream Rout(R+"File"+to_string(fileCounter)+".txt");//Open new file
			//cout<<"\nCreated new file: "<<R+"File"+to_string(fileCounter)+".txt";
			fileCounter += 1;
			for(int i=0; i<M; i++)//Initialising merge pointers to 0
				mergePointer[i] = 0;
			int flag = 0;//While I haven't gone through all the records read so far
			//cout<<"\nSorting all sublists";
			for (int i =0; i<M; i++)
			{
				sort(memoryBuffer[i],memoryBuffer[i]+memoryPointer[i]);
			}
			while(!flag)
			{
				flag = 1;
				minKey = "";
				for(int i=0; i<M; i++)
				{
					if(mergePointer[i] < memoryPointer[i])
					{
						flag = 0;
						line = memoryBuffer[i][mergePointer[i]];
						key = line.substr(0,line.find(", "));
						if(minKey == "" || minKey > key)
						{
							min = i;
							minKey = key;
							minLine = line;
						}
					}
				}
				if(flag)
					break;
				//cout<<"\nMinKey: "<<minKey<<" MinLine: "<<minLine;
				mergePointer[min] += 1;
				outputBuffer[outputBufferPointer] = minLine;
				outputBufferPointer += 1;
				if(outputBufferPointer == blockSize)
				{
					//cout<<"\nOutput Buffer Full, writing to output file";
					outputBufferPointer = 0;
					for(int i=0; i<blockSize; i++)
					{
						Rout<<outputBuffer[i]<<"\n";
					}
					
				}
			}
			//Write remaining tuples into file
			//cout<<"\nWriting left over tuples from outputbuffer to file";
			for(int i=0; i<outputBufferPointer; i++)
			{
				Rout<<outputBuffer[i]<<endl;
			}
			outputBufferPointer = 0;
			bufferPointer = 0;
			for(int i=0; i<M; i++)
				memoryPointer[i] = 0;
			Rout.close();
		}
		else
		{
			//cout<<"\nWriting: "<<line<<" to buffer: "<<bufferPointer<<" at position: "<<memoryPointer[bufferPointer];
			memoryBuffer[bufferPointer][memoryPointer[bufferPointer]] = line;
			memoryPointer[bufferPointer] += 1;
			bufferPointer += 1;
			//cout<<bufferPointer;
			if(bufferPointer == M)
				bufferPointer = 0;
			//cout<<bufferPointer;
		}
		if(bflag)
		{
			//cout<<"\nWriting: "<<line1<<" to buffer: "<<bufferPointer<<" at position: "<<memoryPointer[bufferPointer];
			memoryBuffer[bufferPointer][memoryPointer[bufferPointer]] = line1;
			memoryPointer[bufferPointer] += 1;
			bufferPointer += 1;
			//cout<<bufferPointer;
			if(bufferPointer == M)
				bufferPointer = 0;
			//cout<<bufferPointer;
		}
	}
	Rread.close();
	if(memoryPointer[0] == 0)
	{
		return;
	}
	
	ofstream Rout(R+"File"+to_string(fileCounter)+".txt");
	cout<<"\nCreated new file: "<<R+"File"+to_string(fileCounter)+".txt";
	for(int i=0; i<M; i++)
		mergePointer[i] = 0;
	int flag = 0;
	cout<<"\nSorting all sublists";
	for (int i =0; i<M; i++)
	{
		sort(memoryBuffer[i],memoryBuffer[i]+memoryPointer[i]);
	}
	// for(int i=0; i<M; i++)
	// {
	// 	for(int j=0; j<memoryPointer[i]; j++)
	// 		cout<<"\nBuffer: "<<i<<" Slot: "<<j<<" line: "<<memoryBuffer[i][j];
	// }
	while(!flag)
	{
		flag = 1;
		minKey = "";
		for(int i=0; i<M; i++)
		{
			if(mergePointer[i] < memoryPointer[i])
			{
				flag = 0;
				line = memoryBuffer[i][mergePointer[i]];
				key = line.substr(0,line.find(", "));
				if(minKey == "" || minKey > key)
				{
					min = i;
					minKey = key;
					minLine = line;
				}
			}
		}
		if(flag)
			break;
		cout<<"\nMinKey: "<<minKey<<" MinLine: "<<minLine;
		mergePointer[min] += 1;
		outputBuffer[outputBufferPointer] = minLine;
		outputBufferPointer += 1;
		if(outputBufferPointer == blockSize)
		{
			cout<<"\nOutput Buffer Full, writing to output file";
			outputBufferPointer = 0;
			for(int i=0; i<blockSize; i++)
			{
				Rout<<outputBuffer[i]<<"\n";
			}
					
		}
	}
	//Write remaining tuples into file
	cout<<"\nWriting left over tuples from outputbuffer to file";
	for(int i=0; i<outputBufferPointer; i++)
	{
		Rout<<outputBuffer[i]<<"\n";
	}
	Rout.close();	
}

void closeSort()
{
	int x;
	for(int i=0; i<Rnum; i++)
		x = remove(("RFile"+to_string(i)+".txt").c_str());
	for(int i=0; i<Snum; i++)
		x = remove(("SFile"+to_string(i)+".txt").c_str());
}

void getNextSort()
{
	int n = Rnum + Snum;
	ifstream Rread[n];
	ofstream Rout(outputFile);
	string memoryBuffer[n][blockSize];
	int memoryPointer[n];
	string outputBuffer[blockSize];
	int outputBufferPointer = 0;
	int mergePointer[n];
	string minKey = "";
	string minLine = "";
	int min = 0;
	string key = "";
	string line = "";
	string copyBlock[blockSize];
	int copyBlockPointer = 0;
	string prevMinKey = "";
	for(int i=0; i<n; i++)
	{
		mergePointer[i] = blockSize;
		memoryPointer[i] = blockSize;
	}
	//Opening all the files
	string R = "R";
	for(int i=0; i<Rnum; i++)
		Rread[i].open(R+"File"+to_string(i)+".txt");
	R = "S";
	for(int i=0; i<Snum; i++)
		Rread[i+Rnum].open(R+"File"+to_string(i)+".txt");
	while(1)
	{
		for(int i=0; i<n; i++)
		{
			if(mergePointer[i]==memoryPointer[i])
			{
				memoryPointer[i] = 0;
				mergePointer[i] = 0;
				while(memoryPointer[i] < blockSize)
				{
                    if(getline(Rread[i],line))
                    {
                        cout<<"\nWriting : "<<line<<" to buffer: "<<i<<" at position: "<<memoryPointer[i];
					    memoryBuffer[i][memoryPointer[i]] = line;
					    memoryPointer[i] += 1;
                    }
                    else
                        break;
				}
			}
		}
		//find min amongst R buffers
		minKey = "";
		for(int i=0; i<Rnum; i++)
		{
			if(mergePointer[i] < memoryPointer[i])
			{
				line = memoryBuffer[i][mergePointer[i]];
				key = line.substr(0,line.find(", "));
				if(minKey == "" || minKey > key)
				{
					minKey = key;
					minLine = line;
					min = i;
				}
			}
		}
		cout<<"\nMinKey: "<<minKey<<" minLine: "<<minLine;
		if(minKey == "")
			break;
		mergePointer[min] += 1;
		if(prevMinKey == minKey)
		{
			for(int j=0; j<copyBlockPointer; j++)
			{
				cout<<"\nCopy Matched";
				cout<<"\nWriting to output buffer: "<<minLine + ", "+ copyBlock[j];
				outputBuffer[outputBufferPointer] = minLine + ", "+ copyBlock[j];
				outputBufferPointer += 1;
				if(outputBufferPointer == blockSize)
				{
					for(int k=0; k<outputBufferPointer; k++)
					{
						Rout<<outputBuffer[k]<<endl;
					}
					outputBufferPointer = 0;
				}
			}
		}
		else
		{
			copyBlockPointer = 0;
			for(int i=Rnum; i<n; i++)
			{
				for(int j=mergePointer[i]; j<memoryPointer[i]; j++)
				{
					line = memoryBuffer[i][j];
					cout<<"\nReading: "<<line;
					key = line.substr(0,line.find(", "));
					if(key < minKey)
					{
						cout<<"\nSkipped Key: "<<key;
						mergePointer[i] += 1;
						if(mergePointer[i] == memoryPointer[i])
						{
							memoryPointer[i] = 0;
							mergePointer[i] = 0;
							while(memoryPointer[i] < blockSize)
							{
								if(getline(Rread[i],line))
								{
									cout<<"\nWriting : "<<line<<" to buffer: "<<i<<" at position: "<<memoryPointer[i];
									memoryBuffer[i][memoryPointer[i]] = line;
									memoryPointer[i] += 1;
								}
								else
									break;
							}
							j = -1;
						}
					}
					else if(key > minKey)
					{
						break;
					}
					else
					{
						cout<<"\nMatched";
						cout<<"\nWriting to output buffer: "<<minLine + ", "+ line;
						outputBuffer[outputBufferPointer] = minLine + ", "+ line;
						outputBufferPointer += 1;
						copyBlock[copyBlockPointer] = line;
						copyBlockPointer += 1;
						if(outputBufferPointer == blockSize)
						{
							for(int k=0; k<outputBufferPointer; k++)
							{
								Rout<<outputBuffer[k]<<endl;
							}
							outputBufferPointer = 0;
						}
						mergePointer[i] += 1;
						if(mergePointer[i] == memoryPointer[i])
						{
							memoryPointer[i] = 0;
							mergePointer[i] = 0;
							while(memoryPointer[i] < blockSize)
							{
								if(getline(Rread[i],line))
								{
									cout<<"\nWriting : "<<line<<" to buffer: "<<i<<" at position: "<<memoryPointer[i];
									memoryBuffer[i][memoryPointer[i]] = line;
									memoryPointer[i] += 1;
								}
								else
									break;
							}
							j = -1;
						}
					}
				}
			}
		}
		prevMinKey = minKey;
	}
    cout<<"\nCopying Left Overs from the Output Buffer";
	if(outputBufferPointer != 0)
	{
		for(int k=0; k<outputBufferPointer; k++)
		{
			Rout<<outputBuffer[k]<<endl;
		}	
	}
	Rout.close();
	for(int i=0; i<n; i++)
		Rread[i].close();
}
void openSort()
{
	//Function that creates sorted sublists and stores them in temporary files
	Rnum = ceil((RRecords / float(M*blockSize)));//Number of sorted sublists of R
	Snum = ceil((SRecords / float(M*blockSize)));//Number of sorted sublists of S
	//sort R
	createSortedSublists(pathToR, "R");
	createSortedSublists(pathToS, "S");
	cout<<"\nNumber of Temporary R files: "<<Rnum;
	cout<<"\nNumber of Temporary S files: "<<Snum;
	getNextSort();
	closeSort();
}

int checkMemory()
{
	ifstream Rin(pathToR);
	ifstream Sin(pathToS);
	string line;
	while(getline(Rin,line))
	{
		RRecords++;
		Rsize = line.length();
	}
	while(getline(Sin,line))
	{
		SRecords++;
		Ssize = line.length();
	}
	Rin.close();
	Sin.close();
	// cout<<RRecords<<"\n";
	// cout<<SRecords<<"\n";
	if(Method)//Sorting
	{
		if((RRecords + SRecords)/blockSize > M*M)
			return 0;
	}
	else
	{
		if(RRecords < SRecords)
		{
			if(RRecords/blockSize > M*M)
				return 0;
		}
		else
		{
			if (SRecords/blockSize > M*M)
				return 0;
		}
	}
	return 1;
}

int main(int argc, char *argv[])
{
	pathToR = argv[1];
	pathToS = argv[2];
	size_t pos = 0;
	string token;
	string delimiter = "/";
	while ((pos = pathToR.find(delimiter)) != std::string::npos)
       	{
    	token = pathToR.substr(0, pos);
    	pathToR.erase(0, pos + delimiter.length());
	}
	RFile = pathToR.substr(0, pathToR.find("."));
	pathToR = argv[1];
	pos = 0;
	while ((pos = pathToS.find(delimiter)) != std::string::npos)
    {
    	token = pathToS.substr(0, pos);
    	pathToS.erase(0, pos + delimiter.length());
	}
	SFile = pathToS.substr(0, pathToS.find("."));
	pathToS = argv[2];
	outputFile = RFile+"_"+SFile+"_"+argv[3]+"join.txt";
	M = atoi(argv[4]);
	if(M<2)
	{
		cout<<"\nMinimum number of buffers: 2";
		return 0;
	}
	if(!strcmp(argv[3],"sort"))
		Method = 1;
	if(!checkMemory())
	{
		cout<<"\nErr: Insufficient Memory: ABORTING\n";
		cout<<"\n";
		return 0;
	}
	else
	{
		auto start = std::chrono::high_resolution_clock::now();
		if(Method)
			openSort();
		else
			openHash();
		auto elapsed = std::chrono::high_resolution_clock::now() - start;
		long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
		cout<<"\nTime Taken in microseconds: "<<microseconds;
	}
	return 0;
}
