/*
 * Advanced Propositional Logic Formula Generator
 * Creates random CNF formulas for testing
 */

#include <stdbool.h>
#include <stddef.h> // for NULL
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


// Configuration constants
#define MAX_VARIABLES 26
#define MAX_CLAUSES 100
#define MAX_LITERALS_PER_CLAUSE 3
#define MAX_SYMBOL_LEN 64

// Structure for formula generation
typedef struct {
  int num_variables;
  int num_clauses;
  int literals_per_clause;
  bool allow_tautologies;
} GeneratorConfig;

// Function to initialize random seed
void init_random() { srand((unsigned int)time(NULL)); }

// Function to generate a random variable name
void generate_symbol_name(char *name, int index) {
  name[0] = 'p' + (index % 26);
  name[1] = '\0';
}

// Function to generate a random clause
void generate_clause(char *clause, int max_literals, int num_variables) {
  int num_literals = (rand() % max_literals) + 1;
  int pos = 0;

  for (int i = 0; i < num_literals; i++) {
    if (i > 0) {
      clause[pos++] = ' ';
    }

    // Randomly negate the literal
    if (rand() % 2) {
      clause[pos++] = '!';
    }

    // Generate variable name
    int var_index = rand() % num_variables;
    generate_symbol_name(clause + pos, var_index);
    pos++;
  }

  clause[pos] = '\0';
}

// Function to generate a complete formula
void generate_formula(const GeneratorConfig *config) {
  printf("c Generated formula with %d variables and %d clauses\n",
         config->num_variables, config->num_clauses);
  printf("p cnf %d %d\n", config->num_variables, config->num_clauses);

  char clause[MAX_SYMBOL_LEN * MAX_LITERALS_PER_CLAUSE];
  for (int i = 0; i < config->num_clauses; i++) {
    generate_clause(clause, config->literals_per_clause, config->num_variables);
    printf("%s\n", clause);
  }
}

// Function to parse command line arguments
bool parse_args(int argc, char *argv[], GeneratorConfig *config) {
  if (argc < 3) {
    printf("Usage: %s <num_variables> <num_clauses> [literals_per_clause] "
           "[allow_tautologies]\n",
           argv[0]);
    return false;
  }

  config->num_variables = atoi(argv[1]);
  config->num_clauses = atoi(argv[2]);
  config->literals_per_clause =
      (argc > 3) ? atoi(argv[3]) : MAX_LITERALS_PER_CLAUSE;
  config->allow_tautologies =
      (argc > 4) ? (strcmp(argv[4], "true") == 0) : false;

  if (config->num_variables <= 0 || config->num_variables > MAX_VARIABLES ||
      config->num_clauses <= 0 || config->num_clauses > MAX_CLAUSES ||
      config->literals_per_clause <= 0 ||
      config->literals_per_clause > MAX_LITERALS_PER_CLAUSE) {
    printf("Invalid parameters. Please check the ranges:\n");
    printf("Variables: 1-%d\n", MAX_VARIABLES);
    printf("Clauses: 1-%d\n", MAX_CLAUSES);
    printf("Literals per clause: 1-%d\n", MAX_LITERALS_PER_CLAUSE);
    return false;
  }

  return true;
}

int main(int argc, char *argv[]) {
  GeneratorConfig config;

  if (!parse_args(argc, argv, &config)) {
    return 1;
  }

  init_random();
  generate_formula(&config);

  return 0;
}