#include<stdio.h>

int main(){
    int n;
    
    while (scanf("%d", &n) == 1) {
        double result = 1;
        double matrix[n][n];

        // read in info for matrix
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                scanf("%lf", &matrix[i][j]);
            }
        }

        for (int diag = 0; diag < n; diag++) {
            double pivot = matrix[diag][diag]; // thisis the number at the diagonal

            if (pivot == 0) { // gotta row swap if number in diagonal is zero
                for (int i = diag + 1; i < n; i++) {

                    if (matrix[i][diag] != 0) {
                        double temp;

                        for (int j = 0; j < n; j++) {
                            temp = matrix[i][j];
                            matrix[i][j] = matrix[diag][j];
                            matrix[diag][j] = temp;
                        }
                        result *= -1; // swapping a row changes the sign of the determinant
                        break;
                    }
                }
                pivot = matrix[diag][diag];

                if (pivot == 0) { // if no row swap could be made to remove 0 from diagonal, determinant is 0
                    printf("%d\n", 0);
                    return 1;
                }
            }

            result *= pivot;

            for (int i = diag + 1; i < n; i++) {

                if (matrix[i][diag] != 0) {
                    double c = -(matrix[i][diag]/pivot); // c is the multiple needed to multiply the row by to add and get 0

                    for (int j = 0; j < n; j++) {
                        matrix[i][j] += c * matrix[diag][j];
                    }

                }
            }
        }

        printf("%lf\n", result);
    }
    return 0;
}