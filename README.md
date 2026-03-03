# CNF SAT solver

A DPLL-based SAT solver for CNF formulas, written in C.

## Building

```bash
make
```

This produces a binary called `main` in the project root.

## Running

```bash
./main <input.cnf> <output.txt>
```

Or with the Makefile helper:

```bash
make run INPUT=tests/in/assignment/5_four_clauses_sat.dimacs OUTPUT=out.txt
```

## Input format

The solver reads DIMACS CNF format. Lines starting with `c` are comments and
are ignored. The problem line has the form:

```
p cnf <num_variables> <num_clauses>
```

Each clause is a space-separated list of integers ending with `0`. Positive
integers are positive literals, negative integers are negated literals.

Example — a formula with 3 variables and 2 clauses:

```
c example
p cnf 3 2
1 -2 3 0
-1 2 0
```

Test cases in various sizes are in `tests/in/`.

## Output format

```
s SATISFIABLE
v 1 -2 3 0
```

or

```
s UNSATISFIABLE
```

When satisfiable, the `v` line lists the variable assignments. A positive number
means that variable is `TRUE`, a negative number means it is `FALSE`.

## How it works

The core is a recursive DPLL implementation in `dpll.c`.

At each call, the solver first runs unit propagation: if a clause has only one
unassigned literal left, that literal is forced to the value that satisfies the
clause. This repeats until no unit clauses remain.

If the formula is not yet determined, the solver picks the variable that appears
most often across all clauses and tries assigning it `TRUE` first, then `FALSE` if
that branch fails. Each recursive call gets its own copy of the assignment array
so backtracking is just returning `0`.

The assignment array `t` uses three states:

| value | meaning |
|-------|---------|
| `-1`  | unassigned |
| `0`   | FALSE |
| `1`   | TRUE |

## Files

| file | purpose |
|------|---------|
| `main.c` | argument parsing, input reading, output writing |
| `dpll.c` | solver implementation |
| `dpll.h` | function and struct declarations |
| `Makefile` | build rules |