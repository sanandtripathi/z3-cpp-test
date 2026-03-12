/**
 * Q5: Optimization with Z3
 *
 * Practice: Use Z3's optimization API to maximize or minimize objective functions
 * subject to constraints.
 *
 * Problem 5a: Linear Programming (Integer)
 *   Maximize: 3x + 5y
 *   Subject to:
 *     x + y  <= 10
 *     x + 2y <= 14
 *     x >= 0, y >= 0
 *
 * Problem 5b: Scheduling - Minimize makespan
 *   3 tasks with durations [3, 5, 2] must be scheduled on 2 machines.
 *   Each task runs on exactly one machine, no preemption.
 *   Minimize the total makespan (max finish time across machines).
 */

#include <z3++.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace z3;

void problem_5a() {
    std::cout << "=== Problem 5a: Integer Linear Programming ===" << std::endl;
    std::cout << "  Maximize 3x + 5y  s.t. x+y<=10, x+2y<=14, x,y>=0" << std::endl;

    context ctx;
    optimize opt(ctx);

    expr x = ctx.int_const("x");
    expr y = ctx.int_const("y");

    opt.add(x + y <= 10);
    opt.add(x + 2*y <= 14);
    opt.add(x >= 0);
    opt.add(y >= 0);

    opt.maximize(3*x + 5*y);

    if (opt.check() == sat) {
        model m = opt.get_model();
        int vx = m.eval(x).as_int64();
        int vy = m.eval(y).as_int64();
        std::cout << "Optimal solution:" << std::endl;
        std::cout << "  x = " << vx << std::endl;
        std::cout << "  y = " << vy << std::endl;
        std::cout << "  Objective value: 3*" << vx << " + 5*" << vy
                  << " = " << 3*vx + 5*vy << std::endl;
    } else {
        std::cout << "No optimal solution found." << std::endl;
    }
    std::cout << std::endl;
}

void problem_5b() {
    std::cout << "=== Problem 5b: Minimize Makespan ===" << std::endl;

    const int NUM_TASKS = 3;
    const int NUM_MACHINES = 2;
    const int durations[] = {3, 5, 2};

    std::cout << "  Tasks: 3 tasks with durations [3, 5, 2] on 2 machines" << std::endl;
    std::cout << "  Goal: minimize makespan" << std::endl;

    context ctx;
    optimize opt(ctx);

    // machine[i] = which machine task i runs on (0 or 1)
    std::vector<expr> machine;
    for (int i = 0; i < NUM_TASKS; i++) {
        machine.push_back(ctx.int_const(("m" + std::to_string(i)).c_str()));
        opt.add(machine[i] >= 0 && machine[i] < NUM_MACHINES);
    }

    // load[j] = total duration of tasks on machine j
    std::vector<expr> load;
    for (int j = 0; j < NUM_MACHINES; j++) {
        expr sum = ctx.int_val(0);
        for (int i = 0; i < NUM_TASKS; i++) {
            sum = sum + ite(machine[i] == j, ctx.int_val(durations[i]), ctx.int_val(0));
        }
        load.push_back(sum);
    }

    // makespan = max load across machines
    expr makespan = ctx.int_const("makespan");
    for (int j = 0; j < NUM_MACHINES; j++) {
        opt.add(makespan >= load[j]);
    }

    opt.minimize(makespan);

    if (opt.check() == sat) {
        model m = opt.get_model();
        std::cout << "Optimal schedule:" << std::endl;
        for (int i = 0; i < NUM_TASKS; i++) {
            int mval = m.eval(machine[i]).as_int64();
            std::cout << "  Task " << i << " (duration " << durations[i]
                      << ") -> Machine " << mval << std::endl;
        }
        std::cout << "  Makespan = " << m.eval(makespan) << std::endl;
    } else {
        std::cout << "No optimal solution found." << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "Z3 Practice - Q5: Optimization" << std::endl;
    std::cout << "==============================\n" << std::endl;
    problem_5a();
    problem_5b();
    return 0;
}
