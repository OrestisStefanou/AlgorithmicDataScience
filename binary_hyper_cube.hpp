#ifndef BINARY_HYPER_CUBE_H
#define BINARY_HYPER_CUBE_H
#include<iostream>
#include"hashtable.hpp"
#include"metrics.hpp"
#include<time.h>
#include<math.h>

using namespace std;

class BinaryHyperCube
{
private:
    vector<vector<double>> data;    //image training  dataset
    vector<vector<int>> hyper_cube;
    vector<vector<int>> s_vectors;
    int d;  //d' apo diafanies
    int M;
    int probes;
    int R;

public:
    BinaryHyperCube(vector<vector<double>> &data_vector,int k,int M,int probes,int R);
    ~BinaryHyperCube();
    int f(int );
    int h(vector<double> &,int );
};

BinaryHyperCube::BinaryHyperCube(vector<vector<double>> &data_vector,int k,int M,int probes,int R)
{
    this->data = data_vector;
    this->d = k;
    this->M = M;
    this->probes = probes;
    this->R = R;

    //Resize s_vectors
    this->s_vectors.resize(this->d,vector<int>(data_vector[0].size()));

    //Initialize s_vectors
    for (int i = 0; i < k; i++)
    {
        srand(i);
        for (int j = 0; j < data_vector[0].size(); j++)
        {
            this->s_vectors[i][j] = rand() % (40000-1) + 0;     //To 40000 jame kapos prepi na en metavliti kalitera
        }
    }

    //Initialize the hypercube
    this->hyper_cube.resize(pow(2,this->d),vector<int>(0));
    
    //Create a vector to store f function results
    vector<int> f_results;  //Contains only 0 and 1
    //Insert the data in the hypercube
    for (int i = 0; i < data_vector.size(); i++)
    {
        //Compute f results
        for (int j = 0; j < this->d; j++)
        {
            int h_result = this->h(data_vector[i],j);
            f_results.push_back(this->f(h_result));
        }
        //Na sindiasoume kapos ta 0 kai 1 pou exoume sto vector gia
        //na evroume to index sto hypercube(p.x f_results = [0,1,1]=>index sto hypercube = 3)
        int hyper_cube_index;
    }
    
}

int BinaryHyperCube::h(vector<double> &image,int index){
    vector<int> a;
    unsigned int hash_result;

    //Calculate a vector
    for (int i = 0; i < this->s_vectors[index].size(); i++)
    {
        double temp_a = double(image[i]-s_vectors[index][i])/double(40000);
        a.push_back(int(round(temp_a)));  
    }
    //Calculate h(image)
    int m = pow (2,32-3);     
    int M = pow (2, 32/4);      //to 4(=k) kalitera na en kapou san metavliti
    hash_result = a[a.size()-1] % M;
    for (int d = a.size()-2; d >= 0; d--)
    {
        hash_result+=(a[d] * m) % M;
        m = m*m;
    }
    hash_result = hash % M;
    return hash_result;
}

//Kapos prepi na metatrepi ena hash result se 0 i 1
int BinaryHyperCube::f(int hash_result){
    return 0;
}

BinaryHyperCube::~BinaryHyperCube()
{
}


#endif