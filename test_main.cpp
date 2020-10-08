#include"reader.hpp"
#include"metrics.hpp"

using namespace std;
 
int main()
{
    vector<vector<double>> data;
    ReadData(data,(char *)"/home/orestis/EKPA/7examino/AlgorithmicDataScience/Project1/train-images-idx3-ubyte");
    
    Metrics metrics = Metrics();
    cout << "Manhattan distance between first and second image is:" ;
    cout << metrics.manhattan_dist(data[0],data[1]) << endl;
    return 0;
}