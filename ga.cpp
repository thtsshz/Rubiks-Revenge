#include "rubik.h"
#include<stdio.h>
#include<iostream>
#include<vector>
#include<math.h>
#include<random>
#include<cstdlib>
#include<chrono>
#include <fstream>
#include<algorithm>
using namespace std;
mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());
vector<string> G0 = {"R", "L", "F", "B", "U", "D", "r", "l", "f", "b", "u", "d"};
vector<string> G1 = {"R", "L", "F", "B", "U", "D", "r", "l", "f2", "b2", "u2", "d2"};
vector<string> G2 = {"R2", "L2", "F", "B", "U", "D", "r2", "l2", "f2", "b2", "u2", "d2"};
vector<string> G3 = {"R2", "L2", "F2", "B2", "U", "D", "r2", "l2", "f2", "b2"};

const int TEST_NUM=1;
const int population_size=2000;
const int generation = 1000;
vector<Rubik> pop;
Rubik scramble(Rubik r,int len){
    while(len--){
        if(r.phase==1){
            uniform_int_distribution<int> dis(0, (int)G0.size() - 1);
            r.operation(G0[dis(gen)].c_str());
        }
    }
    r.fitness();
    return r;
}
void initialize(Rubik &r){
    for(int i=0;i<population_size;i++){
        pop.emplace_back(scramble(r,10));
    }
    // for(int i=0;i<5;i++)
    //     pop[i].print();
}
Rubik tournament_selection(int sz){
    uniform_int_distribution<int> dis(0, population_size - 1);
    vector<Rubik> temp;
    while(sz--)
        temp.push_back(pop[dis(gen)]);

    return *max_element(temp.begin(), temp.end(), [](const Rubik& a, const Rubik& b) {
        return a.value > b.value; // Compare directly to find the maximum
    });
}
void mutation(Rubik &r){
    uniform_int_distribution<int> dis(1, 5);
    int len=dis(gen);//generate the length of operation
    r=scramble(r,len);
}
int main(){
    for(int i=0;i<TEST_NUM;i++){
        Rubik r("UUULDFFFLLFFFRBBBLLLRRDLLUBBUDDDLLDDDLLLUuUuFfFfBUURrRrUuUuFDFFUUFfFfFFRrUFfFfFFDDDBUuFfFfFfLLLRRDDFfFF");
        // r.print();
        initialize(r);
        for(int j=0;j<generation;j++){
            vector<Rubik> offspring;
            for(int k=0;k<population_size;k++){
                Rubik x=tournament_selection(5);
                mutation(x);
                offspring.emplace_back(x);
            }        
            pop.insert(pop.end(), offspring.begin(), offspring.end());
		    sort(pop.begin(), pop.end(),[](const Rubik& a, const Rubik& b) {return a.value > b.value;});
		    pop.resize(population_size);
            if(j%100==0){
                printf("fitness : %d\n",pop[0].value);
                pop[0].print();
            }
        }
        printf("fitness : %d\n",pop[0].value);
        pop[0].print();
    }
}
