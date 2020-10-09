#ifndef HASHTABLE_H
#define HASHTABLE_H
#include<vector>
#include<iostream>

using namespace std;

class Hashtable
{
private:
    vector<int> *hashtable;     //Hashtable me ta index ton ikonon.Pinakas me vectors(buckets)
    int hash_type;            //To int en tixeo,theloume kati pu tha kamni diaforetiki hashfunction gia kathe Hashtable
    int table_size;
public:
    Hashtable(int size,int hash_type);
    int hash_function(vector<double> &image,int testing);
    void insert(vector<double> &image,int image_index);
    vector<int> get_bucket_imgs(int bucket_index);
    void print();   //Test function
    ~Hashtable();
};

//Ston constructor prepi kapos meso pu kapio orismatos na tou leme ti hash function na eshi to Hashtable
Hashtable::Hashtable(int size,int hash_type)
{
    this->hashtable = new vector<int>[size];
    this->hash_type = hash_type;
    this->table_size = size;
}

//Mia tixea hash function.To int testing en xriazete apla en gia tora
int Hashtable::hash_function(vector<double> &image,int testing){
    return testing % this->table_size;
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