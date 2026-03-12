# z3-cpp-test

A collection of C++ practice problems solved using the [Z3 Theorem Prover](https://github.com/Z3Prover/z3) from Microsoft Research.

## Prerequisites

- CMake ≥ 3.14
- A C++17 compiler (GCC, Clang, MSVC)
- Z3 development libraries (`libz3-dev` on Debian/Ubuntu)

```bash
sudo apt install libz3-dev cmake build-essential
```

## Build

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j4
```

## Practice Questions

### Q1 – Linear Arithmetic (`q1_linear_arithmetic`)

Solve systems of linear equations and inequalities over reals and integers.

- **1a**: Solve `x + y = 10`, `2x - y = 5` over the reals.
- **1b**: Find integers `x, y, z` satisfying range bounds, a sum constraint, and a linear relationship.

```bash
./q1_linear_arithmetic
```

---

### Q2 – Boolean SAT & Logic Puzzles (`q2_boolean_sat`)

Check satisfiability of propositional formulas and solve logic puzzles.

- **2a**: Find a satisfying assignment for `(p ∨ q) ∧ (¬p ∨ r) ∧ (¬q ∨ ¬r)`.
- **2b**: Verify the contrapositive law `(p ⇒ q) ⇒ (¬q ⇒ ¬p)` is a tautology.
- **2c**: Solve the cryptarithmetic puzzle **SEND + MORE = MONEY**.

```bash
./q2_boolean_sat
```

---

### Q3 – Sudoku Solver (`q3_sudoku`)

Model a 9×9 Sudoku as a constraint satisfaction problem and solve it.  
Constraints: each cell in 1–9; all-different in every row, column, and 3×3 box.

```bash
./q3_sudoku
```

---

### Q4 – N-Queens Problem (`q4_nqueens`)

Place N queens on an N×N chessboard so that no two queens threaten each other.

- **4a**: Find one solution for N = 8.
- **4b**: Count all solutions for N = 6 (expected: 4).

```bash
./q4_nqueens
```

---

### Q5 – Optimization (`q5_optimization`)

Use Z3's `optimize` API to maximize or minimize objective functions.

- **5a**: Maximize `3x + 5y` subject to linear integer constraints.
- **5b**: Assign 3 tasks to 2 machines to minimize the makespan (schedule length).

```bash
./q5_optimization
```

---

### Q6 – Bitvector Arithmetic (`q6_bitvectors`)

Explore fixed-width machine-level integer arithmetic.

- **6a**: Find an 8-bit value `x` such that `(x × 7) mod 256 = 42`.
- **6b**: Find 8-bit values whose unsigned addition overflows.
- **6c**: Formally verify the 32-bit absolute-value bit trick `abs(x) = (x ^ (x >> 31)) − (x >> 31)`.
- **6d**: Enumerate 16-bit powers of 2 using the property `x & (x−1) = 0`.

```bash
./q6_bitvectors
```
