//Input Generator
#include<iostream>
#include<fstream>
#include<cstdlib>
#include<ctime>

using namespace std;

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        cout<<"\nErr: Usage: ./generate <inputFileName> <numberOfRecords>";
        return 0;
    }
    srand(time(0));
    int N = stoi(argv[2]);
    ofstream Rout(argv[1]);
    for(int i=0; i<N; i++)
    {
        Rout<<(rand()%N)<<", "<<rand()<<endl;
    }
    Rout.close();
    return 0;
}