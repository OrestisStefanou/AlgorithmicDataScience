#include"reader.hpp"
#include"lsh.hpp"
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

using namespace std;

void print_help(){
    cout << "Usage is:" << endl;
    cout << "./lsh –d <input file> –q <query file> –k <int> -L <int> -ο <output file> -Ν <number of nearest> -R <radius>";
    cout << endl;
}

int main(int argc, char const *argv[])
{
    int k=4,L=5,N=1;
    double R = 1.0;
    char input_file[250];
    char output_file[250];
    char query_file[250];

    if(argc == 15){
        //Parse the command line arguments
        for (int i = 1; i < argc; i++)
        {
            if(strcmp(argv[i],"-d")==0){
                if(i+1 < argc){
                    stpcpy(input_file,argv[i+1]);
                    i++;
                    continue;
                }else
                {
                    print_help();
                    exit(1);
                } 
            }

            if(strcmp(argv[i],"-q")==0){
                if(i+1 < argc){
                    stpcpy(query_file,argv[i+1]);
                    i++;
                    continue;
                }else
                {
                    print_help();
                    exit(1);
                } 
            }

            if(strcmp(argv[i],"-k")==0){
                if(i+1 < argc){
                    k = atoi(argv[i+1]);
                    i++;
                    continue;
                }else
                {
                    print_help();
                    exit(1);
                } 
            }

            if(strcmp(argv[i],"-L")==0){
                if(i+1 < argc){
                    L = atoi(argv[i+1]);
                    i++;
                    continue;
                }else
                {
                    print_help();
                    exit(1);
                } 
            }

            if(strcmp(argv[i],"-o")==0){
                if(i+1 < argc){
                    stpcpy(output_file,argv[i+1]);
                    i++;
                    continue;
                }else
                {
                    print_help();
                    exit(1);
                } 
            }

            if(strcmp(argv[i],"-N")==0){
                if(i+1 < argc){
                    N = atoi(argv[i+1]);
                    i++;
                    continue;
                }else
                {
                    print_help();
                    exit(1);
                } 
            }

            if(strcmp(argv[i],"-R")==0){
                if(i+1 < argc){
                    R = atof(argv[i+1]);
                    i++;
                    continue;
                }else
                {
                    print_help();
                    exit(1);
                } 
            }
        }
    }else   //Read the filepaths from the user
    {
        cout << "Give input filepath:" << endl;
        fgets(input_file,250,stdin);
        input_file[strlen(input_file)-1] = '\0';    //Strip the '\n' character
        cout << "Give query filepath:" << endl;
        fgets(query_file,250,stdin);
        query_file[strlen(query_file)-1] = '\0';    //Strip the '\n' character    
        cout << "Give output filepath:" << endl;
        fgets(output_file,250,stdin);
        output_file[strlen(output_file)-1] = '\0';    //Strip the '\n' character
    }
    
    
    /*cout << "Input file:" << input_file << endl;
    cout << "Query file:" << query_file << endl;
    cout << "k:" << k << endl;
    cout << "L:" << L << endl;
    cout << "Ouput file:" << output_file << endl;
    cout << "N:" << N << endl;
    cout << "R:" << R << endl;*/

    //Read the training dataset
    vector<vector<double>> training_data;
    if(ReadData(training_data,(char *)input_file)){
        exit(1);
    }

    //Read the query dataset
    vector<vector<double>> query_data;
    if(ReadData(query_data,(char *)query_file)){
        exit(1);
    }    

    LSH lsh = LSH(k,L,training_data);
    return 0;
}
