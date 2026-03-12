/**
 * Q1: Linear Arithmetic with Z3
 *
 * Practice: Use Z3 to solve systems of linear equations and inequalities.
 *
 * Problem 1a: Solve the following system of equations:
 *   x + y = 10
 *   2x - y = 5
 *
 * Problem 1b: Find integer values x, y, z such that:
 *   x + y + z = 100
 *   x >= 10,  y >= 20,  z >= 30
 *   x <= 50,  y <= 40
 *   2x + y = z
 */

#include <z3++.h>
#include <iostream>

using namespace z3;

void problem_1a() {
    std::cout << "=== Problem 1a: Solve x + y = 10, 2x - y = 5 ===" << std::endl;

    context ctx;
    solver s(ctx);

    expr x = ctx.real_const("x");
    expr y = ctx.real_const("y");

    s.add(x + y == 10);
    s.add(2 * x - y == 5);

    if (s.check() == sat) {
        model m = s.get_model();
        std::cout << "Solution found:" << std::endl;
        std::cout << "  x = " << m.eval(x) << std::endl;
        std::cout << "  y = " << m.eval(y) << std::endl;
    } else {
        std::cout << "No solution exists." << std::endl;
    }
    std::cout << std::endl;
}

void problem_1b() {
    std::cout << "=== Problem 1b: Integer constraints ===" << std::endl;
    std::cout << "  x + y + z = 100" << std::endl;
    std::cout << "  x in [10,50], y in [20,40], z >= 30" << std::endl;
    std::cout << "  2x + y = z" << std::endl;

    context ctx;
    solver s(ctx);

    expr x = ctx.int_const("x");
    expr y = ctx.int_const("y");
    expr z = ctx.int_const("z");

    s.add(x + y + z == 100);
    s.add(x >= 10);
    s.add(x <= 50);
    s.add(y >= 20);
    s.add(y <= 40);
    s.add(z >= 30);
    s.add(2 * x + y == z);

    if (s.check() == sat) {
        model m = s.get_model();
        std::cout << "Solution found:" << std::endl;
        std::cout << "  x = " << m.eval(x) << std::endl;
        std::cout << "  y = " << m.eval(y) << std::endl;
        std::cout << "  z = " << m.eval(z) << std::endl;
        // Verify
        int vx = m.eval(x).as_int64();
        int vy = m.eval(y).as_int64();
        int vz = m.eval(z).as_int64();
        std::cout << "  Verification: x+y+z=" << vx+vy+vz
                  << ", 2x+y=" << 2*vx+vy << ", z=" << vz << std::endl;
    } else {
        std::cout << "No solution exists." << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "Z3 Practice - Q1: Linear Arithmetic" << std::endl;
    std::cout << "====================================\n" << std::endl;
    problem_1a();
    problem_1b();
    return 0;
}
