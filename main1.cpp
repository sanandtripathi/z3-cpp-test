#include<bits/stdc++.h>
#include<z3++.h>
using namespace std;
using namespace z3;

struct Task{
    int id;
    int release_time;
    int execution_time;
    int deadline;
};

int main(){

    //step 1 read the input file (basic c++)

    ifstream infile("input.txt");
    if(!infile){
        cerr<<"error could not open the input file"<<endl;
        return 1;
    }

    int n, tau;

    //used to absorb text like taskcount timebound t and %

    string dummy;

    infile>>dummy>>n;//reads TaskCount into dummy then 5 into n
    infile>>dummy>>tau;//reads TimeBound into dummy, then 10 into tau;
    getline(infile, dummy);//finish the current line
    getline(infile, dummy);//read the whole comment line and discard it

    vector<Task> tasks(n);
    for(int i=0;i<n;i++){

        infile>>dummy; //absorb the "T" character
        infile>>tasks[i].id>>tasks[i].release_time>>tasks[i].execution_time>>tasks[i].deadline;

    }

    infile.close();

    //step 2

    //setting up the z3 environment

    //context then optimize then declaration

    context c;
    optimize opt(c);

    //step 3

    //declare the z3 variables

    vector<expr> S; //array to hold the start time
    //array to hold the processor id
    vector<expr> P;
    //the variable we want to minimizze

    expr max_P= c.int_const("max_P");

    for(int i=0;i<n;i++){

        //we name the variables S_0, S_1...
        //P_0... inside z3 so that it can track them

        S.push_back(c.int_const(("S_"+to_string(i)).c_str()));

        P.push_back(c.int_const(("P_"+to_string(i)).c_str()));
    }

    //step 4

    //add constraints the rules

    for(int i=0;i<n;i++){
        //task cannot start before release time

        opt.add(S[i]>=tasks[i].release_time);

        //task must finish before the deadline

        opt.add(S[i]+tasks[i].execution_time<=tasks[i].deadline);

        //task must finish before global time bound
        opt.add(S[i]+tasks[i].execution_time<=tau);

        //processor IDs must be between 1 and max_P
        opt.add(P[i]>=1);
        opt.add(P[i]<=max_P);
    }

    //the non-overlap rule 
    //compariing every pair of tasks

    for(int i=0;i<n;i++){
        for(int j=i;j<n;j++){
            //check if they are assigned to the same processor

            expr same_processor=(P[i]==P[j]);

            //task i finsihes before task j starts
            expr i_finishes_first=(S[i]+tasks[i].execution_time<=S[j]);

            //task j finsihes before task i starts
            expr j_finishes_first= (S[j]+tasks[j].execution_time<=S[i]);

            //the z3 implies logic
            //if same processor
            //then one msut finish before the other starts

            opt.add (implies(same_processor, i_finishes_first||j_finishes_first));
        }
    }

    //step 5

    //solve and print

    //tell z3 what our goal is

    opt.minimize(max_P);

    //run the solver
    if (opt.check()==sat){
        //'sat means satisfiable'
        model m=opt.get_model();
        cout<<"minimum processors "<<m.eval(max_P).get_numeral_int()<<endl;

        //optional: print the schedule to see how z3 solved it

        cout<<"schedule details "<<endl;

        for(int i=0;i<n;i++){
            cout<<"task "<<tasks[i].id<< "-> Processor: "<<m.eval(P[i]).get_numeral_int()<<", Starts at: "<<m.eval(S[i]).get_numeral_int()<<endl;
        }

        }
        else {
            cout<<"infeasible "<<endl;
        }

        return 0;

}
