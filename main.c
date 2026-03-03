#include "dpll.h"

int main(int argc, char *argv[]) {
	// Open input file passed in command line
	(void)argc;
	char *input_file_path = argv[1];
	char *output_file_path = argv[2];
	FILE *input_file = fopen(input_file_path, "r");
	FILE *output_file = fopen(output_file_path, "w");

	// Read input
	char buff[256];
	int literals, clauses;

	// Get first line
	fgets(buff, 256, input_file);
	// Ignore comments
	while (buff[0] == 'c')
		fgets(buff, 256, input_file);
	buff[strcspn(buff, "\n")] = '\0';
	// Take each word
	char *word = strtok(buff, " ");
	word = strtok(NULL, " ");
	// Got over the formalities
	// Get number of literals
	word = strtok(NULL, " ");
	literals = atoi(word);
	// Get number of clauses
	word = strtok(NULL, " ");
	clauses = atoi(word);

	// Initialzie clauses_array[clauses][literals];
	int **clauses_array = (int **)malloc(clauses * sizeof(int *));
	for (int i = 0; i < clauses; i++)
		clauses_array[i] = (int *)malloc(literals * sizeof(int));

	// Initialize array
	for (int i = 0; i < clauses; i++) {
		for (int j = 0; j < literals; j++) {
			clauses_array[i][j] = 0;
		}
	}

	// Populate matrix
	for (int i = 0; i < clauses; i++) {
		int curr_var = 0;
		fgets(buff, 256, input_file);
		buff[strcspn(buff, "\n")] = '\0';
		char *word = strtok(buff, " ");
		while (word[0] != '0') {
			int var = atoi(word);
			clauses_array[i][curr_var] = var;
			curr_var++;
			word = strtok(NULL, " ");
		}
	}
	
	// Initialize assignment array here with -1
    int *t = (int *)malloc((literals + 1) * sizeof(int));
    for (int i = 0; i <= literals; i++) {
        t[i] = -1;
    }

	// Call DPLL
	int result = dpll(clauses_array, clauses, literals, t);

	// Format output
	if (result == 1) {
		fprintf(output_file, "s SATISFIABLE\n");
		fprintf(output_file, "v ");
		for (int i = 1; i <= literals; i++) {
			if (t[i] == 0) {
				fprintf(output_file, "-%d ", i);
			} else {
				// Any unassigned literal comes out as TRUE
				fprintf(output_file, "%d ", i);
			}
		}
	} else {
		fprintf(output_file, "s UNSATISFIABLE\n");
	}

	// Close files
	fclose(input_file);
	fclose(output_file);

	// Free memory
	free(t);
	for (int i = 0; i < clauses; i++)
		free(clauses_array[i]);
	free(clauses_array);

	return 0;
}
