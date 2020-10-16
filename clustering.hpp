#ifndef CLUSTERING_H
#define CLUSTERING_H

#include<time.h>
#include<fstream>
#include<iostream>
#include<vector>

using namespace std;

class Clustering
{
private:
    vector<vector<double>> data;
public:
    Clustering(vector<vector<double>> &);
    ~Clustering();
    vector<int> loyds(int k);
};

Clustering::Clustering(vector<vector<double>> &data_vector)
{
    this->data = data_vector;
}

//Return a vector with the cluster that each image belongs
vector<int> Clustering::loyds(int k){
    Metrics metrics = Metrics();
    vector<int>clusters;
    for (int i = 0; i < this->data.size(); i++)
    {
        clusters.push_back(-1);  //Stin arxi oi eikones den anikoun se kanena cluster
    }
    //Initialize the centroids
    vector<vector<double>> centroids;
    centroids.resize(k,vector<double>(this->data[0].size()));
    //Choose k random centers from the dataset
    srand(time(NULL));
    for (int i = 0; i < k; i++)
    {
        int img_number = rand() % (this->data.size()-1) + 0;
        for (int j = 0; j < this->data[0].size(); j++)
        {
            centroids[i][j] = this->data[img_number][j];
        }
    }
    //Assign the data to the clusters
    for (int i = 0; i < this->data.size(); i++)
    {
        //Compute the distance from each cluster
        vector<pair<int,int>>distances; //A pair with cluster number and distance
        for (int j = 0; j < k; j++)
        {
            int manhattan_dist = metrics.get_distance(this->data[i],centroids[j],(char *)"L1");
            distances.push_back(make_pair(j,manhattan_dist));
        }
        //Sort distances vector
        sort(distances.begin(),distances.end(),sortbysec);
        //Assign the closest cluster to the image
        clusters[i] = distances[0].first;
        //Prepi na kanoume update ta centroids me ti xrisi median dianismatos
    }
    
    return clusters;
}

Clustering::~Clustering()
{
}



#endif