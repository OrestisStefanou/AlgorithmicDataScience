#ifndef READER_H
#define READER_H

#include <iostream>
#include <vector>
#include <fstream>
#include<string.h>
#include<stdlib.h>

using namespace std;

int NumReverse(int i)
{
    int SwapedNum;
    unsigned char FirstByte, SecondByte, ThirdByte, FourthByte;
    FirstByte = i & 255;
    SecondByte = (i >> 8) & 255;
    ThirdByte = (i >> 16) & 255;
    FourthByte = (i >> 24) & 255;
    SwapedNum = ((int)FirstByte << 24) + ((int)SecondByte << 16) + ((int)ThirdByte << 8) + FourthByte;
    return SwapedNum;
}

//Read the MNIST data and load the dataset to data_vector
int ReadData(vector<vector<double>> &data_vector, char *data_path)
{
    ifstream file(data_path, ios::binary);
    if (file.is_open())
    {
        int number_of_rows = 0;
        int number_of_cols = 0;
        int number_of_images = 0;
        int magic_num = 0;

        //READ THE METADATA
        file.read((char *)&magic_num, sizeof(magic_num));
        magic_num = NumReverse(magic_num);
        file.read((char *)&number_of_images, sizeof(number_of_images));
        number_of_images = NumReverse(number_of_images);
        file.read((char *)&number_of_rows, sizeof(number_of_rows));
        number_of_rows = NumReverse(number_of_rows);
        file.read((char *)&number_of_cols, sizeof(number_of_cols));
        number_of_cols = NumReverse(number_of_cols);

        //cout << magic_num << endl;
        //cout << number_of_images << endl;
        //cout << n_rows << endl;
        //cout << n_cols << endl;

        data_vector.resize(number_of_images, vector<double>(number_of_rows * number_of_cols)); //Resize the vector to fit the data
        //READ THE PIXEL DATA
        unsigned char pixel = 0;
        for (int i = 0; i < number_of_images; ++i)
        {
            for (int r = 0; r < number_of_rows; ++r)
            {
                for (int c = 0; c < number_of_cols; ++c)
                {
                    pixel = 0;
                    file.read((char *)&pixel, sizeof(pixel));
                    data_vector[i][(number_of_rows * r) + c] = (double)pixel;
                }
            }
        }
        return 0;
    }
    else
    {
        cout << "Something went wrond at opening the file\n";
        return -1;
    }
}

//Read the compressed data
int ReadDataCompressed(vector<vector<double>> &data,char *data_path){
    int len = 10000;    //Buffer size to get each image pixel
    char line[len];
    FILE *fp;
    fp = fopen(data_path,"r");
	if (fp == NULL){
		printf("Error during opening the file\n");
		return -1;
	}
	char *eptr;	//for strtod
	//Read number of images
	fgets(line,len,fp);
	int imgNumber = atoi(line);
	//printf("NUmber of images:%d\n",imgNumber);
	//Read number of rows
    fgets(line,len,fp);
    int rowsNumber = atoi(line);
    //printf("NUmber of rows:%d\n",rowsNumber);
	//Read number of columns
    fgets(line,len,fp);
    int colNumber = atoi(line);
    //printf("NUmber of columns:%d\n",colNumber);
	//Resize the data vector
	for (int i = 0; i < imgNumber; i++)
	{	
		vector<double> v1;
		for (int j = 0; j < colNumber * rowsNumber; j++)
		{
			v1.push_back(0);
		}
		data.push_back(v1);
	}
	int i = 0;
	//Read image's pixels
    while(fgets(line,len,fp)){
		strtok(line,"\n");
		char *token = strtok(line," ");
		for(int j=0; j < rowsNumber * colNumber; j++) {
				//printf("%s ",token);
				data[i][j] = strtod(token,&eptr);
				token = strtok(NULL," ");
		}
		i++;
		//printf("\n");
    }
	fclose(fp);
	//printf("\n\n\n");
	//Print the first 10 images from the data
	//for(int i=0;i<11;i++){
	//	for(int j=0;j<rowsNumber * colNumber;j++){
			//printf("%f ",data[i][j]);
    //    }
		//printf("\n");
	//}
    return 0;	
}

int readLabels(vector<int> &labels,char *labels_path){
    int len = 20;    //Buffer size to get each image pixel
    char line[len];
    FILE *fp;
    fp = fopen(labels_path,"r");
	if (fp == NULL){
		printf("Error during opening the file\n");
		return -1;
	}
	//Read number of labels
	fgets(line,len,fp);
	int lblNumber = atoi(line);
	//printf("NUmber of labels:%d\n",imgNumber);
	//Read the labels
    while(fgets(line,len,fp)){
        labels.push_back(atoi(line));
    }
	fclose(fp);
	//printf("\n\n\n");
	//Print the first 10 labels from the data
	//for(int i=0;i<10;i++){
	//	printf("%d\n",labels[i]);
	//}
    return 0;	
}

#endif