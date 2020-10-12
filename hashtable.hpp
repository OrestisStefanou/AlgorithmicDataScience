#ifndef HASHTABLE_H
#define HASHTABLE_H
#include<vector>
#include<iostream>
#include<time.h>

using namespace std;

class Hashtable
{
private:
    vector<int> *hashtable;     //Hashtable me ta index ton ikonon.Pinakas me vectors(buckets)
    int hash_type;            //To int en tixeo,theloume kati pu tha kamni diaforetiki hashfunction gia kathe Hashtable
    int table_size;
    int K;
    int w;
    vector<int> s;          //To dianisma s apo diafania 19
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

    srand (hash_type);
    //Create vector s
    for (int i = 0; i < d; i++)
    {
        this->s.push_back(rand() % (w-1) + 0);
    }
    
}

//Mia tixea hash function.To int testing en xriazete apla en gia tora
int Hashtable::hash_function(vector<double> &image,int testing){
    vector<int>a;
    vector<int>hash_results;    //h1,h2,...,hk
    for (int counter = 0; counter < this->K; counter++)
    {
        /*Calculate a vector(o vector a apo tis diafanies(sel 19)*/
        for (int i = 0; i < this->s.size(); i++)
        {
            a.push_back((image[i]-s[i])/w);
        }
        //Calculate h(image)
        
        int m = 10;     //TOUTO DAME PREPI NA ALLAKSI
        int M = 10;      //PREPI NA ALLAKSI
        int hash = a[a.size()-1] % M;
        for (int d = a.size()-2; d >= 0; d--)
        {
            hash+=(a[d] * m) % M;
            m = m*m;
        }
        hash = hash % M;
        hash_results.push_back(hash);
    }
    //Concatenate the hash_results to one final result
    
    return testing % this->table_size;  //Return to teliko result tis hash function
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