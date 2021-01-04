#ifndef METRICS_H
#define METRICS_H
#include <vector>
#include <bits/stdc++.h>
#include <string.h>
#include "EMD.hpp"
#include <math.h>

using namespace std;

// Driver function to sort the vector elements
// by second element of pairs
bool sortbysec(const pair<int, int> &a, const pair<int, int> &b)
{
    return (a.second < b.second);
}

//Class where we hold different kind of LSH-able metrics
class Metrics
{
private:
public:
    Metrics();
    ~Metrics();
    int get_distance(vector<double> a, vector<double> b, char *type);
    //Add more distance metrics here
};

Metrics::Metrics()
{
}

Metrics::~Metrics()
{
}

//Returns the "type" distance between vector a and vector b
//type = "L1"->returns Manhatan distance
//type = "EMD"->returns Earth mover's distance
int Metrics::get_distance(vector<double> a, vector<double> b, char *type)
{
    int distance = 0;

    if (a.size() != b.size())
    {
        cout << "Size of two vectors is not the same" << endl;
        return -1;
    }
    //Manhattan distance
    if (strcmp((char *)"L1", type) == 0)
    {
        for (int i = 0; i < a.size(); i++)
        {
            distance += abs(double(a[i]) - double(b[i]));
        }
        return distance;
    }
    //Earth mover's distance
    if (strcmp((char *)"EMD", type) == 0)
    {
        vector<double> aw;
        vector<double> bw;
        //Initialize weights(We give the same weight to each pixel)
        for (int i = 0; i < a.size(); i++)
        {
            aw.push_back(10);
            bw.push_back(10);
        }
        distance = emd(a, aw, b, bw);
        return distance;
    }
    return distance;
}

//Creates a new dataset in imgsCluster vector with a cluster of size dim for every image
void getImgsCluster(vector<vector<double>> &images, int dim, vector<vector<double>> &Centroids, vector<vector<double>> &Weights)
{
    Centroids.resize(images.size(), vector<double>(dim * dim)); //Resize the vector
    Weights.resize(images.size(), vector<double>(dim * dim));   //Resize the vector
    int originalDim = int(sqrt(images[0].size()));
    int newImageDim = int(sqrt(dim));
    int PixelsPerCluster = originalDim / newImageDim;
    int centroid = ((PixelsPerCluster ^ 2) / 2) + 1;
    for (int i = 0; i < images.size(); i++)
    {
        int clusterColIndex = 0;
        int clusterRowIndex = 0;
        int RowCounter = newImageDim - 1;
        for (int j = 0; j < dim; j++)
        {
            double ClusterSign = 0;
            int centroidCounter = 0;
            for (int k = clusterRowIndex; k < PixelsPerCluster + clusterRowIndex; k++)
            {
                for (int m = clusterColIndex; m < PixelsPerCluster + clusterColIndex; m++)
                {
                    ClusterSign = ClusterSign + images[i][(PixelsPerCluster * k) + m];
                    if (centroidCounter == centroid)
                    {
                        Centroids[i][j] = images[i][(PixelsPerCluster * k) + m];
                    }
                }
            }
            if (RowCounter == 0)
            {
                RowCounter = newImageDim - 1;
                clusterRowIndex = clusterRowIndex + PixelsPerCluster;
                clusterColIndex = 0;
            }
            else
            {
                clusterColIndex = clusterColIndex + PixelsPerCluster;
                RowCounter--;
            }
            Weights[i][j] = ClusterSign;
        }
    }
}

//Returns a pair with number of correct and wrong predictions
pair<int, int> compareResults(vector<int> &trainLabels, vector<int> &testLabels, vector<pair<int, int>> &results, int queryIndex)
{
    int correct = 0;
    int wrong = 0;
    int queryLabel = testLabels[queryIndex];
    for (int i = 0; i < results.size(); i++)
    {
        if (trainLabels[results[i].first] == queryLabel)
        {
            correct++;
        }
        else
        {
            wrong++;
        }
    }
    //cout << correct << " correct neighbors found" << endl;
    //cout << wrong << " wrong neighbors found" << endl;
    return make_pair(correct, wrong);
}

#endif
