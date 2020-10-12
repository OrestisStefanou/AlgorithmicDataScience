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
    pair<int,int> temp_results;
    temp_results = lsh.nearest_neighbor(data[100],100);
    cout << "Appr nn of img 100 is " << temp_results.first << " with distance "<< temp_results.second << endl;

    vector<double> test_img;
    for (int i = 0; i < 784; i++)
    {
        test_img.push_back(20);
    }
    temp_results = lsh.nearest_neighbor(test_img,600001);
    cout << "Appr nn of test_img is " << temp_results.first << " with distance "<< temp_results.second << endl;
    cout << "Testing KNN" << endl;
    vector<pair<int,int>> results = lsh.knn(data[100],100,10);
    for (int i = 0; i < results.size(); i++)
    {
        cout << results[i].first << ":" << results[i].second << endl;
    }
    cout << "Testing range search" << endl;
    vector<int> range_results = lsh.range_search(data[100],100,10);
    //range_results = lsh.range_search(data[100],100,10);
    for (int i = 0; i < range_results.size(); i++)
    {
        cout << range_results[i] << endl;
    }
    cout << "Testing exact nearest neighbor" << endl;
    results = lsh.exact_nearest_neighbor(data[100],5);
    for (int i = 0; i < results.size(); i++)
    {
        cout << results[i].first << ":" << results[i].second << endl;
    }
    ///test
    return 0;
}