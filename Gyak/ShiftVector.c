#include <stdio.h>

int main(){

    int matrix[3][3] = {
        {1, 0, 5},
        {0, 1, 2},
        {0, 0, 1}
    };
    int n = sizeof(matrix) / sizeof(matrix[0]);
    int eltolt_vektor[] = {0,0,0};
    int vektor[] = {10,4,1};
    
    printf("The martix and the vektor\n");
    for (int i = 0; i<n; i++){
        for(int j = 0; j < n;j++){
            printf("%d, ", matrix[i][j]);

        }
        printf("             %d\n",vektor[i]);
       

    }
    printf("\nShifted vector \n");

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            eltolt_vektor[i] += matrix[i][j]*vektor[j];
            
        }
        printf("%d\n", eltolt_vektor[i]);
    }

    return 0;
}