#ifndef METRICS_H
#define METRICS_H
#include<vector>
#include <bits/stdc++.h> 

using namespace std;

//Class where we hold different kind of LSH-able metrics
class Metrics
{
private:

public:
    Metrics();
    ~Metrics();
    int manhattan_dist(vector<double> a,vector<double> b);
};

Metrics::Metrics()
{
}

Metrics::~Metrics()
{
}

//Returns the manhattan distance between vector a and vector b
int Metrics::manhattan_dist(vector<double> a,vector<double> b){
    int distance = 0;
    for (int i = 0; i < a.size(); i++)
    {
        distance += abs(double(a[i]) - double(b[i]));
    }
    return distance;
}

#endif