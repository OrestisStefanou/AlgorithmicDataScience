#include"reader.hpp"
#include"metrics.hpp"
#include"hashtable.hpp"

using namespace std;
 
int main()
{
    vector<vector<double>> data;
    ReadData(data,(char *)"/home/orestis/EKPA/7examino/AlgorithmicDataScience/Project1/train-images-idx3-ubyte");
    
    Metrics metrics = Metrics();
    cout << "Manhattan distance between first and second image is:" ;
    cout << metrics.get_distance(data[0],data[1],(char *)"L1") << endl;

    Hashtable table = Hashtable(10,1);
    for (int i = 0; i < 20; i++)
    {
        table.insert(data[i],i);
    }
    table.print();
    return 0;
}