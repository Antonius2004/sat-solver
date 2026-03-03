#ifndef DPLL_H
#define DPLL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct dpll_data {
	int clauses;				// Nr of lines of matrix
	int literals;				// Nr of columns of matrix
	int **clauses_array;		// Clauses matrix
	int *literal_frequency;		// Frequency vector of the variables
} dpll_data;

int dpll(int **clauses_array, int clauses, int literals, int *t);
int dpll_r(struct dpll_data data, int *t);
int find_most_common(int *arr, int *t, int size);
int verify_clause(int *clause_array, int size, int *t);
int find_first_literal(int *t, int size);
int has_unit_clause(struct dpll_data data, int *t);

#endif // DPLL_H
