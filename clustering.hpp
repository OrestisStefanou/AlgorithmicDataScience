#ifndef CLUSTERING_H
#define CLUSTERING_H

#include <time.h>
#include <fstream>
#include <iostream>
#include <vector>
#include "metrics.hpp"
#include "lsh.hpp"

#define R 10000
#define YES 1
#define NO 0
#define FLAG 0
#define CLUSTER 1
using namespace std;

class Clustering
{
private:
    vector<vector<double>> data;

public:
    Clustering(vector<vector<double>> &);
    ~Clustering();
    vector<vector<int>> loyds(int k);
    vector<vector<int>> lsh(int k, int L, int KforLSH);
};

Clustering::Clustering(vector<vector<double>> &data_vector)
{
    this->data = data_vector;
}

//Return a vector with the cluster that each image belongs
vector<vector<int>> Clustering::loyds(int k)
{
    Metrics metrics = Metrics();
    vector<vector<int>> clusters; //clusters[0]->vector with image numbers that belong to cluster 0
    //Initialize the clusters
    clusters.resize(k, vector<int>(0));

    //Initialize the centroids
    vector<vector<double>> centroids;
    centroids.resize(k, vector<double>(this->data[0].size()));
    //Initialize the new centroids vector
    vector<vector<double>> new_centroids;
    new_centroids.resize(k, vector<double>(this->data[0].size()));

    double centroids_difference = -10;

    //Choose k random centers from the dataset
    srand(time(NULL));
    for (int i = 0; i < k; i++)
    {
        int img_number = rand() % (this->data.size() - 1) + 0;
        for (int j = 0; j < this->data[0].size(); j++)
        {
            centroids[i][j] = this->data[img_number][j];
        }
    }

    while (1)
    {
        if (centroids_difference < 1000.0 && centroids_difference >= 0.0)
        {
            break;
        }
        centroids_difference = 0.0;
        //Initialize the clusters
        for (int i = 0; i < k; i++)
        {
            if (clusters[i].size() > 0)
            {
                clusters[i].clear();
            }
        }
        clusters.resize(k, vector<int>(0));
        //Assign the data to the clusters
        for (int i = 0; i < this->data.size(); i++)
        {
            //Compute the distance from each cluster
            vector<pair<int, int>> distances; //A pair with cluster number and distance
            for (int j = 0; j < k; j++)
            {
                int manhattan_dist = metrics.get_distance(this->data[i], centroids[j], (char *)"L1");
                distances.push_back(make_pair(j, manhattan_dist));
            }
            //Sort distances vector
            sort(distances.begin(), distances.end(), sortbysec);
            //Assign the closest cluster to the image
            clusters[distances[0].first].push_back(i);
        }

        //Update the centroids
        for (int i = 0; i < k; i++) //Number of centroids
        {
            for (int c = 0; c < this->data[clusters[i][0]].size(); c++) //Get each column from the images in the cluster
            {
                double sum = 0.0;
                for (int r = 0; r < clusters[i].size(); r++) //Get each row from the images in the cluster
                {
                    sum += this->data[clusters[i][r]][c]; //Sum their data
                }
                //Calculate the mean
                double mean = sum / clusters[i].size();
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
                centroids[i][j] = new_centroids[i][j]; //Update centroids to new_centroids for the next iteration
            }
        }
    }
    return clusters;
}
vector<vector<int>> Clustering::lsh(int k, int L, int KforLSH)
{
    Metrics metrics = Metrics();
    vector<vector<int>> clusters; //clusters[0]->vector with image numbers that belong to cluster 0
    //Initialize the clusters
    clusters.resize(k, vector<int>(0));

    //Initialize the centroids
    vector<vector<double>> centroids;
    centroids.resize(k, vector<double>(this->data[0].size()));
    //Initialize the new centroids vector
    vector<vector<double>> new_centroids;
    new_centroids.resize(k, vector<double>(this->data[0].size()));

    double centroids_difference = -10;

    //Choose k random centers from the dataset
    srand(time(NULL));
    for (int i = 0; i < k; i++)
    {
        int img_number = rand() % (this->data.size() - 1) + 0;
        for (int j = 0; j < this->data[0].size(); j++)
        {
            centroids[i][j] = this->data[img_number][j];
        }
    }
    //Initialize the clusters
    for (int i = 0; i < k; i++)
    {
        if (clusters[i].size() > 0)
        {
            clusters[i].clear();
        }
    }

    //Mark assigned points
    //en enas pinakas pou krata ena flag an i ikona empike se kapio cluster i oi.
    //Ypaxri periptosi 1 ikona na pezete se 2(i je pio polla cluster) na mpi opote emis prepi
    // na kseroume an idi aniki se ena opote na piasoume se jino pou eshi tin pio mikri apostasi(me manhatan apotastasi)

    //Initialize vector with zeros
    //O pinakas exi diastasis data.size*2
    //Stin proti stili mpeni to flag(0 h 1) an exi xrisimopoithi i ikona kai stin deuteri stili mpeni o arithmos tou cluster opou aniki
    vector<vector<int>> AssignedPoints(2, vector<int>(this->data.size(), 0));

    //LSH Initialization
    LSH lsh = LSH(KforLSH, L, this->data, R);

    int r=R;
    for (int m = 0; m < 10; m++)
    {

        //Assign the data to the clusters
        for (int i = 0; i < k; i++)
        {

            //Do the range search
            vector<int> range_results = lsh.range_search(centroids[i], i, r);

            //gemizoume to cluster me ta kontina simia pou mas edose to range search
            for (int j = 0; j < range_results.size(); j++)
            {

                //an to simio den aniki se kapoio cluster tote to bazoume sto cluster
                if (AssignedPoints[range_results[j]][FLAG] == NO)
                {
                    AssignedPoints[range_results[j]][FLAG] = 1;
                    AssignedPoints[range_results[j]][CLUSTER] = i;
                    clusters[i].push_back(range_results[j]);
                }
                //an to simio ani se kapio cluster epilegoume to kontinotero cluster
                else
                {
                    vector<pair<int, int>> distances; //A pair with cluster number and distance
                    int manhattan_dist;

                    //ipologizi tin apostasi apo to paron cluster
                    manhattan_dist = metrics.get_distance(this->data[range_results[j]], centroids[i], (char *)"L1");
                    distances.push_back(make_pair(i, manhattan_dist));

                    //ipologizi tin apostasi apo to cluster opou aniki
                    manhattan_dist = metrics.get_distance(this->data[range_results[j]], centroids[AssignedPoints[range_results[j]][CLUSTER]], (char *)"L1");
                    distances.push_back(make_pair(AssignedPoints[range_results[j]][CLUSTER], manhattan_dist));

                    //Sort distances vector
                    sort(distances.begin(), distances.end(), sortbysec);
                    //Assign the closest cluster to the image
                    //An i ikona mpeni sto paron cluster fie tin pou to palio cluster je varti sto jenourko alios afistin jiame pou eni
                    if (distances[0].first == j)
                    {
                        //remove image from old cluster
                        clusters[AssignedPoints[range_results[j]][CLUSTER]].erase(remove(clusters[AssignedPoints[range_results[j]][CLUSTER]].begin(), clusters[AssignedPoints[range_results[j]][CLUSTER]].end(), i), clusters[AssignedPoints[range_results[j]][CLUSTER]].end());

                        clusters[distances[0].first].push_back(range_results[j]);
                        AssignedPoints[range_results[j]][CLUSTER] = i;
                    }
                }
            }
        }

        return clusters;
        r=r*2;
    }
}
Clustering::~Clustering()
{
}

#endif