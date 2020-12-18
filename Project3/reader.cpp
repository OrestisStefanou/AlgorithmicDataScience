#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<vector>

using namespace std;

int main(){
    char line[300];
    FILE *fp;
    fp = fopen("compressedImgs","r");
	char *eptr;	//for strtod
	//Read number of images
	fgets(line,300,fp);
	int imgNumber = atoi(line);
	printf("NUmber of images:%d\n",imgNumber);
	//Read number of rows
    fgets(line,300,fp);
    int rowsNumber = atoi(line);
    printf("NUmber of rows:%d\n",rowsNumber);
	//Read number of columns
    fgets(line,300,fp);
    int colNumber = atoi(line);
    printf("NUmber of columns:%d\n",colNumber);
	vector<vector<double> > data;
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
    while(fgets(line,300,fp)){
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
	for(int i=0;i<11;i++){
		for(int j=0;j<rowsNumber * colNumber;j++)
			printf("%f ",data[i][j]);
		printf("\n");
	}

    return 0;
}
