/**
 * Q4: N-Queens Problem with Z3
 *
 * Practice: Place N queens on an NxN chessboard so that no two queens
 * threaten each other (no two queens in the same row, column, or diagonal).
 *
 * The variable queens[i] represents the column (0-indexed) of the queen
 * placed in row i.
 *
 * Problem 4a: Find one solution for N=8
 * Problem 4b: Count all solutions for N=6
 */

#include <z3++.h>
#include <iostream>
#include <vector>

using namespace z3;

// Add N-queens constraints for an N-queens problem.
// queens[i] = column of queen in row i, so rows are automatically distinct.
void add_nqueens_constraints(solver& s, const std::vector<expr>& queens, int N) {
    context& ctx = queens[0].ctx();

    for (int i = 0; i < N; i++) {
        // Column bounds
        s.add(queens[i] >= 0 && queens[i] < N);
    }

    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            // No two queens in the same column
            s.add(queens[i] != queens[j]);
            // No two queens on the same diagonal
            s.add(queens[i] - queens[j] != ctx.int_val(i - j));
            s.add(queens[i] - queens[j] != ctx.int_val(j - i));
        }
    }
}

void print_board(const std::vector<int>& queens, int N) {
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            std::cout << (queens[r] == c ? " Q" : " .");
        }
        std::cout << std::endl;
    }
}

void problem_4a() {
    int N = 8;
    std::cout << "=== Problem 4a: One solution for " << N << "-Queens ===" << std::endl;

    context ctx;
    solver s(ctx);

    std::vector<expr> queens;
    for (int i = 0; i < N; i++) {
        queens.push_back(ctx.int_const(("q" + std::to_string(i)).c_str()));
    }

    add_nqueens_constraints(s, queens, N);

    if (s.check() == sat) {
        model m = s.get_model();
        std::vector<int> sol(N);
        for (int i = 0; i < N; i++) {
            sol[i] = m.eval(queens[i]).as_int64();
        }
        std::cout << "Solution (Q = queen, . = empty):" << std::endl;
        print_board(sol, N);
        std::cout << "Column positions (row 0..7): ";
        for (int i = 0; i < N; i++) std::cout << sol[i] << " ";
        std::cout << std::endl;
    } else {
        std::cout << "No solution found." << std::endl;
    }
    std::cout << std::endl;
}

void problem_4b() {
    int N = 6;
    std::cout << "=== Problem 4b: Count all solutions for " << N << "-Queens ===" << std::endl;

    context ctx;
    solver s(ctx);

    std::vector<expr> queens;
    for (int i = 0; i < N; i++) {
        queens.push_back(ctx.int_const(("q" + std::to_string(i)).c_str()));
    }

    add_nqueens_constraints(s, queens, N);

    int count = 0;
    while (s.check() == sat) {
        model m = s.get_model();
        count++;

        // Block this solution by requiring at least one queen to be different
        expr block = ctx.bool_val(false);
        for (int i = 0; i < N; i++) {
            block = block || (queens[i] != m.eval(queens[i]));
        }
        s.add(block);
    }

    std::cout << "Total solutions for " << N << "-Queens: " << count << std::endl;
    std::cout << "(Expected: 4)" << std::endl;
    std::cout << std::endl;
}

int main() {
    std::cout << "Z3 Practice - Q4: N-Queens Problem" << std::endl;
    std::cout << "===================================\n" << std::endl;
    problem_4a();
    problem_4b();
    return 0;
}
