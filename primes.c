/*
Sonam Tailor 
Lab #2: Cellular Automaton 
Due: November 30th 
Professor Zahran
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>

int dim;

int num_neighbors(int matrix[dim][dim], int i, int j, int Y){
  int count = 0;
  // printf("\n");
  // printf("%d %d", i,j);
  //only increment the count if it is alive!! 

  //condensed the expanded if statements 
  if (i-1 >= 0 && j-1 >= 0){ //middle case
      if (matrix[i-1][j-1] == 1){
          count+=1;
      }
  }

    if (i-1 >= 0){ //middle case
      if(matrix[i-1][j] == 1){
        count+=1;
      }
  }

  if (i-1 >= 0 && j+1 < Y){ //middle case
      if(matrix[i-1][j+1] == 1){
        count+=1;
      }
    }

    if (j-1 >= 0){ //outer
      if(matrix[i][j-1] == 1){
        count+=1;
      }
    }

    if (j+1 < Y){
      if(matrix[i][j+1] == 1){
        count+=1;
      }
    }

  if (i+1 < Y && j-1 >=0){ //midd
    if(matrix[i+1][j-1] == 1){
      count+=1;
    }
  }

  if (i+1 < Y){
    if(matrix[i+1][j] == 1){
        count+=1;
    }
  }

  if (i+1 < Y && j+1 < Y){
    if(matrix[i+1][j+1] == 1){
      count+=1;
    }
  }

  return count;

}


int main(int c, char* argv[]){

  int generations = atoi(argv[1]); //store the number of generations 
  dim = atoi(argv[2]); //dimension of the matrix
  int numThreads = atoi(argv[3]);

  FILE *input;
  input = fopen(argv[4],"r");

  int i,j;
  int matrix[dim][dim];
  int newGen[dim][dim];
  // int matrix = (int*)malloc(sizeof(int[dim][dim]));
  // int newGen = (int*)malloc(sizeof(int[dim][dim]));
  int gen;
  //for timing purposes
  // double start;
  // double end;


  //store data from the file in a 2D array
  for (i = 0; i < dim; i++){
    for (j = 0; j < dim; j++){
      //int temp;
      fscanf(input, "%d", &matrix[i][j]); //can just directly store
      //matrix[i][j] = temp;
    }
  }

    //start = omp_get_wtime();
    for(gen = 0; gen < generations; gen++){
        # pragma omp parallel for num_threads(numThreads)
        for(int i = 0; i < dim; i++){
            for(int j = 0; j < dim; j++){
                int neighbor_count = num_neighbors(matrix, i, j, dim);
                if(neighbor_count == 2){ //if its dead - stays dead.. if its alive - stays alive
                  newGen[i][j] = matrix[i][j];
                }
                if(neighbor_count == 3){
                  newGen[i][j] = 1;
                }
                if(neighbor_count < 2 || neighbor_count>3){ //dies
                  newGen[i][j] = 0;
                }
            }
        }
        # pragma omp barrier
        //# pragma omp parallel for num_threads(threadNum)
        for(i = 0; i < dim; i++){
            for(j = 0; j < dim; j++){
                matrix[i][j] = newGen[i][j];
            }
        }
    }
   // end = omp_get_wtime();
   // printf("The total time took %f seconds\n",end - start);
   // printMatrix(x,y,matrix);


    FILE *output;

    char file[50] = "filename.out";

  //open the file for writing 
    output = fopen(file,"w");

  //write to file
  for (i = 0; i < dim; i++){
    for (j = 0; j < dim; j++){
      fprintf(output,"%d ",matrix[i][j]);
    }
    fprintf(output,"\n");
  }
  fclose(output);
    return 0;
}