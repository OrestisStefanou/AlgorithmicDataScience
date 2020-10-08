#ifndef METRICS_H
#define METRICS_H
#include<vector>
#include <bits/stdc++.h> 
#include<string.h>

using namespace std;

//Class where we hold different kind of LSH-able metrics
class Metrics
{
private:

public:
    Metrics();
    ~Metrics();
    int get_distance(vector<double> a,vector<double> b,char *type);
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
int Metrics::get_distance(vector<double> a,vector<double> b,char *type){
    int distance = 0;

    if(strcmp((char *)"L1",type)==0){
        for (int i = 0; i < a.size(); i++)
        {
            distance += abs(double(a[i]) - double(b[i]));
        }
        return distance;
    }
    return distance;
}

#endif