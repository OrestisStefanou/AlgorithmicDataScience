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
    vector<vector<int>> loyds(int k);
};

Clustering::Clustering(vector<vector<double>> &data_vector)
{
    this->data = data_vector;
}

//Return a vector with the cluster that each image belongs
vector<vector<int>> Clustering::loyds(int k){
    Metrics metrics = Metrics();
    vector<vector<int>>clusters;    //clusters[0]->vector with image numbers that belong to cluster 0
    //Initialize the clusters
    clusters.resize(k,vector<int>(0));

    //Initialize the centroids
    vector<vector<double>> centroids;
    centroids.resize(k,vector<double>(this->data[0].size()));
    //Initialize the new centroids vector
    vector<vector<double>>new_centroids;
    new_centroids.resize(k,vector<double>(this->data[0].size()));

    double centroids_difference = -10;

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
    
    while(1){
        if(centroids_difference < 1000.0 && centroids_difference >= 0.0){
            break;
        }
        centroids_difference = 0.0;
        //Initialize the clusters
        for (int i = 0; i < k; i++)
        {
            if(clusters[i].size()>0){
                clusters[i].clear();
            }
        }
        clusters.resize(k,vector<int>(0));
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
            clusters[distances[0].first].push_back(i);
        }
        
        //Update the centroids
        for (int i = 0; i < k; i++) //Number of centroids
        {
            for (int c = 0; c < this->data[clusters[i][0]].size(); c++) //Get each column from the images in the cluster
            {
                double sum = 0.0;
                for (int r = 0; r < clusters[i].size(); r++)    //Get each row from the images in the cluster
                {
                    sum += this->data[clusters[i][r]][c];       //Sum their data
                }
                //Calculate the mean 
                double mean = sum/clusters[i].size();
                //Update the centroid column
                new_centroids[i][c] = mean;
            }
        }
        
        //Find how different are the new_centroids from the old ones
        for (int i = 0; i < k; i++)
        {
            for (int j = 0; j < centroids[i].size(); j++)
            {
                centroids_difference += abs(new_centroids[i][j] - centroids[i][j]);
                centroids[i][j] = new_centroids[i][j];  //Update centroids to new_centroids for the next iteration
            }
        }
    }
    return clusters;
}

Clustering::~Clustering()
{
}



#endif