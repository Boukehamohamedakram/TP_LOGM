/*
 * Advanced Propositional Logic Solver
 * Implements an enhanced resolution-based algorithm
 */

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// Console color definitions
#define DEFAULT_COLOR 7
#define ERROR_COLOR 12
#define SUCCESS_COLOR 10
#define WARNING_COLOR 14
#define INFO_COLOR 9
#define HIGHLIGHT_COLOR 13
#define DEBUG_COLOR 11

// Core constants
#define MAX_SYMBOL_LEN 64
#define BASE_SIZE 100
#define EXPANSION_RATE 2
#define MAX_BUFFER 1024

// Propositional logic element structures
typedef struct {
  char symbol[MAX_SYMBOL_LEN];
} PropositionSymbol;

typedef struct {
  PropositionSymbol symbol;
  bool is_negated;
} PropositionTerm;

typedef struct {
  PropositionTerm *terms;
  int term_count;
  int max_terms;
} PropositionClause;

typedef struct {
  PropositionClause *clauses;
  int clause_count;
  int max_clauses;
  PropositionSymbol *symbols;
  int symbol_count;
  int max_symbols;
} PropositionalFormula;

// Display utilities
void set_display_color(int color_code) {
  HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(console, color_code);
}

void display_header() {
  set_display_color(HIGHLIGHT_COLOR);
  printf("\n");
  printf("╔════════════════════════════════════════════════════════╗\n");
  printf("║           Propositional Logic Solver v2.0              ║\n");
  printf("╚════════════════════════════════════════════════════════╝\n");

  set_display_color(INFO_COLOR);
  printf("\n╔════════════════════════════════════════════════════════╗\n");
  printf("║                   Project Team:                        ║\n");
  set_display_color(SUCCESS_COLOR);
  printf("║           LASFER  •  MOUSLIM  •  HECHEHOUCHE         ║\n");
  set_display_color(INFO_COLOR);
  printf("╚════════════════════════════════════════════════════════╝\n");

  set_display_color(WARNING_COLOR);
  printf("\n              ESI-EX-INI Project - 2024                \n");
  printf("               Advanced Logic Project                   \n\n");
  set_display_color(DEFAULT_COLOR);
}

// Function to initialize a proposition symbol
void init_symbol(PropositionSymbol *symbol) { symbol->symbol[0] = '\0'; }

// Function to compare two proposition symbols
bool symbols_equal(PropositionSymbol *s1, PropositionSymbol *s2) {
  return strcmp(s1->symbol, s2->symbol) == 0;
}

// Function to initialize a proposition clause
bool init_clause(PropositionClause *clause) {
  clause->terms = malloc(BASE_SIZE * sizeof(PropositionTerm));
  if (!clause->terms)
    return false;
  clause->term_count = 0;
  clause->max_terms = BASE_SIZE;
  return true;
}

// Function to free a clause
void free_clause(PropositionClause *clause) {
  free(clause->terms);
  clause->terms = NULL;
  clause->term_count = 0;
  clause->max_terms = 0;
}

// Function to initialize a formula
bool init_formula(PropositionalFormula *formula) {
  formula->clauses = malloc(BASE_SIZE * sizeof(PropositionClause));
  formula->symbols = malloc(BASE_SIZE * sizeof(PropositionSymbol));
  if (!formula->clauses || !formula->symbols) {
    free(formula->clauses);
    free(formula->symbols);
    return false;
  }
  formula->clause_count = 0;
  formula->max_clauses = BASE_SIZE;
  formula->symbol_count = 0;
  formula->max_symbols = BASE_SIZE;
  return true;
}

// Function to free a formula
void free_formula(PropositionalFormula *formula) {
  for (int i = 0; i < formula->clause_count; i++) {
    free_clause(&formula->clauses[i]);
  }
  free(formula->clauses);
  free(formula->symbols);
  formula->clauses = NULL;
  formula->symbols = NULL;
  formula->clause_count = 0;
  formula->max_clauses = 0;
  formula->symbol_count = 0;
  formula->max_symbols = 0;
}

// Function to find or add a symbol to the formula
int find_or_add_symbol(PropositionalFormula *formula, const char *symbol) {
  // First try to find the symbol
  for (int i = 0; i < formula->symbol_count; i++) {
    if (strcmp(formula->symbols[i].symbol, symbol) == 0) {
      return i;
    }
  }

  // If not found, add it
  if (formula->symbol_count >= formula->max_symbols) {
    int new_capacity = formula->max_symbols * EXPANSION_RATE;
    PropositionSymbol *new_symbols =
        realloc(formula->symbols, new_capacity * sizeof(PropositionSymbol));
    if (!new_symbols)
      return -1;
    formula->symbols = new_symbols;
    formula->max_symbols = new_capacity;
  }

  strncpy(formula->symbols[formula->symbol_count].symbol, symbol,
          MAX_SYMBOL_LEN - 1);
  formula->symbols[formula->symbol_count].symbol[MAX_SYMBOL_LEN - 1] = '\0';
  return formula->symbol_count++;
}

// Function to add a term to a clause
bool add_term(PropositionClause *clause, const char *symbol_name,
              bool is_negated) {
  if (clause->term_count >= clause->max_terms) {
    int new_capacity = clause->max_terms * EXPANSION_RATE;
    PropositionTerm *new_terms =
        realloc(clause->terms, new_capacity * sizeof(PropositionTerm));
    if (!new_terms)
      return false;
    clause->terms = new_terms;
    clause->max_terms = new_capacity;
  }

  strncpy(clause->terms[clause->term_count].symbol.symbol, symbol_name,
          MAX_SYMBOL_LEN - 1);
  clause->terms[clause->term_count].symbol.symbol[MAX_SYMBOL_LEN - 1] = '\0';
  clause->terms[clause->term_count].is_negated = is_negated;
  clause->term_count++;
  return true;
}

// Function to check if a clause contains a term
bool clause_contains(PropositionClause *clause, const char *symbol_name,
                     bool is_negated) {
  for (int i = 0; i < clause->term_count; i++) {
    if (strcmp(clause->terms[i].symbol.symbol, symbol_name) == 0 &&
        clause->terms[i].is_negated == is_negated) {
      return true;
    }
  }
  return false;
}

// Function to check if a clause is a tautology (contains both p and ¬p)
bool is_tautology(PropositionClause *clause) {
  for (int i = 0; i < clause->term_count; i++) {
    const char *symbol = clause->terms[i].symbol.symbol;
    bool is_negated = clause->terms[i].is_negated;
    // Check if the opposite polarity exists in the clause
    if (clause_contains(clause, symbol, !is_negated)) {
      return true;
    }
  }
  return false;
}

// Function to check if a clause is empty (contradiction)
bool is_empty_clause(PropositionClause *clause) {
  return clause->term_count == 0;
}

// Function to check if two clauses are identical
bool clauses_equal(PropositionClause *c1, PropositionClause *c2) {
  if (c1->term_count != c2->term_count) {
    return false;
  }

  // Check if every term in c1 is in c2
  for (int i = 0; i < c1->term_count; i++) {
    if (!clause_contains(c2, c1->terms[i].symbol.symbol,
                         c1->terms[i].is_negated)) {
      return false;
    }
  }

  // Check if every term in c2 is in c1
  for (int i = 0; i < c2->term_count; i++) {
    if (!clause_contains(c1, c2->terms[i].symbol.symbol,
                         c2->terms[i].is_negated)) {
      return false;
    }
  }

  return true;
}

// Function to check if a formula already contains a clause
bool formula_contains(PropositionalFormula *formula,
                      PropositionClause *clause) {
  for (int i = 0; i < formula->clause_count; i++) {
    if (clauses_equal(&formula->clauses[i], clause)) {
      return true;
    }
  }
  return false;
}

// Function to resolve two clauses on a symbol
bool resolve(PropositionClause *c1, PropositionClause *c2,
             const char *symbol_name, bool is_negated,
             PropositionClause *result) {
  init_clause(result);

  // Add terms from c1 except symbol
  for (int i = 0; i < c1->term_count; i++) {
    if (strcmp(c1->terms[i].symbol.symbol, symbol_name) != 0 ||
        c1->terms[i].is_negated != is_negated) {
      add_term(result, c1->terms[i].symbol.symbol, c1->terms[i].is_negated);
    }
  }

  // Add terms from c2 except -symbol
  for (int i = 0; i < c2->term_count; i++) {
    if (strcmp(c2->terms[i].symbol.symbol, symbol_name) != 0 ||
        c2->terms[i].is_negated != !is_negated) {
      add_term(result, c2->terms[i].symbol.symbol, c2->terms[i].is_negated);
    }
  }

  // Check if result is a tautology
  if (is_tautology(result)) {
    return false; // Don't use tautologies
  }

  return true;
}

// Function to check if a string is a valid symbol name
bool is_valid_symbol_name(const char *name) {
  if (!name || !*name)
    return false;

  // First character must be a letter or underscore
  if (!isalpha(name[0]) && name[0] != '_')
    return false;

  // Rest can be letters, numbers, or underscores
  for (int i = 1; name[i]; i++) {
    if (!isalnum(name[i]) && name[i] != '_')
      return false;
  }

  return true;
}

// Function to copy a clause properly
bool copy_clause(PropositionClause *dest, PropositionClause *src) {
  if (!init_clause(dest))
    return false;

  for (int i = 0; i < src->term_count; i++) {
    if (!add_term(dest, src->terms[i].symbol.symbol,
                  src->terms[i].is_negated)) {
      free_clause(dest);
      return false;
    }
  }
  return true;
}

// Function to add a clause to a formula
bool add_clause(PropositionalFormula *formula, PropositionClause *clause) {
  if (formula->clause_count >= formula->max_clauses) {
    int new_capacity = formula->max_clauses * EXPANSION_RATE;
    PropositionClause *new_clauses =
        realloc(formula->clauses, new_capacity * sizeof(PropositionClause));
    if (!new_clauses)
      return false;
    formula->clauses = new_clauses;
    formula->max_clauses = new_capacity;
  }

  // Make a proper copy of the clause
  if (!copy_clause(&formula->clauses[formula->clause_count], clause)) {
    return false;
  }
  formula->clause_count++;
  return true;
}

// Function to perform unit propagation
bool unit_propagation(PropositionalFormula *formula) {
  bool changes_made;
  do {
    changes_made = false;
    for (int i = 0; i < formula->clause_count; i++) {
      if (formula->clauses[i].term_count == 1) {
        // Found a unit clause
        PropositionTerm unit = formula->clauses[i].terms[0];

        // Propagate through other clauses
        for (int j = 0; j < formula->clause_count; j++) {
          if (i == j)
            continue;

          PropositionClause *clause = &formula->clauses[j];
          for (int k = 0; k < clause->term_count; k++) {
            if (strcmp(clause->terms[k].symbol.symbol, unit.symbol.symbol) ==
                0) {
              if (clause->terms[k].is_negated == unit.is_negated) {
                // Remove this clause (it's satisfied)
                formula->clauses[j] =
                    formula->clauses[formula->clause_count - 1];
                formula->clause_count--;
                j--;
                changes_made = true;
                break;
              } else {
                // Remove this term
                clause->terms[k] = clause->terms[clause->term_count - 1];
                clause->term_count--;
                k--;
                changes_made = true;
              }
            }
          }
        }
      }
    }
  } while (changes_made);

  return true;
}

// Function to perform resolution by refutation
bool resolution(PropositionalFormula *formula) {
  // Create a working set of clauses
  PropositionClause *work_clauses =
      malloc(BASE_SIZE * sizeof(PropositionClause));
  int work_capacity = BASE_SIZE;
  int work_size = 0;

  if (!work_clauses)
    return true; // Memory error, assume satisfiable

  // Copy original clauses to working set
  for (int i = 0; i < formula->clause_count; i++) {
    if (work_size >= work_capacity) {
      int new_capacity = work_capacity * EXPANSION_RATE;
      PropositionClause *new_work =
          realloc(work_clauses, new_capacity * sizeof(PropositionClause));
      if (!new_work) {
        // Clean up and return
        for (int j = 0; j < work_size; j++) {
          free_clause(&work_clauses[j]);
        }
        free(work_clauses);
        return true; // Memory error, assume satisfiable
      }
      work_clauses = new_work;
      work_capacity = new_capacity;
    }

    if (!copy_clause(&work_clauses[work_size], &formula->clauses[i])) {
      // Clean up and return
      for (int j = 0; j < work_size; j++) {
        free_clause(&work_clauses[j]);
      }
      free(work_clauses);
      return true; // Memory error, assume satisfiable
    }
    work_size++;
  }

  // Perform resolution
  bool found_empty = false;
  int start = 0;

  while (start < work_size && !found_empty) {
    int end = work_size;

    for (int i = 0; i < end && !found_empty; i++) {
      for (int j = i + 1; j < end && !found_empty; j++) {
        // Try each term in clause i
        for (int k = 0; k < work_clauses[i].term_count; k++) {
          const char *symbol = work_clauses[i].terms[k].symbol.symbol;
          bool is_negated = work_clauses[i].terms[k].is_negated;

          // Look for complementary term
          if (clause_contains(&work_clauses[j], symbol, !is_negated)) {
            PropositionClause resolvant;
            if (!init_clause(&resolvant))
              continue;

            if (resolve(&work_clauses[i], &work_clauses[j], symbol, is_negated,
                        &resolvant)) {
              if (is_empty_clause(&resolvant)) {
                found_empty = true;
                free_clause(&resolvant);
                break;
              }

              // Check if this is a new clause
              bool is_new = true;
              for (int m = 0; m < work_size; m++) {
                if (clauses_equal(&work_clauses[m], &resolvant)) {
                  is_new = false;
                  break;
                }
              }

              if (is_new) {
                if (work_size >= work_capacity) {
                  int new_capacity = work_capacity * EXPANSION_RATE;
                  PropositionClause *new_work = realloc(
                      work_clauses, new_capacity * sizeof(PropositionClause));
                  if (!new_work) {
                    free_clause(&resolvant);
                    continue;
                  }
                  work_clauses = new_work;
                  work_capacity = new_capacity;
                }
                work_clauses[work_size++] = resolvant;
              } else {
                free_clause(&resolvant);
              }
            }
          }
        }
      }
    }
    start = end;
  }

  // Clean up
  for (int i = 0; i < work_size; i++) {
    free_clause(&work_clauses[i]);
  }
  free(work_clauses);

  return !found_empty;
}

// Function to read a formula from a file
bool read_formula_from_file(const char *filename,
                            PropositionalFormula *formula) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    printf("Error: Unable to open file %s\n", filename);
    return false;
  }

  if (!init_formula(formula)) {
    printf("Error: Failed to initialize formula\n");
    fclose(file);
    return false;
  }

  char line[MAX_BUFFER];
  int line_num = 0;

  while (fgets(line, sizeof(line), file)) {
    line_num++;

    // Remove trailing newline and whitespace
    char *end = line + strlen(line) - 1;
    while (end >= line && isspace(*end))
      *end-- = '\0';

    // Skip empty lines and comments
    if (line[0] == '\0' || line[0] == '#')
      continue;

    PropositionClause clause;
    if (!init_clause(&clause)) {
      free_formula(formula);
      fclose(file);
      return false;
    }

    char *token = strtok(line, " \t");
    while (token) {
      bool is_negated = (token[0] == '!');
      char *symbol_name = token + (is_negated ? 1 : 0);

      if (!is_valid_symbol_name(symbol_name)) {
        free_clause(&clause);
        free_formula(formula);
        fclose(file);
        return false;
      }

      if (!add_term(&clause, symbol_name, is_negated)) {
        free_clause(&clause);
        free_formula(formula);
        fclose(file);
        return false;
      }

      token = strtok(NULL, " \t");
    }

    if (clause.term_count > 0 && !is_tautology(&clause)) {
      if (!add_clause(formula, &clause)) {
        free_clause(&clause);
        free_formula(formula);
        fclose(file);
        return false;
      }
    }
    free_clause(&clause);
  }

  fclose(file);
  return true;
}

// Main function
int main(int argc, char *argv[]) {
  display_header();

  if (argc != 2) {
    set_display_color(ERROR_COLOR);
    printf("Error: Please provide a CNF file path\n");
    printf("Usage: %s <input_file.cnf>\n", argv[0]);
    set_display_color(DEFAULT_COLOR);
    return 1;
  }

  PropositionalFormula formula;
  if (!init_formula(&formula)) {
    set_display_color(ERROR_COLOR);
    printf("Error: Failed to initialize formula\n");
    set_display_color(DEFAULT_COLOR);
    return 1;
  }

  if (!read_formula_from_file(argv[1], &formula)) {
    set_display_color(ERROR_COLOR);
    printf("Error: Failed to read formula from file\n");
    set_display_color(DEFAULT_COLOR);
    free_formula(&formula);
    return 1;
  }

  set_display_color(INFO_COLOR);
  printf("\nAnalyzing formula...\n");
  set_display_color(DEFAULT_COLOR);

  if (resolution(&formula)) {
    set_display_color(SUCCESS_COLOR);
    printf("\nResult: Formula is SATISFIABLE\n");
  } else {
    set_display_color(ERROR_COLOR);
    printf("\nResult: Formula is UNSATISFIABLE\n");
  }
  set_display_color(DEFAULT_COLOR);

  free_formula(&formula);
  return 0;
}
