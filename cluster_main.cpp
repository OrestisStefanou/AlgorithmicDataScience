#include"reader.hpp"
#include"clustering.hpp"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

using namespace std;

void print_help(){
    cout << "Usage is:" << endl;
    cout << "./cluster –i <input file> –c <configuration file> -o <output file> -complete <optional> -m <method: Classic OR LSH or Hypercube>" << endl;
}

int main(int argc, char const *argv[])
{
    int K,L=3,number_of_vector_hash_functions=4;
    int max_number_M_hypercube=10,number_of_hypercube_dimensions=3,number_of_probes=2;

    char input_file[250],output_file[250],conf_file[250];
    char method[50];

    if(argc == 10){
        //Parse the command line arguments
        for (int i = 1; i < argc; i++)
        {
            if(strcmp(argv[i],"-i")==0){
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

            if(strcmp(argv[i],"-c")==0){
                if(i+1 < argc){
                    stpcpy(conf_file,argv[i+1]);
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

            if(strcmp(argv[i],"-m")==0){
                if(i+1 < argc){
                    strcpy(method,argv[i+1]);
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
        print_help();
    }
    //Read the conf file
    FILE *fp = fopen(conf_file,"r");
    if(fp==NULL){
        cout << "Error during opening the file" << endl;
        exit(1);
    }
    char buf[250];
    char variable[100];
    char temp_value[10];
    int value;

    while (fgets(buf,250,fp))
    {
        int buf_index = 0;
        int index = 0;
        memset(variable,0,100);
        memset(temp_value,0,10);
        while (buf[buf_index]!=':')
        {
           variable[index] = buf[buf_index];
           index++;
           buf_index++;  
        }
        variable[index] = '\0';
        buf_index++;

        index = 0;
        while (buf[buf_index]!='\n' || buf[buf_index]==EOF)
        {
           temp_value[index] = buf[buf_index];
           index++;
           buf_index++;  
        }
        index++;
        buf_index++;
        temp_value[index] = '\0';
        value = atoi(temp_value);
        if(strlen(temp_value)==0){
            continue;
        }
        if(strcmp(variable,"number_of_clusters")==0){
            K = value;
            continue;
        }
        if(strcmp(variable,"number_of_vector_hash_tables")==0){
            L = value;
            continue;
        }
        if(strcmp(variable,"number_of_vector_hash_functions")==0){
            number_of_vector_hash_functions = value;
            continue;
        }
        if(strcmp(variable,"max_number_M_hypercube")==0){
            max_number_M_hypercube = value;
            continue;
        }
        if(strcmp(variable,"number_of_hypercube_dimensions")==0){
            number_of_hypercube_dimensions = value;
            continue;
        }
        if(strcmp(variable,"number_of_probes")==0){
            number_of_probes = value;
            continue;
        }
    }
    fclose(fp);
    cout << "Number of clusters:" << K << endl;
    cout << "number_of_vector_hash_tables:" << L << endl;
    cout << "number_of_vector_hash_functions:" << number_of_vector_hash_functions << endl;
    cout << "max_number_M_hypercube:" << max_number_M_hypercube << endl;
    cout << "number_of_hypercube_dimensions:" << number_of_hypercube_dimensions << endl;
    cout << "number of probes:" << number_of_probes << endl;
    return 0;
}
