#include"reader.hpp"
#include"metrics.hpp"
#include"hashtable.hpp"
#include"lsh.hpp"
#include"clustering.hpp"

using namespace std;
 
int main()
{
    vector<vector<double>> data;
    ReadData(data,(char *)"/home/orestis/EKPA/7examino/AlgorithmicDataScience/Project1/train-images-idx3-ubyte");
    Clustering cluster = Clustering(data);
    
    pair<vector<vector<int>>,vector<vector<double>>> results = cluster.loyds(10); 
    vector<vector<double>> scores = cluster.silhouette_score(results.first,results.second);
    for (int i = 0; i < scores.size(); i++)
    {
        double sum = 0.0;
        for (int j = 0; j < scores[i].size(); j++)
        {
            sum+=scores[i][j];
        }
        cout << "Average score of " << i << " cluster is:" << sum/scores[i].size() << endl;
    }
}