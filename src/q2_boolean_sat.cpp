/**
 * Q2: Boolean Satisfiability (SAT) with Z3
 *
 * Practice: Use Z3 to check satisfiability of propositional logic formulas.
 *
 * Problem 2a: Find an assignment for p, q, r such that:
 *   (p || q) && (!p || r) && (!q || !r)
 *
 * Problem 2b: Verify that the following is a tautology (always true):
 *   (p => q) => (!q => !p)   (contrapositive law)
 *
 * Problem 2c: Cryptarithmetic puzzle - SEND + MORE = MONEY
 *   Each letter is a distinct digit (0-9), leading digits S and M are non-zero.
 */

#include <z3++.h>
#include <iostream>

using namespace z3;

void problem_2a() {
    std::cout << "=== Problem 2a: SAT - (p||q) && (!p||r) && (!q||!r) ===" << std::endl;

    context ctx;
    solver s(ctx);

    expr p = ctx.bool_const("p");
    expr q = ctx.bool_const("q");
    expr r = ctx.bool_const("r");

    s.add((p || q) && (!p || r) && (!q || !r));

    if (s.check() == sat) {
        model m = s.get_model();
        std::cout << "Formula is satisfiable." << std::endl;
        std::cout << "  p = " << m.eval(p) << std::endl;
        std::cout << "  q = " << m.eval(q) << std::endl;
        std::cout << "  r = " << m.eval(r) << std::endl;
    } else {
        std::cout << "Formula is unsatisfiable." << std::endl;
    }
    std::cout << std::endl;
}

void problem_2b() {
    std::cout << "=== Problem 2b: Tautology check - (p=>q) => (!q=>!p) ===" << std::endl;

    context ctx;
    solver s(ctx);

    expr p = ctx.bool_const("p");
    expr q = ctx.bool_const("q");

    // To check tautology, negate the formula and check unsat
    expr tautology = implies(implies(p, q), implies(!q, !p));
    s.add(!tautology);

    if (s.check() == unsat) {
        std::cout << "The formula IS a tautology (contrapositive law verified)." << std::endl;
    } else {
        model m = s.get_model();
        std::cout << "NOT a tautology. Counterexample:" << std::endl;
        std::cout << "  p = " << m.eval(p) << std::endl;
        std::cout << "  q = " << m.eval(q) << std::endl;
    }
    std::cout << std::endl;
}

void problem_2c() {
    std::cout << "=== Problem 2c: Cryptarithmetic - SEND + MORE = MONEY ===" << std::endl;

    context ctx;
    solver s(ctx);

    expr S = ctx.int_const("S");
    expr E = ctx.int_const("E");
    expr N = ctx.int_const("N");
    expr D = ctx.int_const("D");
    expr M = ctx.int_const("M");
    expr O = ctx.int_const("O");
    expr R = ctx.int_const("R");
    expr Y = ctx.int_const("Y");

    expr vars[] = {S, E, N, D, M, O, R, Y};
    int n = 8;

    // Each digit in [0,9]
    for (int i = 0; i < n; i++) {
        s.add(vars[i] >= 0);
        s.add(vars[i] <= 9);
    }

    // All digits distinct
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            s.add(vars[i] != vars[j]);
        }
    }

    // Leading digits non-zero
    s.add(S != 0);
    s.add(M != 0);

    // SEND + MORE = MONEY
    expr SEND  = 1000*S + 100*E + 10*N + D;
    expr MORE  = 1000*M + 100*O + 10*R + E;
    expr MONEY = 10000*M + 1000*O + 100*N + 10*E + Y;
    s.add(SEND + MORE == MONEY);

    if (s.check() == sat) {
        model m = s.get_model();
        std::cout << "Solution:" << std::endl;
        std::cout << "  S=" << m.eval(S) << " E=" << m.eval(E)
                  << " N=" << m.eval(N) << " D=" << m.eval(D) << std::endl;
        std::cout << "  M=" << m.eval(M) << " O=" << m.eval(O)
                  << " R=" << m.eval(R) << " Y=" << m.eval(Y) << std::endl;
        std::cout << "  SEND  = " << m.eval(SEND)  << std::endl;
        std::cout << "  MORE  = " << m.eval(MORE)  << std::endl;
        std::cout << "  MONEY = " << m.eval(MONEY) << std::endl;
    } else {
        std::cout << "No solution found." << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "Z3 Practice - Q2: Boolean SAT & Logic Puzzles" << std::endl;
    std::cout << "==============================================\n" << std::endl;
    problem_2a();
    problem_2b();
    problem_2c();
    return 0;
}
