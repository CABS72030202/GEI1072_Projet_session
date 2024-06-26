#include "../src/karnaugh.h"

Equation simplified_eq(Equation* eq) {
    // Check if the equation has less than 2 variables, indicating it's already simplified
    if(eq->var_count < 2)
        return *eq;

    // Convert to Sum of Products (SOP) if the default expression type is Product of Sums (POS)
    if(default_bool_exp_type == 2)
        eq->bool_exp = sum_of_products(eq->var_count, eq->truth_table);

    // Apply Karnaugh algorithm to simplify the equation
    char* simp_exp = karnaugh_algorithm(eq);

    // Create a simplified equation struct
    Equation simp_eq = {eq->var_count, eq->truth_table, simp_exp};

    // Check if the simplified equation is identical to the original equation
    if(!compare_eq(&simp_eq, eq)) {
        printf("\nErreur. Impossible de simplifier l'expression %s\n", eq->bool_exp);
        return *eq;
    }

    // Return the simplified equation
    return simp_eq;
}

char* karnaugh_algorithm(Equation* eq) {
    // Initialize variables
    int k_size[2] = {0,0}, i = 0, j = 0, p = 0, q = 0;
    int count = 0, full_line = 0;
    int current_group = 1;

    // Calculate Karnaugh map size
    calc_karnaugh_size(k_size, eq->var_count);      // Rows = [0] | Columns = [1]

    // Generate Karnaugh map
    int** k_map = generate_karnaugh_map(eq, k_size);

    // Initialize simplified expression and linked list
    char* simp_exp;
    Cell* cell = (Cell*)malloc(sizeof(Cell));
    cell->cell_exp = NULL;
    cell->group_id = -1;
    cell->next = NULL;

    // Loop through Karnaugh map cells
    for(i = 0; i < k_size[0]; i++) 
        for(j = 0; j < k_size[1]; j++)
            if(k_map[i][j] == 1) {
                // Check neighboring cells for grouping
                // Down
                if(k_map[relative_pos(k_size[0], i, 1)][relative_pos(k_size[1], j, 0)] == 1 || k_map[relative_pos(k_size[0], i, 1)][relative_pos(k_size[1], j, 0)] == -1)
                    count++;
                // Right
                if(k_map[relative_pos(k_size[0], i, 0)][relative_pos(k_size[1], j, 1)] == 1 || k_map[relative_pos(k_size[0], i, 0)][relative_pos(k_size[1], j, 1)] == -1)
                    count++;
                // Up
                if(k_map[relative_pos(k_size[0], i, -1)][relative_pos(k_size[1], j, 0)] == 1 || k_map[relative_pos(k_size[0], i, -1)][relative_pos(k_size[1], j, 0)] == -1)
                    count++;
                // Left
                if(k_map[relative_pos(k_size[0], i, 0)][relative_pos(k_size[1], j, -1)] == 1 || k_map[relative_pos(k_size[0], i, 0)][relative_pos(k_size[1], j, -1)] == -1)
                    count++;

                // Group cells based on count of neighboring ones
                switch(count) {
                    case 0:         // Group of 1
                        add_back(cell, get_cell_exp(eq->var_count, i, j), current_group);
                        k_map[i][j] = -1;
                    break;

                    case 1:         // Line group
    L:                  // Check for vertical full line
                        for(p = 0, full_line = 0; p < k_size[0]; p++) {
                            if(k_map[p][j] == 0)
                                break;
                            full_line++;
                        }
                        if(full_line == k_size[0]) {    // Vertical full line
                            for(p = 0; p < k_size[0]; p++) {
                                add_back(cell, get_cell_exp(eq->var_count, p, j), current_group);
                                k_map[p][j] = -1;
                            }
                            break;
                        }
                        // Check for horizontal full line
                        for(p = 0, full_line = 0; p < k_size[1]; p++) {
                            if(k_map[i][p] == 0)
                                break;
                            full_line++;
                        }
                        if(full_line == k_size[1]) {    // Horizontal full line
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
                            if(k_map[relative_pos(k_size[0], i, 1)][relative_pos(k_size[1], j, 0)] == 1 || k_map[relative_pos(k_size[0], i, 1)][relative_pos(k_size[1], j, 0)] == -1) {
                                add_back(cell, get_cell_exp(eq->var_count, relative_pos(k_size[0], i, 1), relative_pos(k_size[1], j, 0)), current_group);
                                k_map[relative_pos(k_size[0], i, 1)][relative_pos(k_size[1], j, 0)] = -1;
                            }
                            // Right
                            else if(k_map[relative_pos(k_size[0], i, 0)][relative_pos(k_size[1], j, 1)] == 1 || k_map[relative_pos(k_size[0], i, 0)][relative_pos(k_size[1], j, 1)] == -1) {
                                add_back(cell, get_cell_exp(eq->var_count, relative_pos(k_size[0], i, 0), relative_pos(k_size[1], j, 1)), current_group);
                                k_map[relative_pos(k_size[0], i, 0)][relative_pos(k_size[1], j, 1)] = -1;                       
                            }
                            // Up
                            else if(k_map[relative_pos(k_size[0], i, -1)][relative_pos(k_size[1], j, 0)] == 1 || k_map[relative_pos(k_size[0], i, -1)][relative_pos(k_size[1], j, 0)] == -1) {
                                add_back(cell, get_cell_exp(eq->var_count, relative_pos(k_size[0], i, -1), relative_pos(k_size[1], j, 0)), current_group);
                                k_map[relative_pos(k_size[0], i, -1)][relative_pos(k_size[1], j, 0)] = -1;                        
                            }
                            // Left
                            else if(k_map[relative_pos(k_size[0], i, 0)][relative_pos(k_size[1], j, -1)] == 1 || k_map[relative_pos(k_size[0], i, 0)][relative_pos(k_size[1], j, -1)] == -1) {
                                add_back(cell, get_cell_exp(eq->var_count, relative_pos(k_size[0], i, 0), relative_pos(k_size[1], j, -1)), current_group);
                                k_map[relative_pos(k_size[0], i, 0)][relative_pos(k_size[1], j, -1)] = -1;                        
                            }
                        }
                    break;

                    // Potential square group
                    case 2:
                    case 3:
                    case 4:
                        count = 0;
                        // Check diagonals for potential square grouping
                        // Down | Right
                        if(k_map[relative_pos(k_size[0], i, 1)][relative_pos(k_size[1], j, 1)] == 1 || k_map[relative_pos(k_size[0], i, 1)][relative_pos(k_size[1], j, 1)] == -1)
                            count++;
                        // Down | Left
                        if(k_map[relative_pos(k_size[0], i, 1)][relative_pos(k_size[1], j, -1)] == 1 || k_map[relative_pos(k_size[0], i, 1)][relative_pos(k_size[1], j, -1)] == -1)
                            count++;
                        // Up | Left
                        if(k_map[relative_pos(k_size[0], i, -1)][relative_pos(k_size[1], j, -1)] == 1 || k_map[relative_pos(k_size[0], i, -1)][relative_pos(k_size[1], j, -1)] == -1)
                            count++;
                        // Up | Right
                        if(k_map[relative_pos(k_size[0], i, -1)][relative_pos(k_size[1], j, 1)] == 1 || k_map[relative_pos(k_size[0], i, -1)][relative_pos(k_size[1], j, 1)] == -1)
                            count++;

                        // Determine group type and add cells to the linked list
                        switch (count) {
                        case 0:         // Line group
                            goto L;

                        case 1:
                        case 2:
                        case 3:
                        case 4:
                            // Check for right vertical rectangle
                            for(p = 0, full_line = 0; p < k_size[0]; p++) 
                                for(q = 0; q < 2; q++) {
                                    if(k_map[p][relative_pos(k_size[1], j, q)] == 0)
                                        break;
                                    full_line++;
                                }
                            if(full_line == k_size[0] * 2) {        // Right vertical rectangle
                                for(p = 0; p < k_size[0]; p++) 
                                    for(q = 0; q < 2; q++){
                                        add_back(cell, get_cell_exp(eq->var_count, p, relative_pos(k_size[1], j, q)), current_group);
                                        k_map[p][relative_pos(k_size[1], j, q)] = -1;
                                    }
                                break;
                            }
                            // Check for left vertical rectangle
                            for(p = 0, full_line = 0; p < k_size[0]; p++) 
                                for(q = -1; q < 1; q++) {
                                    if(k_map[p][relative_pos(k_size[1], j, q)] == 0)
                                        break;
                                    full_line++;
                                }
                            if(full_line == k_size[0] * 2) {        // Left vertical rectangle
                                for(p = 0; p < k_size[0]; p++)
                                    for(q = -1; q < 1; q++) {
                                        add_back(cell, get_cell_exp(eq->var_count, p, relative_pos(k_size[1], j, q)), current_group);
                                        k_map[p][relative_pos(k_size[1], j, q)] = -1;
                                    }
                                break;
                            }
                            // Check for down horizontal rectangle
                            for(p = 0, full_line = 0; p < k_size[1]; p++) 
                                for(q = 0; q < 2; q++) {
                                    if(k_map[relative_pos(k_size[0], i, q)][p] == 0)
                                        break;
                                    full_line++;
                                }
                            if(full_line == k_size[1] * 2) {        // Down horizontal rectangle
                                for(p = 0; p < k_size[0]; p++)
                                    for(q = 0; q < 2; q++) {
                                        add_back(cell, get_cell_exp(eq->var_count, relative_pos(k_size[0], i, q), p), current_group);
                                        k_map[relative_pos(k_size[0], i, q)][p] = -1;
                                    }
                                break;
                            }
                            // Check for up horizontal rectangle
                            for(p = 0, full_line = 0; p < k_size[1]; p++) 
                                for(q = -1; q < 1; q++) {
                                    if(k_map[relative_pos(k_size[0], i, q)][p] == 0)
                                        break;
                                    full_line++;
                                }
                            if(full_line == k_size[1] * 2) {        // Up horizontal rectangle
                                for(p = 0; p < k_size[0]; p++)
                                    for(q = -1; q < 1; q++) {
                                        add_back(cell, get_cell_exp(eq->var_count, relative_pos(k_size[0], i, q), p), current_group);
                                        k_map[relative_pos(k_size[0], i, q)][p] = -1;
                                    }
                                break;
                            }
                            else {                              // Group of 4                 
                                add_back(cell, get_cell_exp(eq->var_count, i, j), current_group);
                                k_map[i][j] = -1;
                                // Down | Right
                                if( (k_map[relative_pos(k_size[0], i, 1)][relative_pos(k_size[1], j, 0)] == 1 || k_map[relative_pos(k_size[0], i, 1)][relative_pos(k_size[1], j, 0)] == -1) &&
                                    (k_map[relative_pos(k_size[0], i, 0)][relative_pos(k_size[1], j, 1)] == 1 || k_map[relative_pos(k_size[0], i, 0)][relative_pos(k_size[1], j, 1)] == -1) &&
                                    (k_map[relative_pos(k_size[0], i, 1)][relative_pos(k_size[1], j, 1)] == 1 || k_map[relative_pos(k_size[0], i, 1)][relative_pos(k_size[1], j, 1)] == -1)) {
                                    add_back(cell, get_cell_exp(eq->var_count, relative_pos(k_size[0], i, 1), relative_pos(k_size[1], j, 0)), current_group);
                                    add_back(cell, get_cell_exp(eq->var_count, relative_pos(k_size[0], i, 0), relative_pos(k_size[1], j, 1)), current_group);
                                    add_back(cell, get_cell_exp(eq->var_count, relative_pos(k_size[0], i, 1), relative_pos(k_size[1], j, 1)), current_group);
                                    k_map[relative_pos(k_size[0], i, 1)][relative_pos(k_size[1], j, 0)] = -1;
                                    k_map[relative_pos(k_size[0], i, 0)][relative_pos(k_size[1], j, 1)] = -1;
                                    k_map[relative_pos(k_size[0], i, 1)][relative_pos(k_size[1], j, 1)] = -1;
                                }
                                // Down | Left
                                else if((k_map[relative_pos(k_size[0], i, 1)][relative_pos(k_size[1], j, 0)] == 1 || k_map[relative_pos(k_size[0], i, 1)][relative_pos(k_size[1], j, 0)] == -1) &&
                                        (k_map[relative_pos(k_size[0], i, 0)][relative_pos(k_size[1], j, -1)] == 1 || k_map[relative_pos(k_size[0], i, 0)][relative_pos(k_size[1], j, -1)] == -1) && 
                                        (k_map[relative_pos(k_size[0], i, 1)][relative_pos(k_size[1], j, -1)] == 1 || k_map[relative_pos(k_size[0], i, 1)][relative_pos(k_size[1], j, -1)] == -1)) {
                                    add_back(cell, get_cell_exp(eq->var_count, relative_pos(k_size[0], i, 1), relative_pos(k_size[1], j, 0)), current_group);
                                    add_back(cell, get_cell_exp(eq->var_count, relative_pos(k_size[0], i, 0), relative_pos(k_size[1], j, -1)), current_group);
                                    add_back(cell, get_cell_exp(eq->var_count, relative_pos(k_size[0], i, 1), relative_pos(k_size[1], j, -1)), current_group);
                                    k_map[relative_pos(k_size[0], i, 1)][relative_pos(k_size[1], j, 0)] = 0;                               
                                    k_map[relative_pos(k_size[0], i, 0)][relative_pos(k_size[1], j, -1)] = -1;
                                    k_map[relative_pos(k_size[0], i, 1)][relative_pos(k_size[1], j, -1)] = -1;
                                }
                                // Up | Left
                                else if((k_map[relative_pos(k_size[0], i, -1)][relative_pos(k_size[1], j, 0)] == 1 || k_map[relative_pos(k_size[0], i, -1)][relative_pos(k_size[1], j, 0)] == -1) &&
                                        (k_map[relative_pos(k_size[0], i, 0)][relative_pos(k_size[1], j, -1)] == 1 || k_map[relative_pos(k_size[0], i, 0)][relative_pos(k_size[1], j, -1)] == -1) &&
                                        (k_map[relative_pos(k_size[0], i, -1)][relative_pos(k_size[1], j, -1)] == 1 || k_map[relative_pos(k_size[0], i, -1)][relative_pos(k_size[1], j, -1)] == -1)) {
                                    add_back(cell, get_cell_exp(eq->var_count, relative_pos(k_size[0], i, -1), relative_pos(k_size[1], j, 0)), current_group);
                                    add_back(cell, get_cell_exp(eq->var_count, relative_pos(k_size[0], i, 0), relative_pos(k_size[1], j, -1)), current_group);
                                    add_back(cell, get_cell_exp(eq->var_count, relative_pos(k_size[0], i, -1), relative_pos(k_size[1], j, -1)), current_group);
                                    k_map[relative_pos(k_size[0], i, -1)][relative_pos(k_size[1], j, 0)] = -1;
                                    k_map[relative_pos(k_size[0], i, 0)][relative_pos(k_size[1], j, -1)] = -1;
                                    k_map[relative_pos(k_size[0], i, -1)][relative_pos(k_size[1], j, -1)] = -1;
                                }
                                // Up | Right
                                else if((k_map[relative_pos(k_size[0], i, -1)][relative_pos(k_size[1], j, 0)] == 1 || k_map[relative_pos(k_size[0], i, -1)][relative_pos(k_size[1], j, 0)] == -1) &&
                                        (k_map[relative_pos(k_size[0], i, 0)][relative_pos(k_size[1], j, 1)] == 1 || k_map[relative_pos(k_size[0], i, 0)][relative_pos(k_size[1], j, 1)] == -1) &&
                                        (k_map[relative_pos(k_size[0], i, -1)][relative_pos(k_size[1], j, 1)] == 1 || k_map[relative_pos(k_size[0], i, -1)][relative_pos(k_size[1], j, 1)] == -1)) {
                                    add_back(cell, get_cell_exp(eq->var_count, relative_pos(k_size[0], i, -1), relative_pos(k_size[1], j, 0)), current_group);
                                    add_back(cell, get_cell_exp(eq->var_count, relative_pos(k_size[0], i, 0), relative_pos(k_size[1], j, 1)), current_group);
                                    add_back(cell, get_cell_exp(eq->var_count, relative_pos(k_size[0], i, -1), relative_pos(k_size[1], j, 1)), current_group);
                                    k_map[relative_pos(k_size[0], i, -1)][relative_pos(k_size[1], j, 0)] = -1;
                                    k_map[relative_pos(k_size[0], i, 0)][relative_pos(k_size[1], j, 1)] = -1;
                                    k_map[relative_pos(k_size[0], i, -1)][relative_pos(k_size[1], j, 1)] = -1;
                                }
                            else        // Line group
                                goto L;
                            }
                        break;
                        
                        default:
                            exit_error("karnaugh_algorithm | karnaugh.c");
                        };
                    break;

                    default:
                        exit_error("karnaugh_algorithm | karnaugh.c");
                };

                // Reset variables
                count = 0; full_line = 0;
                current_group++;
            }
    int group_count = current_group - 1;

    // Remove the first element used to initialize the linked list
    pop_front(&cell);

    // Simplification
    simp_exp = simplify_list(cell, group_count, eq);

    // Free linked list
    while(cell != NULL)
        pop_front(&cell);
    free(cell);

    if(simp_exp == NULL)
        return eq->bool_exp;
    else
        return simp_exp;
}

char* simplify_list(Cell* list, int group_count, Equation* eq) {
    // Check if there are no groups to simplify
    if(group_count == 0)
        return NULL;

    int current_group = 1, i = 0, j = 0, l = 0;
    Cell* current = list;

    // Allocate memory for the group truth table and simplified expression
    int** group_TT = (int*)malloc(pow(2, eq->var_count) * sizeof(int*));
    char temp[3];
    char* simp_exp = (char*)malloc(required_size(eq->var_count, eq->truth_table, "POS") * sizeof(char)); // Maximum size is the current string size
    simp_exp[0] = '\0';

    // Loop through each group
    while(current_group <= group_count) {
        // Build the group truth table
        while(current->group_id == current_group) {
            group_TT[l++] = convert_term_to_line(eq->var_count, current->cell_exp);
            if(current->next == NULL)
                break;
            else
                current = current->next;
        }

        // Simplify the group
        for(j = 0; j < eq->var_count; j++)
            for(i = 1; i <= l; i++) {
                if(i == l) {
                    if(group_TT[i - 1][j] == 0) 
                        temp[1] = '\'';
                    else 
                        temp[1] = '\0';
                    
                    temp[0] = (char)(a_ascii + j);
                    strcat(simp_exp, temp);
                }
                else if(group_TT[i - 1][j] != group_TT[i][j])
                    break; // Exit the loop if a different value is found in the current variable column
            }
        strcat(simp_exp, "+");

        // Free allocated memory for the group truth table
        for(i = 0; i < l; i++)
            free(group_TT[i]);

        // Move to the next group
        l = 0;
        current_group++;
    }

    // Free allocated memory for the group truth table
    free(group_TT);

    // Replace the last '+' by '\0'
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

void print_k_map(int** k_map, int row, int col) {
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++)
        printf("%i\t", k_map[i][j]);
    printf("\n");
    }
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

int relative_pos(int size, int current, int relative) {
    int index = current + relative;
    index = current + relative;
    if(index < 0)
        return size - 1;
    if(index == size)
        return 0;
    return index;    
}   

char* get_cell_exp(int var_count, int row, int col) {
    switch(var_count) {
        case 2:
        char* var_state2[2][2] = {"A'B'","A'B",
                                "AB'","AB"};
        return var_state2[row][col];

        case 3:
        char* var_state3[2][4] = {"A'B'C'","A'B'C","A'BC","A'BC'",
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
    int i, n;
    int* vars;
    while(current != NULL) {
        n = count_var_from_BE(current->cell_exp);
        vars = convert_term_to_line(n, current->cell_exp);
        printf("\n%-8s\t", current->cell_exp);
        for (i = 0; i < n; i++)
            printf("%i\t", vars[i]);
        printf("group_id = %i", current->group_id);
        current = current->next;
    }
    printf("\n");
}