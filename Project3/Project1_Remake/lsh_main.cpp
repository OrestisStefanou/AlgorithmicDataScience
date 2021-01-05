#include "reader.hpp"
#include "lsh.hpp"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

using namespace std;

void print_help()
{
    cout << "Usage is:" << endl;
    cout << "./search -d <input file original space> -i <input file new space> -q <query file original space> -s <query file new space> -k <int> -L <int> -ο <output file>";
    cout << endl;
}

int main(int argc, char const *argv[])
{
    int k = 4, L = 5, N = 1;
    double R = 1.0;
    char input_file_original[250];
    char input_file_new_space[250];
    char output_file[250];
    char query_file_original[250];
    char query_file_new_space[250];
    char trainLabelsPath[250];
    char queryLabelsPath[250];

    int EMD_flag = 0;
    int exactDist[2];
    if (argc == 15)
    {
        //Parse the command line arguments
        for (int i = 1; i < argc; i++)
        {
            if (strcmp(argv[i], "-d") == 0)
            {
                if (i + 1 < argc)
                {
                    stpcpy(input_file_original, argv[i + 1]);
                    i++;
                    continue;
                }
                else
                {
                    print_help();
                    exit(1);
                }
            }

            if (strcmp(argv[i], "-q") == 0)
            {
                if (i + 1 < argc)
                {
                    stpcpy(query_file_original, argv[i + 1]);
                    i++;
                    continue;
                }
                else
                {
                    print_help();
                    exit(1);
                }
            }

            if (strcmp(argv[i], "-k") == 0)
            {
                if (i + 1 < argc)
                {
                    k = atoi(argv[i + 1]);
                    i++;
                    continue;
                }
                else
                {
                    print_help();
                    exit(1);
                }
            }

            if (strcmp(argv[i], "-L") == 0)
            {
                if (i + 1 < argc)
                {
                    L = atoi(argv[i + 1]);
                    i++;
                    continue;
                }
                else
                {
                    print_help();
                    exit(1);
                }
            }

            if (strcmp(argv[i], "-o") == 0)
            {
                if (i + 1 < argc)
                {
                    stpcpy(output_file, argv[i + 1]);
                    i++;
                    continue;
                }
                else
                {
                    print_help();
                    exit(1);
                }
            }

            if (strcmp(argv[i], "-i") == 0)
            {
                if (i + 1 < argc)
                {
                    stpcpy(input_file_new_space, argv[i + 1]);
                    i++;
                    continue;
                }
                else
                {
                    print_help();
                    exit(1);
                }
            }

            if (strcmp(argv[i], "-s") == 0)
            {
                if (i + 1 < argc)
                {
                    stpcpy(query_file_new_space, argv[i + 1]);
                    i++;
                    continue;
                }
                else
                {
                    print_help();
                    exit(1);
                }
            }
        }
    }
    else if (argc == 12)
    {
        EMD_flag = 1;
        //Parse the command line arguments
        for (int i = 1; i < argc; i++)
        {
            if (strcmp(argv[i], "-d") == 0)
            {
                if (i + 1 < argc)
                {
                    stpcpy(input_file_original, argv[i + 1]);
                    i++;
                    continue;
                }
                else
                {
                    print_help();
                    exit(1);
                }
            }

            if (strcmp(argv[i], "-q") == 0)
            {
                if (i + 1 < argc)
                {
                    stpcpy(query_file_original, argv[i + 1]);
                    i++;
                    continue;
                }
                else
                {
                    print_help();
                    exit(1);
                }
            }

            if (strcmp(argv[i], "-o") == 0)
            {
                if (i + 1 < argc)
                {
                    stpcpy(output_file, argv[i + 1]);
                    i++;
                    continue;
                }
                else
                {
                    print_help();
                    exit(1);
                }
            }

            if (strcmp(argv[i], "-l1") == 0)
            {
                if (i + 1 < argc)
                {
                    stpcpy(trainLabelsPath, argv[i + 1]);
                    i++;
                    continue;
                }
                else
                {
                    print_help();
                    exit(1);
                }
            }

            if (strcmp(argv[i], "-l2") == 0)
            {
                if (i + 1 < argc)
                {
                    stpcpy(queryLabelsPath, argv[i + 1]);
                    i++;
                    continue;
                }
                else
                {
                    print_help();
                    exit(1);
                }
            }
        }
    }

    else //Read the filepaths from the user
    {
        cout << "Give original input filepath:" << endl;
        fgets(input_file_original, 250, stdin);
        input_file_original[strlen(input_file_original) - 1] = '\0'; //Strip the '\n' character
        cout << "Give original query filepath:" << endl;
        fgets(query_file_original, 250, stdin);
        query_file_original[strlen(query_file_original) - 1] = '\0'; //Strip the '\n' character
        cout << "Give new space input filepath:" << endl;
        fgets(input_file_new_space, 250, stdin);
        input_file_new_space[strlen(input_file_new_space) - 1] = '\0'; //Strip the '\n' character
        cout << "Give new space query filepath:" << endl;
        fgets(query_file_new_space, 250, stdin);
        query_file_new_space[strlen(query_file_new_space) - 1] = '\0'; //Strip the '\n' character
        cout << "Give output filepath:" << endl;
        fgets(output_file, 250, stdin);
        output_file[strlen(output_file) - 1] = '\0'; //Strip the '\n' character
    }

    /*cout << "Input file:" << input_file << endl;
    cout << "Query file:" << query_file << endl;
    cout << "k:" << k << endl;
    cout << "L:" << L << endl;
    cout << "Ouput file:" << output_file << endl;
    cout << "N:" << N << endl;
    cout << "R:" << R << endl;*/

    vector<vector<double>> training_data_original;
    vector<vector<double>> query_data_original;
    vector<vector<double>> training_data_new_space;
    vector<vector<double>> query_data_new_space;

    //Read the original training dataset
    if (ReadData(training_data_original, (char *)input_file_original))
    {
        exit(1);
    }

    //Read the original query dataset
    if (ReadData(query_data_original, (char *)query_file_original))
    {
        exit(1);
    }

    if (EMD_flag == 0)
    {
        //Read the new space training dataset
        if (ReadDataCompressed(training_data_new_space, (char *)input_file_new_space))
        {
            exit(1);
        }

        //Read the new space query dataset
        if (ReadDataCompressed(query_data_new_space, (char *)query_file_new_space))
        {
            exit(1);
        }

        LSH lsh = LSH(k, L, training_data_original, R, (char *)"L1");
        LSH lsh2 = LSH(k, L, training_data_new_space, R, (char *)"L1");

        ofstream outfile;
        outfile.open(output_file); //Create the outpout file

        double ApproxFactorLSH, ApproxFactorRed;
        int ApproxFactorCounter;
        ApproxFactorLSH = 0;
        ApproxFactorRed = 0;
        ApproxFactorCounter = 0;
        
        for (int i = 0; i < query_data_original.size(); i++)
        {
            //Find the approximate N nearest neighbors
            clock_t start, end;
            start = clock();
            vector<pair<int, int>> appr_results = lsh.knn(query_data_original[i], i, N);
            end = clock();
            double appr_knn_time = double(end - start) / double(CLOCKS_PER_SEC);
            //Find the exact nearest neighbors
            start = clock();
            vector<pair<int, int>> exact_results = lsh.exact_nearest_neighbor(query_data_original[i], N);
            exactDist[1]=exact_results[0].second;
            end = clock();
            double exact_knn_time = double(end - start) / double(CLOCKS_PER_SEC);
            //Do knn for latent space
            //Find the exact  N nearest neighbors in latent space
            start = clock();
            vector<pair<int, int>> exact_results_latent = lsh2.exact_nearest_neighbor(query_data_new_space[i], N);
            end = clock();
            double exact_knn_time_latent = double(end - start) / double(CLOCKS_PER_SEC);

            //Check the results
            //compareResults(trainLabels,testLabels,appr_results,i);

            //Write the results in the output file
            outfile << "Query:" << i << "\n";
            for (int j = 0; j < appr_results.size(); j++)
            {
                outfile << "Nearest neighbor Reduced:" << j + 1 << ": " << exact_results_latent[j].first << "\n";
                outfile << "Nearest neighbor LSH:" << j + 1 << ": " << appr_results[j].first << "\n";
                outfile << "Nearest neighbor True:" << j + 1 << ": " << exact_results[j].first << "\n";
                outfile << "distanceReduced:" << exact_results_latent[j].second << "\n";
                outfile << "distanceLSH:" << appr_results[j].second << "\n";
                outfile << "distanceTrue:" << exact_results[j].second << "\n";

                //Sum Approximation Factor
                ApproxFactorLSH = (double)ApproxFactorLSH + (double)(appr_results[j].second+exactDist[0]) / (double)exact_results[j].second;
                ApproxFactorRed = (double)ApproxFactorRed + (double)(exact_results_latent[j].second+exactDist[1]) / (double)exact_results[j].second;
                
            }
            ApproxFactorCounter++;
            outfile << "tReduced:" << setprecision(5) << exact_knn_time_latent << "\n";
            outfile << "tLSH:" << setprecision(5) << appr_knn_time << "\n";
            outfile << "tTrue:" << setprecision(5) << exact_knn_time << "\n";

            //Calculate average Approximation Factor
        }
        double ApproximationFactorLSH = (double)ApproxFactorLSH / (double)ApproxFactorCounter;
        double ApproximationFactorRed = (double)ApproxFactorRed / (double)ApproxFactorCounter;
        outfile << "Approximation Factor LSH:" << setprecision(5) << ApproximationFactorLSH << "\n";
        outfile << "Approximation Factor Reduced:" << setprecision(5) << ApproximationFactorRed << "\n";
        outfile.close();

        return 0;
    }
    else
    {
        N = 10;
        //Read the lables
        vector<int> trainLabels;
        if (readLabels(trainLabels, trainLabelsPath))
        {
            exit(1);
        }
        vector<int> testLabels;
        if (readLabels(testLabels, queryLabelsPath))
        {
            exit(1);
        }

        vector<vector<double>> training_data_clusters;
        vector<vector<double>> training_data_clusters_weights;

        vector<vector<double>> query_data_clusters;
        vector<vector<double>> query_data_clusters_weights;
        getImgsCluster(training_data_original, 16, training_data_clusters, training_data_clusters_weights);
        getImgsCluster(query_data_original, 16, query_data_clusters, query_data_clusters_weights);

        LSH lsh = LSH(k, L, training_data_clusters, training_data_clusters_weights, R, (char *)"EMD");
        LSH lsh2 = LSH(k, L, training_data_original, R, (char *)"L1");
        ofstream outfile;
        outfile.open(output_file); //Create the outpout file

        int totalCorrectEMD = 0;
        int totalWrongEMD = 0;
        int totalCorrectL1 = 0;
        int totalWrongL1 = 0;
        for (int i = 0; i < 10; i++)
        {
            //Find the approximate N nearest neighbors
            vector<pair<int, int>> appr_results_EMD = lsh.knn(query_data_clusters[i], query_data_clusters_weights[i], i, N);
            vector<pair<int, int>> appr_results_L1 = lsh2.knn(query_data_original[i], i, N);
            //Check the results
            //cout << "Average Correct Search Results EMD:" << setprecision(5) << appr_results_EMD[0][0] << "\n";
            //cout << "Average Correct Search Results MANHATTAN:" << setprecision(5) << appr_results_L1 << "\n";

            pair<int, int> results;
            results = compareResults(trainLabels, testLabels, appr_results_EMD, i);
            totalCorrectEMD = totalCorrectEMD + results.first;
            totalWrongEMD = totalWrongEMD + results.second;
            results = compareResults(trainLabels, testLabels, appr_results_L1, i);
            totalCorrectL1 = totalCorrectL1 + results.first;
            totalWrongL1 = totalWrongL1 + results.second;
        }

        double avgCorrectEMD = double(double(totalCorrectEMD) / double((totalCorrectEMD + totalWrongEMD)));
        double avgCorrectL1 = double(double(totalCorrectL1) / double((totalCorrectL1 + totalWrongL1)));
        //Write the results

        cout << "Average Correct Search Results EMD:" << setprecision(5) << avgCorrectEMD << "\n";
        cout << "Average Correct Search Results MANHATTAN:" << setprecision(5) << avgCorrectL1 << "\n";
        outfile.close();

        return 0;
    }
}
