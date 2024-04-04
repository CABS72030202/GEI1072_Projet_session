#include "../src/karnaugh.h"

Equation simplified_eq(Equation* eq) {
    if(eq->var_count < 2)       // Equation is already simplified
        return *eq;
    char* simp_exp = karnaugh_algorithm(eq);
    Equation simp_eq = {eq->var_count, eq->truth_table, simp_exp};
    return simp_eq;
}

char* karnaugh_algorithm(Equation* eq) {
    int k_size[2] = {0,0}, i = 0, j = 0;
    calc_karnaugh_size(&k_size, eq->var_count);                                             // Rows = [0] | Columns = [1]
    int** k_map = generate_karnaugh_map(eq, k_size);
    char* simp_exp = NULL;
    for(i = 0; i < k_size[0]; i++) 
        for(j = 0; j < k_size[1]; j++)
            if(k_map[i][j] == 1) {

                // Linked list initialization
                Cell* cell = (Cell*)malloc(sizeof(Cell));
                cell->cell_exp = "First element";
                cell->next = NULL;

                // Find consecutive cells and add them to the list
                add_back(cell, get_cell_exp(eq->var_count, i, j));

                // Replace the value in the Karnaugh map

                // Remove the first element used to initialize the linked list
                pop_front(&cell);

                // Group simplification
                
                // String concatenation with the simplified term and the simplified expression

                // Free linked list
                while(cell != NULL)
                    pop_front(&cell);
                free(cell);
            }
    if(simp_exp == NULL)
        return eq->bool_exp;
    else
        print_bool_exp(simp_exp, stdout);
        return simp_exp;
}

int** generate_karnaugh_map(Equation* eq, int k_size[]) {
    int i, j, s = 0;
    
    // Memory allocation
    int** k_map = (int**)calloc(k_size[0], sizeof(int*));
    int* fill_order = (int*)calloc(k_size[0] * k_size[1], sizeof(int));
    for(i = 0; i < k_size[0]; i++)
        k_map[i] = (int*)calloc(k_size[1], sizeof(int));

    // Fill Karnaugh map array
    get_fill_order(fill_order, eq->var_count);
    for(i = 0; i < k_size[0]; i++) 
        for(j = 0; j < k_size[1]; j++) 
            k_map[i][j] = eq->truth_table[fill_order[s++]][eq->var_count];
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

void get_fill_order(int order[], int var_count) {
    switch(var_count) {
        case 2:
        int order2[4] = {0,1,2,3};
        for(int i = 0; i < 4; i++)
            order[i] = order2[i];
        break;

        case 3:
        int order3[8] = {0,1,3,2,4,5,7,6};
        for(int i = 0; i < 8; i++)
            order[i] = order3[i];
        break;

        case 4:
        int order4[16] = {0,1,3,2,
                         4,5,7,6,
                         12,13,15,14,
                         8,9,11,10};
        for(int i = 0; i < 16; i++)
            order[i] = order4[i];
        break;
        
        default:
            exit_error("get_fill_order (invalid size) | karnaugh.c");
    }
}

char* get_cell_exp(int var_count, int row, int col) {
    switch(var_count) {
        case 2:
        char* var_state2[2][2] = {"A'B'","A'B",
                                "AB'","AB"};
        return var_state2[row][col];

        case 3:
        char* var_state3[4][2] = {"A'B'C'","A'B'C","A'BC","A'BC'",
                                "AB'C'","AB'C","ABC","ABC'"};
        return var_state3[row][col];

        case 4:
        char* var_state4[4][4] = {"A'B'C'D'","A'B'C'D","A'B'CD","A'B'CD'",
                                "A'BC'D'","A'BC'D","A'BCD","A'BCD'",
                                "ABC'D'","ABC'D","ABCD","ABCD'",
                                "AB'C'D'","AB'C'D","AB'CD","AB'CD'",};
        return var_state4[row][col];
        
        default:
            exit_error("get_fill_order (invalid size) | karnaugh.c");
    }
}

// Linked list manipulations
void add_back(Cell* cell, char* cell_exp) {
    Cell* current = cell;
    while (current->next != NULL) 
        current = current->next;
    current->next = (Cell*)malloc(sizeof(Cell));
    current->next->cell_exp = cell_exp;
    current->next->next = NULL;
}

void add_front(Cell** cell, char* cell_exp) {
    Cell* new_cell;
    new_cell = (Cell*)malloc(sizeof(Cell));
    new_cell->cell_exp = cell_exp;
    new_cell->next = *cell;
    *cell = new_cell;
}

char* pop_back(Cell* cell) {
    char* str = NULL;
    if(cell->next == NULL) {  // Only one item in the list
        str = cell->cell_exp;
        free(cell);
        return str;
    }
    Cell* current = cell;
    while (current->next->next != NULL)
        current = current->next;
    str = current->next->cell_exp;
    free(current->next);
    current->next = NULL;
    return str;
}

char* pop_front(Cell** cell) {
    char* next_exp = NULL;
    Cell* next_cell = NULL;
    if (*cell == NULL)
        return NULL;
    next_cell = (*cell)->next;
    next_exp = (*cell)->cell_exp;
    free(*cell);
    *cell = next_cell;
    return next_exp;
}

char* pop_at(Cell** cell, int pos) {
    int i = 0;
    char* str = NULL;
    Cell* current = *cell;
    Cell* temp_cell = NULL;
    if (pos == 0) 
        return pop_front(cell);
    for (i = 0; i < pos-1; i++) {
        if (current->next == NULL)
            return NULL;
        current = current->next;
    }
    if (current->next == NULL)
        return NULL;
    temp_cell = current->next;
    str = temp_cell->cell_exp;
    current->next = temp_cell->next;
    free(temp_cell);
    return str;
}

void print_list(Cell* cell) {
    Cell* current = cell;
    while(current != NULL) {
        printf("\ncell exp = %s\n", current->cell_exp);
        current = current->next;
    }
}