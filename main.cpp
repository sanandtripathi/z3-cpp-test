#include<iostream>
#include<z3++.h>
using namespace std;
using namespace z3;

int main(){

    //create a context this managess all your z3 memory and variables
    context c;

    //create an optmizer (because both your problems ask for min/max)
    //if it just asked is it possible you would use solver s(c)
    optimize opt(c);

    //declare variable

    expr x=c.int_const("x");
    expr y= c.int_const("y");

    //add constraints (the rules)

    opt.add(x>5);
    opt.add(x+y==4);
    opt.add(y>5);

    //add objective (what to minimize/maximize)

    opt.minimize(x);
    //check if a solution exists and print it

    if (opt.check()==sat){

        model m=opt.get_model();
        cout<<"minimum x is "<<m.eval(x).get_numeral_int()<<endl;

    }
    else{
        cout<<"infeasible"<<endl;
    }
    return 0;


    

}
