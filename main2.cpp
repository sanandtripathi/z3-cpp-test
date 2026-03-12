//vehicle problem
//minimizing time and cost of charging

#include<bits/stdc++.h>
#include<z3++.h>

using namespace std;
using namespace z3;

struct Vehicle{
    int id;
    int arrival;
    int departure;
    int charge_time; // this is called the c_i time on the slowest time

}
int main(){

    //step 1 read the input file
    ifstream infile("input.txt");
    if(!infile)
    {
        cerr<"error "<<endl;
        return 1;
    }

    string dummy;
    int K;

    //read the number of ports
    getline(infile, dummy);
    infile>>dummy>>K;

    //read the prices
    vector<int>prices(K+1); 1 //1 indexed to match the port numbers

    getline(infile, dummy);
    getline(infile, dummy);
    infile>>dummy; //read P
    for(int k=1;k<=K;k++){
        infile>>prices[k];
    }

    //read vehicles
    vector<Vehicle> vehicles;
    getline(infile, dummy);//skip vehincles
    getline(infile, dummy);// skip comments

    Vehicle v;
    while(infile>>dummy>>v.id>>v.arrival>>v.departure>>v.charge_time){
        vehicles.push_back(v);
    }
    infile.close();

    int N= vehicles.size();

    //step 2 set up the z3 envrionment

    context c;
    optimize opt(c);

    //step 3 declare z3 variables

    vector<expr> S;
    vector<expr> Port;
    vector<expr> Duration;
    vector<expr> Cost;

    for(int i=0;i<N;i++){

        S.push_back(c.int_const(("S_"+to_string(i)).c_str()));
        Port.push_back(c.int_const(("Port_"+to_string(i)).c_str()));
        Duration.push_back(c.int_const(("Dur_"+to_string(i)).c_str()));
        Cost.push_back(c.int_const(("Cost_"+to_string(i) ).c_str()));
    }

    //step 4 add constraintss

    for(int i=0;i<N;i++){
        //valid port assignemnet

        opt.add(Port[i]>=1);
        opt.add(Port[i]<=K);

        //time bound
        //cannot start before arrival

        opt.add(S[i]>=vehicles[i].arrival);
        //must finish by deadline
        opt.add(S[i]+Duration[i]<=vehicles[i].departure);

        //link the port choice to the duration and cost using implies

        for(int k=0;k<=K;k++){
            //calculate the ceiling division cein
            //ceil c_i/k=c_i+k-1/k

            int actual_time=(vehicles[i].charge_time+k-1)/k;
            int actual_cost=actual_time*prices[k];

            //if this vehicle uses prot k, force th duration and cost to match

            opt.add(implies(Port[i]==k, Duration[i]==actual_time));
            opt.add(implies(Port[i]==k), Cost[i]==actual_cost);
        }
    }

    //no overlapping constraints

    for(int i=0;i<N;i++){
        for(int j=i+1;j<N;j++){
            expr same_port=(Port[i]==Port[j]);
            expr i_finishes_first=(S[i]+Duration[i]<=S[j]);
            expr j_finishes_first=(S[j]+Duration[j]<=S[i]);

            //if they share a port hey cannot overlap

            opt.add(implies(same_port, i_finishes_first||j_finishes_first));
        }
    }

    //objective and solve

    expr total_cost=c.int_const("total_cost");

    //create an expression tha sums all individiaj vehible cost

    expr sum_expr=Cost[0];

    for(int i=1;i<N;i++){
        sum_expr=sum_expr+Cost[i];


    }

    //constratins total_cost to equal to sum

    opt.add(total_cost==sum_expr);

    //minimize th etoal cost

    opt.minimize(total_cost);

    if(opt.check()==sat){
        model m = opt.get_model();
        cout<<"optimal schedle found"<<endl;

        cout<<"minimum toatl cost "<<m.eval(total_cost).get_numeral_int()<<endl;

        for(int i=0;i<N;i){
            cout<<"vehicle "<<vehicles[i].id<<"-> porst"<< m.eval(Port[i]).get_numeral_int()<<" starts at"<<m.eval(S[i]).get_numeral_int()<<"Duration "<<m.eval(Duration[i]).get_numeral_int()<<"cost  "<<m.eval(Cost[i]).get_numeral_int()<<endl;
        }


    }

    else{
        cout<<"not feasible"<<endl;
    }

    return 0;




}
