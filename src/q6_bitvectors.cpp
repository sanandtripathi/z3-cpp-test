/**
 * Q6: Bitvector Arithmetic with Z3
 *
 * Practice: Use Z3's bitvector (fixed-width integer) theory, which models
 * machine-level integer arithmetic including overflow behavior.
 *
 * Problem 6a: Find an 8-bit value x such that (x * 7) == 42 (mod 256)
 *
 * Problem 6b: Overflow detection
 *   Given 8-bit unsigned integers a and b, find values where a + b overflows
 *   (i.e., the mathematical sum exceeds 255).
 *
 * Problem 6c: Verify that for 32-bit integers, the "bit trick" for computing
 *   absolute value abs(x) = (x ^ (x >> 31)) - (x >> 31) is correct.
 *   (Check by trying to find a counterexample.)
 *
 * Problem 6d: Find a 16-bit value x such that x & (x-1) == 0 and x > 1
 *   (i.e., x is a power of 2).
 */

#include <z3++.h>
#include <iostream>

using namespace z3;

void problem_6a() {
    std::cout << "=== Problem 6a: 8-bit x such that (x * 7) mod 256 == 42 ===" << std::endl;

    context ctx;
    solver s(ctx);

    expr x = ctx.bv_const("x", 8);
    expr seven = ctx.bv_val(7, 8);
    expr forty_two = ctx.bv_val(42, 8);

    s.add(x * seven == forty_two);

    if (s.check() == sat) {
        model m = s.get_model();
        unsigned val = m.eval(x).as_uint64();
        std::cout << "  x = " << val << "  (0x" << std::hex << val << std::dec << ")" << std::endl;
        std::cout << "  Verify: " << val << " * 7 mod 256 = " << (val * 7) % 256 << std::endl;
    } else {
        std::cout << "  No solution." << std::endl;
    }
    std::cout << std::endl;
}

void problem_6b() {
    std::cout << "=== Problem 6b: 8-bit unsigned addition overflow ===" << std::endl;

    context ctx;
    solver s(ctx);

    expr a = ctx.bv_const("a", 8);
    expr b = ctx.bv_const("b", 8);

    // Zero-extend to 9 bits to detect overflow
    expr a9 = zext(a, 1);
    expr b9 = zext(b, 1);
    expr sum9 = a9 + b9;
    expr limit = ctx.bv_val(256, 9);

    // Overflow: sum > 255 in 9-bit unsigned arithmetic
    s.add(uge(sum9, limit));
    s.add(ugt(a, ctx.bv_val(100, 8)));   // additional constraint to get an interesting example
    s.add(ugt(b, ctx.bv_val(100, 8)));

    if (s.check() == sat) {
        model m = s.get_model();
        unsigned va = m.eval(a).as_uint64();
        unsigned vb = m.eval(b).as_uint64();
        std::cout << "  Overflow example: a=" << va << " b=" << vb << std::endl;
        std::cout << "  a + b = " << va + vb << " (overflows 8-bit, wraps to "
                  << (va + vb) % 256 << ")" << std::endl;
    } else {
        std::cout << "  No overflow example found." << std::endl;
    }
    std::cout << std::endl;
}

void problem_6c() {
    std::cout << "=== Problem 6c: Verify 32-bit abs() bit trick ===" << std::endl;
    std::cout << "  abs(x) = (x ^ (x >> 31)) - (x >> 31)  [arithmetic right shift]" << std::endl;

    context ctx;
    solver s(ctx);

    expr x = ctx.bv_const("x", 32);
    expr zero = ctx.bv_val(0, 32);

    // Bit trick: arithmetic right shift fills with sign bit
    expr mask = ashr(x, ctx.bv_val(31, 32));
    expr abs_trick = (x ^ mask) - mask;

    // Mathematical abs: if x >= 0 then x else -x
    expr abs_math = ite(sge(x, zero), x, -x);

    // We skip INT_MIN (0x80000000) because abs(INT_MIN) overflows in 32-bit
    expr int_min = ctx.bv_val((int64_t)0x80000000LL, 32);
    s.add(x != int_min);

    // Check if there's a counterexample where the trick disagrees with math
    s.add(abs_trick != abs_math);

    if (s.check() == unsat) {
        std::cout << "  VERIFIED: bit trick equals mathematical abs for all 32-bit integers "
                  << "(excluding INT_MIN)" << std::endl;
    } else {
        model m = s.get_model();
        std::cout << "  COUNTEREXAMPLE found: x = " << m.eval(x) << std::endl;
    }
    std::cout << std::endl;
}

void problem_6d() {
    std::cout << "=== Problem 6d: Find 16-bit x > 1 where x & (x-1) == 0 (power of 2) ===" << std::endl;

    context ctx;
    solver s(ctx);

    expr x = ctx.bv_const("x", 16);
    expr one = ctx.bv_val(1, 16);
    expr zero = ctx.bv_val(0, 16);

    s.add((x & (x - one)) == zero);
    s.add(ugt(x, one));  // x > 1 (unsigned)

    // Enumerate a few solutions
    std::cout << "  First few powers of 2 (16-bit):" << std::endl;
    int found = 0;
    while (s.check() == sat && found < 5) {
        model m = s.get_model();
        unsigned val = m.eval(x).as_uint64();
        std::cout << "  x = " << val << std::endl;
        s.add(x != ctx.bv_val(val, 16));
        found++;
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "Z3 Practice - Q6: Bitvector Arithmetic" << std::endl;
    std::cout << "=======================================\n" << std::endl;
    problem_6a();
    problem_6b();
    problem_6c();
    problem_6d();
    return 0;
}
