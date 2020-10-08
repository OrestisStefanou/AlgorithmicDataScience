#ifndef READER_H
#define READER_H
 
#include<iostream>
#include<vector>
#include<fstream>

using namespace std;

int ReverseInt (int i)
{
    unsigned char ch1, ch2, ch3, ch4;
    ch1=i&255;
    ch2=(i>>8)&255;
    ch3=(i>>16)&255;
    ch4=(i>>24)&255;
    return((int)ch1<<24)+((int)ch2<<16)+((int)ch3<<8)+ch4;
}

//Read the MNIST data and load the dataset to data_vector
void ReadData(vector<vector<double>> &data_vector,char *data_path)
{
    ifstream file (data_path,ios::binary);
    if (file.is_open())
    {
        int number_of_rows=0;
        int number_of_cols=0;
        int number_of_images=0;
        int magic_num=0;
        
        //READ THE METADATA
        file.read((char*)&magic_num,sizeof(magic_num));
        magic_num= ReverseInt(magic_num);
        file.read((char*)&number_of_images,sizeof(number_of_images));
        number_of_images= ReverseInt(number_of_images);
        file.read((char*)&number_of_rows,sizeof(number_of_rows));
        number_of_rows= ReverseInt(number_of_rows);
        file.read((char*)&number_of_cols,sizeof(number_of_cols));
        number_of_cols= ReverseInt(number_of_cols);

        //cout << magic_num << endl;
        //cout << number_of_images << endl;
        //cout << n_rows << endl;
        //cout << n_cols << endl;

        data_vector.resize(number_of_images,vector<double>(number_of_rows * number_of_cols));   //Resize the vector to fit the data
        //READ THE PIXEL DATA
        unsigned char pixel=0;
        for(int i=0;i<number_of_images;++i)
        {
            for(int r=0;r<number_of_rows;++r)
            {
                for(int c=0;c<number_of_cols;++c)
                {
                    pixel=0;
                    file.read((char*)&pixel,sizeof(pixel));
                    data_vector[i][(number_of_rows*r)+c]= (double)pixel;
                }
            }
        }
    }else
    {
        cout << "Something went wrond at opening the file";
    }
    
}
 
#endif