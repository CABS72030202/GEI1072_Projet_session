#include "../src/karnaugh.h"

void simplify_eq(Equation* eq) {
    if(eq->var_count < 2)       // Equation is already simplified
        return;
    int** k_map = generate_karnaugh_map(eq);
}

int** generate_karnaugh_map(Equation* eq) {
    int k_size[2] = {0,0};
    calc_karnaugh_size(&k_size, eq->var_count);          // Rows = [0] | Columns = [1]
    int** k_map = (int**)calloc(k_size[0], sizeof(int*));
    for(int i = 0; i < k_size[0]; i++)
        k_map[i] = (int*)calloc(k_size[1], sizeof(int));
    




    printf("\n");
    for(int i = 0; i < k_size[0]; i++) {
        printf("\n");
        for(int j = 0; j < k_size[1]; j++)
            printf("%i\t", k_map[i][j]);
    }
    printf("\n");
    return k_map;
}

void calc_karnaugh_size(int size[], int var_count) {
    if(var_count % 2 == 0) {    // Var_count is even
        size[0] = pow(2, (var_count / 2));
        size[1] = size[0];
    }
    else {                      // Var_count is odd
        size[0] = pow(2, ((var_count - 1) / 2));
        size[1] = size[0] * 2;
    }
}