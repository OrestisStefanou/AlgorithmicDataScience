#include"reader.hpp"
#include"metrics.hpp"
#include"hashtable.hpp"
#include"lsh.hpp"

using namespace std;
 
int main()
{
    vector<vector<double>> data;
    ReadData(data,(char *)"/home/orestis/EKPA/7examino/AlgorithmicDataScience/Project1/train-images-idx3-ubyte");
    
    Metrics metrics = Metrics();
    //cout << "Manhattan distance between first and second image is:" ;
    //cout << metrics.get_distance(data[0],data[1],(char *)"L1") << endl;

    Hashtable table = Hashtable(10,1);
    for (int i = 0; i < 20; i++)
    {
        table.insert(data[i],i);
    }
    //table.print();

    LSH lsh = LSH(10,10,data);
    cout << lsh.nearest_neighbor(data[100],100) << endl;

    vector<double> test_img;
    for (int i = 0; i < 784; i++)
    {
        test_img.push_back(20);
    }
    cout << lsh.nearest_neighbor(test_img,600001) << endl;
    cout << "Testing KNN" << endl;
    vector<int> results = lsh.knn(data[100],100,10);
    for (int i = 0; i < results.size(); i++)
    {
        cout << results[i] << endl;
    }
    
    return 0;
}