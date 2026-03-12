/**
 * Q3: Sudoku Solver with Z3
 *
 * Practice: Model a 9x9 Sudoku puzzle as a Z3 constraint satisfaction problem.
 *
 * Rules:
 *   - Each cell contains a digit 1-9
 *   - Each row contains all digits 1-9 (no repeats)
 *   - Each column contains all digits 1-9 (no repeats)
 *   - Each 3x3 box contains all digits 1-9 (no repeats)
 *
 * The puzzle uses 0 to represent an empty cell.
 */

#include <z3++.h>
#include <iostream>
#include <array>

using namespace z3;

// A moderately difficult Sudoku puzzle (0 = empty)
const std::array<std::array<int,9>,9> PUZZLE = {{
    {{5, 3, 0, 0, 7, 0, 0, 0, 0}},
    {{6, 0, 0, 1, 9, 5, 0, 0, 0}},
    {{0, 9, 8, 0, 0, 0, 0, 6, 0}},
    {{8, 0, 0, 0, 6, 0, 0, 0, 3}},
    {{4, 0, 0, 8, 0, 3, 0, 0, 1}},
    {{7, 0, 0, 0, 2, 0, 0, 0, 6}},
    {{0, 6, 0, 0, 0, 0, 2, 8, 0}},
    {{0, 0, 0, 4, 1, 9, 0, 0, 5}},
    {{0, 0, 0, 0, 8, 0, 0, 7, 9}}
}};

void print_grid(const std::array<std::array<int,9>,9>& grid) {
    for (int r = 0; r < 9; r++) {
        if (r % 3 == 0 && r != 0) std::cout << "------+-------+------" << std::endl;
        for (int c = 0; c < 9; c++) {
            if (c % 3 == 0 && c != 0) std::cout << " |";
            if (grid[r][c] == 0)
                std::cout << " .";
            else
                std::cout << " " << grid[r][c];
        }
        std::cout << std::endl;
    }
}

int main() {
    std::cout << "Z3 Practice - Q3: Sudoku Solver" << std::endl;
    std::cout << "================================\n" << std::endl;

    std::cout << "Puzzle:" << std::endl;
    print_grid(PUZZLE);
    std::cout << std::endl;

    context ctx;
    solver s(ctx);

    // Create 9x9 grid of integer variables
    std::array<std::array<expr,9>,9> cells = {
        std::array<expr,9>{ctx.int_const("c00"), ctx.int_const("c01"), ctx.int_const("c02"),
                           ctx.int_const("c03"), ctx.int_const("c04"), ctx.int_const("c05"),
                           ctx.int_const("c06"), ctx.int_const("c07"), ctx.int_const("c08")},
        std::array<expr,9>{ctx.int_const("c10"), ctx.int_const("c11"), ctx.int_const("c12"),
                           ctx.int_const("c13"), ctx.int_const("c14"), ctx.int_const("c15"),
                           ctx.int_const("c16"), ctx.int_const("c17"), ctx.int_const("c18")},
        std::array<expr,9>{ctx.int_const("c20"), ctx.int_const("c21"), ctx.int_const("c22"),
                           ctx.int_const("c23"), ctx.int_const("c24"), ctx.int_const("c25"),
                           ctx.int_const("c26"), ctx.int_const("c27"), ctx.int_const("c28")},
        std::array<expr,9>{ctx.int_const("c30"), ctx.int_const("c31"), ctx.int_const("c32"),
                           ctx.int_const("c33"), ctx.int_const("c34"), ctx.int_const("c35"),
                           ctx.int_const("c36"), ctx.int_const("c37"), ctx.int_const("c38")},
        std::array<expr,9>{ctx.int_const("c40"), ctx.int_const("c41"), ctx.int_const("c42"),
                           ctx.int_const("c43"), ctx.int_const("c44"), ctx.int_const("c45"),
                           ctx.int_const("c46"), ctx.int_const("c47"), ctx.int_const("c48")},
        std::array<expr,9>{ctx.int_const("c50"), ctx.int_const("c51"), ctx.int_const("c52"),
                           ctx.int_const("c53"), ctx.int_const("c54"), ctx.int_const("c55"),
                           ctx.int_const("c56"), ctx.int_const("c57"), ctx.int_const("c58")},
        std::array<expr,9>{ctx.int_const("c60"), ctx.int_const("c61"), ctx.int_const("c62"),
                           ctx.int_const("c63"), ctx.int_const("c64"), ctx.int_const("c65"),
                           ctx.int_const("c66"), ctx.int_const("c67"), ctx.int_const("c68")},
        std::array<expr,9>{ctx.int_const("c70"), ctx.int_const("c71"), ctx.int_const("c72"),
                           ctx.int_const("c73"), ctx.int_const("c74"), ctx.int_const("c75"),
                           ctx.int_const("c76"), ctx.int_const("c77"), ctx.int_const("c78")},
        std::array<expr,9>{ctx.int_const("c80"), ctx.int_const("c81"), ctx.int_const("c82"),
                           ctx.int_const("c83"), ctx.int_const("c84"), ctx.int_const("c85"),
                           ctx.int_const("c86"), ctx.int_const("c87"), ctx.int_const("c88")},
    };

    // Domain: each cell is in [1,9]
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            s.add(cells[r][c] >= 1 && cells[r][c] <= 9);
        }
    }

    // Fix given cells
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            if (PUZZLE[r][c] != 0) {
                s.add(cells[r][c] == PUZZLE[r][c]);
            }
        }
    }

    // Row constraints: all distinct in each row
    for (int r = 0; r < 9; r++) {
        for (int c1 = 0; c1 < 9; c1++) {
            for (int c2 = c1 + 1; c2 < 9; c2++) {
                s.add(cells[r][c1] != cells[r][c2]);
            }
        }
    }

    // Column constraints: all distinct in each column
    for (int c = 0; c < 9; c++) {
        for (int r1 = 0; r1 < 9; r1++) {
            for (int r2 = r1 + 1; r2 < 9; r2++) {
                s.add(cells[r1][c] != cells[r2][c]);
            }
        }
    }

    // 3x3 box constraints: all distinct in each box
    for (int br = 0; br < 3; br++) {
        for (int bc = 0; bc < 3; bc++) {
            std::vector<expr> box;
            for (int r = 0; r < 3; r++) {
                for (int c = 0; c < 3; c++) {
                    box.push_back(cells[br*3+r][bc*3+c]);
                }
            }
            for (int i = 0; i < 9; i++) {
                for (int j = i + 1; j < 9; j++) {
                    s.add(box[i] != box[j]);
                }
            }
        }
    }

    if (s.check() == sat) {
        model m = s.get_model();
        std::array<std::array<int,9>,9> solution{};
        for (int r = 0; r < 9; r++) {
            for (int c = 0; c < 9; c++) {
                solution[r][c] = m.eval(cells[r][c]).as_int64();
            }
        }
        std::cout << "Solution:" << std::endl;
        print_grid(solution);
    } else {
        std::cout << "No solution found - check puzzle validity." << std::endl;
    }

    return 0;
}
