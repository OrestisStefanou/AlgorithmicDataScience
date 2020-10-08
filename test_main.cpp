#include"reader.hpp"

using namespace std;
 
int main()
{
    vector<vector<double>> data;
    ReadData(data,(char *)"/home/orestis/EKPA/7examino/AlgorithmicDataScience/Project1/train-images-idx3-ubyte");
    for (int i = 100; i < 200; i++)
    {
        cout << data[1000][i] << endl;    
    }
    
    return 0;
}