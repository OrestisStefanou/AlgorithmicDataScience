#ifndef HASHTABLE_H
#define HASHTABLE_H
#include<vector>
#include<iostream>
#include<time.h>
#include<math.h>

using namespace std;

class Hashtable
{
private:
    vector<int> *hashtable;     //Hashtable me ta index ton ikonon.Pinakas me vectors(buckets)
    int hash_type;            //To int en tixeo,theloume kati pu tha kamni diaforetiki hashfunction gia kathe Hashtable
    int table_size;
    int K;
    int w;
    vector<vector<int>> s_vectors;          //To dianisma s apo diafania 19
public:
    Hashtable(int size,int hash_type,int k,int w,int r,int d);
    int hash_function(vector<double> &image,int testing);
    void insert(vector<double> &image,int image_index);
    vector<int> get_bucket_imgs(int bucket_index);
    void print();   //Test function
    ~Hashtable();
};

//To hash_type ine apla gia ton random seed generator
//To size ine to size tou Hashtable
//To k,w,r,d ine pu tis diafanies(sel.19)
Hashtable::Hashtable(int size,int hash_type,int k,int w,int r,int d)
{
    this->hashtable = new vector<int>[size];
    this->hash_type = hash_type;
    this->table_size = size;
    this->K = k;
    this->w = w;

    //Resize s_vectors
    this->s_vectors.resize(k,vector<int>(d));   //Resize the vector to fit the data

    for(int i=0;i<k;i++){
        srand (hash_type + i);
        //Create vector s_vectors
        for (int j = 0; j < d; j++)
        {
            this->s_vectors[i].push_back(rand() % (w-1) + 0);
        }
    }
    
}

//Mia tixea hash function.To int testing en xriazete apla en gia tora
int Hashtable::hash_function(vector<double> &image,int testing){
    vector<int>a;
    vector<unsigned int>hash_results;    //h1,h2,...,hk
    for (int counter = 0; counter < this->K; counter++)
    {
        /*Calculate a vector(o vector a apo tis diafanies(sel 19)*/
        for (int i = 0; i < this->s_vectors[counter].size(); i++)
        {
            double temp_a = double(image[i]-s_vectors[counter][i])/double(w);
            a.push_back(int(round(temp_a)));
        }
        //Calculate h(image)
        
        int m = pow (2,32-3);     //TOUTO DAME PREPI NA ALLAKSI
        int M = pow (2, 32/this->K);      //PREPI NA ALLAKSI
        int hash = a[a.size()-1] % M;
        for (int d = a.size()-2; d >= 0; d--)
        {
            hash+=(a[d] * m) % M;
            m = m*m;    //Overflow?
        }
        hash = hash % M;
        hash_results.push_back(hash);
    }
    //Concatenate the hash_results to one final result
    unsigned int final_hash=0;
    //O tipos pu kamnoume concat ta hash results(pou to stack overflow pu estile o xamodrakas stis apories)
    //unsigned int temp = (byte1) | (byte2 << 8) | (byte3 << 16) | (byte4 << 24);
    for (int i = 0; i < hash_results.size() ; i++)
    {
        final_hash = final_hash | hash_results[i] << i*8 ;
    }
    
    return final_hash % 60000/16;  //Return to teliko result tis hash function
}

//Inset the index of image in the Hashtable
void Hashtable::insert(vector<double> &image,int image_index){
    int index = this->hash_function(image,image_index);
    this->hashtable[index].push_back(image_index);
}

//Print the contents of each bucket
void Hashtable::print(){
    for (int i = 0; i < this->table_size; i++)
    {
        cout << "Bucket " << i << " has:" << endl;
        for (int j = 0; j < this->hashtable[i].size(); j++)
        {
            cout << this->hashtable[i].at(j) << " " ;
        }
        cout << endl;
    }
    
}

//Returns the vector with the images indexes that are in bucket "bucket index"
vector<int> Hashtable::get_bucket_imgs(int bucket_index){
    return this->hashtable[bucket_index];
}

Hashtable::~Hashtable()
{
    delete[] this->hashtable;
}


#endif