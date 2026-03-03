#include "dpll.h"

// Function to initiate the DPLL process
int dpll(int **clauses_array, int clauses, int literals, int *t) {
    struct dpll_data data;

    data.clauses = clauses;
    data.literals = literals;
    data.clauses_array = clauses_array;

	// Initialize frequency vector
	int *literal_frequency = (int *)calloc((data.literals + 1), sizeof(int));

	// Populate frequency vector
	for (int i = 0; i < data.clauses; i++)
		for (int j = 0; j < data.literals; j++)
			literal_frequency[abs(data.clauses_array[i][j])]++;

	data.literal_frequency = literal_frequency;

	// Call recursive DPLL
    int result = dpll_r(data, t);

	free(literal_frequency);
    return result;
}

// Recursive DPLL function to check satisfiability
int dpll_r(struct dpll_data data, int *t) {
	// Create a local copy of the assignment vector
    int local_t[data.literals + 1];
    memcpy(local_t, t, sizeof(int) * (data.literals + 1));

	// Check if any clause is falsified by t
	for (int i = 0; i < data.clauses; i++)
		if (!verify_clause(data.clauses_array[i], data.literals, t))
			return 0;
	
	// Unit propagation
	int l = has_unit_clause(data, t);
	while (l != 0) {
		// Add in teta in a way to satisfy the clause
		if (l > 0) {
			t[l] = 1;
		} else {
			t[-l] = 0;
		}
		// Check if any clause is falsified by t
		for (int i = 0; i < data.clauses; i++)
			if (!verify_clause(data.clauses_array[i], data.literals, t)) {
				// Restore teta for backtracking
				memcpy(t, local_t, sizeof(int) * (data.literals + 1));
				return 0;
			}
		l = has_unit_clause(data, t);
	}

	// Check if t contains a literal for every variable
	int c = 0;
	for (int i = 1; i <= data.literals; i++) {
		if (t[i] != -1)
			c++;
	}
	if (c == data.literals)
		return 1;

	// Recursive call
    int first_literal = find_most_common(data.literal_frequency, t, data.literals);

	t[first_literal] = 1;
	if (dpll_r(data, t)) return 1;

	t[first_literal] = 0;
	if (dpll_r(data, t)) return 1;

	// Restore t for backtracking
	memcpy(t, local_t, sizeof(int) * (data.literals + 1));
	return 0;
}

int find_most_common(int *arr, int *t, int size) {
	int most_common = -1;
	int max_appearances = -1;

	for (int i = 1; i <= size; i++) {
		if (arr[i] > max_appearances && t[i] == -1) {
			max_appearances = arr[i];
			most_common = i;
		}
	}

	return most_common;
}

int verify_clause(int *clause_array, int size, int *t) {
	for (int i = 0; clause_array[i] != 0 && i < size; i++) {
		int value = t[abs(clause_array[i])];
		if (value == -1)
			// If it's not assigned yet, it still might be SAT
			return 1;

		// Flip value for calculating formula if var is falsified
		if (value == 0 && clause_array[i] < 0 || value == 1 && clause_array[i] > 0)
			value = 1;
		else if (value == 0 && clause_array[i] > 0 || value == 1 && clause_array[i] < 0)
			value = 0;

		if (value == 1)
			// If one value in clause is true it is all true
			return 1;
	}

	return 0;
}

int find_first_literal(int *t, int size) {
	// First suitable literal to assign
	for (int i = 1; i <= size; i++) {
		if (t[i] == -1)
			return i;
	}

	return -1;
}

int has_unit_clause(struct dpll_data data, int *t) {
	// Return index of assignment that MUST be TRUE or 0 if it doesn't exist
	int j, ret = 0;

	for (int i = 0; i < data.clauses; i++) {
		int false_counter = 0;

		int unassigned_counter = 0;

		for (j = 0; data.clauses_array[i][j] != 0 && j < data.literals; j++) {
			int value = t[abs(data.clauses_array[i][j])];

			if (value == -1) {
				unassigned_counter++;
				ret = data.clauses_array[i][j];
			}
			else if (value == 0 && data.clauses_array[i][j] > 0 || value == 1 && data.clauses_array[i][j] < 0)
				false_counter++;
		}

		if (false_counter + unassigned_counter == j && unassigned_counter == 1)
			return ret;
	}
	return 0;
}
