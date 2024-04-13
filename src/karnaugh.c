#include "../src/karnaugh.h"

Equation simplified_eq(Equation* eq) {
    if(eq->var_count < 2)       // Equation is already simplified
        return *eq;
    char* simp_exp = karnaugh_algorithm(eq);
    Equation simp_eq = {eq->var_count, eq->truth_table, simp_exp};
    return simp_eq;
}

char* karnaugh_algorithm(Equation* eq) {
    int k_size[2] = {0,0}, i = 0, j = 0, p = 0;
    int count = 0, is_first_row = 0, is_last_row = 0, is_first_col = 0, is_last_col = 0, full_line = 0;
    int current_group = 1;
    calc_karnaugh_size(&k_size, eq->var_count);                                             // Rows = [0] | Columns = [1]
    int** k_map = generate_karnaugh_map(eq, k_size);
    char* simp_exp = (char*)malloc(100);
    Cell* cell = (Cell*)malloc(10 * sizeof(Cell));
    cell->cell_exp = NULL;
    cell->group_id = -1;
    cell->next = NULL;
    for(i = 0; i < k_size[0]; i++) 
        for(j = 0; j < k_size[1]; j++)
            if(k_map[i][j] == 1) {
                if(i == k_size[0] - 1)
                    is_last_row = 1;
                else if(i == 0)
                    is_first_row = 1;
                if(j == k_size[1] - 1)
                    is_last_col = 1;
                else if(j == 0)
                    is_first_col = 1;
                    
                // Down
                if((!is_last_row && (k_map[i + 1][j] == 1 || k_map[i + 1][j] == -1)) ||
                    (is_last_row && (k_map[0][j] == 1 || k_map[0][j] == -1)))
                    count++;
                // Right
                if((!is_last_col && (k_map[i][j + 1] == 1 || k_map[i][j + 1] == -1)) ||
                    (is_last_col && (k_map[i][k_size[1] + 1] == 1 || k_map[i][k_size[1] + 1] == -1)))
                    count++;
                // Up
                if((!is_first_row && (k_map[i - 1][j] == 1 || k_map[i - 1][j] == -1)) ||
                    (is_first_row && (k_map[k_size[0] - 1][j] == 1 || k_map[k_size[0] - 1][j] == -1)))
                    count++;
                // Left
                if((!is_first_col && (k_map[i][j - 1] == 1 || k_map[i][j - 1] == -1)) ||
                    (is_first_col && (k_map[i][k_size[1] - 1] == 1 || k_map[i][k_size[1] - 1] == -1)))
                    count++;
count = 1;
                switch(count) {
                    case 0:         // Group of 1
                    add_back(cell, get_cell_exp(eq->var_count, i, j), current_group);
                    k_map[i][j] = -1;
                    break;

                    case 1:         // Line group
                    for(p = 0, full_line = 0; p < k_size[0]; p++) {
                        if(k_map[p][j] == 0)
                            break;
                        full_line++;
                    }
                    if(full_line == k_size[0] - 1) {    // Vertical full line
                        for(p = 0; p < k_size[0]; p++) {
                            add_back(cell, get_cell_exp(eq->var_count, p, j), current_group);
                            k_map[p][j] = -1;
                        }
                        break;
                    }
                    for(p = 0, full_line = 0; p < k_size[1]; p++) {
                        if(k_map[i][p] == 0)
                            break;
                        full_line++;
                    }
                    if(full_line == k_size[1] - 1) {    // Horizontal full line
                        for(p = 0; p < k_size[1]; p++) {
                            add_back(cell, get_cell_exp(eq->var_count, i, p), current_group);
                            k_map[i][p] = -1;
                        }
                        break;
                    }
                    else {                              // Group of 2
                        add_back(cell, get_cell_exp(eq->var_count, i, j), current_group);
                        k_map[i][j] = -1;
                        // Down
                        if(!is_last_row && (k_map[i + 1][j] == 1 || k_map[i + 1][j] == -1)) {
                            add_back(cell, get_cell_exp(eq->var_count, i + 1, j), current_group);
                            k_map[i + 1][j] = -1;
                        }
                        else if(is_last_row && (k_map[0][j] == 1 || k_map[0][j] == -1)) {
                            add_back(cell, get_cell_exp(eq->var_count, 0, j), current_group);
                            k_map[0][j] = -1;
                        }
                        // Right
                        else if(!is_last_col && (k_map[i][j + 1] == 1 || k_map[i][j + 1] == -1)) {
                            add_back(cell, get_cell_exp(eq->var_count, i, j + 1), current_group);
                            k_map[i][j + 1] = -1;
                        }
                        else if(is_last_col && (k_map[i][k_size[1] + 1] == 1 || k_map[i][k_size[1] + 1] == -1)) {
                            add_back(cell, get_cell_exp(eq->var_count, 0, j), current_group);
                            k_map[0][j] = -1;
                        }
                        // Up
                        else if(!is_first_row && (k_map[i - 1][j] == 1 || k_map[i - 1][j] == -1)) {
                            add_back(cell, get_cell_exp(eq->var_count, i, k_size[1] + 1), current_group);
                            k_map[i][k_size[1] + 1] = -1;
                        }
                        else if(is_first_row && (k_map[k_size[0] - 1][j] == 1 || k_map[k_size[0] - 1][j] == -1)) {
                            add_back(cell, get_cell_exp(eq->var_count, k_size[0] - 1, j), current_group);
                            k_map[k_size[0] - 1][j] = -1;
                        }
                        // Left
                        else if(!is_first_col && (k_map[i][j - 1] == 1 || k_map[i][j - 1] == -1)) {
                            add_back(cell, get_cell_exp(eq->var_count, i, j - 1), current_group);
                            k_map[i][j - 1] = -1;
                        }
                        else if(is_first_col && (k_map[i][k_size[1] - 1] == 1 || k_map[i][k_size[1] - 1] == -1)) {
                            add_back(cell, get_cell_exp(eq->var_count, i, k_size[1] - 1), current_group);
                            k_map[i][k_size[1] - 1] = -1;
                        }
                    }
                    break;

                    case 2:         // Potential square group
                    case 3:
                    case 4:
                    break;

                    default:
                        exit_error("karnaugh_algorithm | karnaugh.c");
                };

                // Reset variables
                count = 0; is_first_row = 0; is_last_row = 0; is_first_col = 0; is_last_col = 0; full_line = 0;
                current_group++;
            }
    int group_count = current_group - 1;

    // Remove the first element used to initialize the linked list
    pop_front(&cell);

    // Simplification
    simp_exp = simplify_list(cell, group_count, eq->var_count);

    // Free linked list
    while(cell != NULL)
        pop_front(&cell);
    free(cell);

    if(simp_exp == NULL)
        return eq->bool_exp;
    else
        return simp_exp;
}

char* simplify_list(Cell* list, int group_count, int var_count) {
    if(group_count == 0)
        return NULL;
    int current_group = 1, i = 0, j = 0, l = 0;
    Cell* current = list;
    int** group_TT = (int*)malloc(pow(2, var_count) * sizeof(int*));
    char temp[3];
    char* simp_exp = (char*)malloc(100);
    while(current_group <= group_count) {
        // Build group truth table
        while(current->group_id == current_group) {
            group_TT[l++] = convert_term_to_line(var_count, current->cell_exp);
            if(current->next == NULL)
                break;
            else
                current = current->next;
        }
        // Simplify group
        if(l == 1) {
            strcat(simp_exp, current->cell_exp);                            // Single cell group
            strcat(simp_exp, "+");
        }
        else {
            for(j = 0; j < var_count; j++)
                for(i = 1; i <= l; i++) {
                    if(i == l)
                        if(group_TT[0][j] == 0) {
                            temp[0] = (char)(a_ascii + j);
                            temp[1] = '\'';
                            strcat(simp_exp, temp);
                        }
                        else {
                            temp[0] = (char)(a_ascii + j);
                            strcat(simp_exp, temp);
                        }
                    else if(group_TT[i - 1][j] != group_TT[i][j])
                        break;                                             // Exit loop if a different value is found in current variable column
                }
            strcat(simp_exp, "+");   
        }

        // Free allocated memory
        for(i = 0; i < l; i++)
            free(group_TT[i]);

        // Loop to next group
        l = 0;
        current_group++;
    }

    // Free allocated memory
    free(group_TT);

    // Replace the last '+' by \0
    simp_exp[strlen(simp_exp) - 1] = '\0';
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
void add_back(Cell* cell, char* cell_exp, int group_id) {
    Cell* current = cell;
    while (current->next != NULL) 
        current = current->next;
    current->next = (Cell*)malloc(sizeof(Cell));
    current->next->cell_exp = cell_exp;
    current->next->group_id = group_id;
    current->next->next = NULL;
}

void add_front(Cell** cell, char* cell_exp, int group_id) {
    Cell* new_cell;
    new_cell = (Cell*)malloc(sizeof(Cell));
    new_cell->cell_exp = cell_exp;
    new_cell->group_id = group_id;
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
        printf("\ncell exp = %s\tgroup_id = %i", current->cell_exp, current->group_id);
        current = current->next;
    }
    printf("\n");
}