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
#include <cassert>
using namespace std;
mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());
vector<string> G0 = {"R", "L", "F", "B", "U", "D", "r", "l", "f", "b", "u", "d"};
vector<string> G1 = {"R", "L", "F", "B", "U", "D", "r", "l", "f2", "b2", "u2", "d2"};
vector<string> G2 = {"R2", "L2", "F", "B", "U", "D", "r2", "l2", "f2", "b2", "u2", "d2"};
vector<string> G3 = {"R2", "L2", "F2", "B2", "U", "D", "r2", "l2", "f2", "b2"};
vector<string> G4 = {"R", "L", "F", "B", "U", "D"};
vector<string> G5 = {"R2", "L2", "F", "B", "U", "D"};
vector<string> G6 = {"R2", "L2", "F2", "B2", "U", "D"};
vector<string> G7 = {"R2", "L2", "F2", "B2", "U2", "D2"};

const int TEST_NUM=1;
int population_size=2000;
int generation = 1000;

vector<Rubik> pop;
Rubik scramble(Rubik r,int len){
    while(len--){
        if(r.phase==1){
            uniform_int_distribution<int> dis(0, (int)G0.size() - 1);
            r.operation(G0[dis(gen)].c_str());
        }
        else if(r.phase==2){
            uniform_int_distribution<int> dis(0, (int)G1.size() - 1);
            r.operation(G1[dis(gen)].c_str());
        }
        else if(r.phase==3){
            uniform_int_distribution<int> dis(0, (int)G2.size() - 1);
            r.operation(G2[dis(gen)].c_str());
        }
        else if(r.phase==4){
            uniform_int_distribution<int> dis(0, (int)G3.size() - 1);
            r.operation(G3[dis(gen)].c_str());
        }
        else if(r.phase==5){
            uniform_int_distribution<int> dis(0, (int)G4.size() - 1);
            r.operation(G4[dis(gen)].c_str());
        }
        else if(r.phase==6){
            uniform_int_distribution<int> dis(0, (int)G5.size() - 1);
            r.operation(G5[dis(gen)].c_str());
        }
        else if(r.phase==7){
            uniform_int_distribution<int> dis(0, (int)G6.size() - 1);
            r.operation(G6[dis(gen)].c_str());
        }
        else if(r.phase==8){
            uniform_int_distribution<int> dis(0, (int)G7.size() - 1);
            r.operation(G7[dis(gen)].c_str());
        } 
    }   

    r.fitness();//necessary!!!

    return r;
}
void initialize(Rubik &r){
    for(int i=0;i<population_size;i++){
        pop.push_back(scramble(r,10));
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
void f(Rubik &r,char *str){
    for(int i=0; str[i];i++){
        char tmp[2]={str[i],'\0'};
        r.operation(tmp);
    }
    r.fitness();
}
void mutation(Rubik &r){
    uniform_int_distribution<int> dis(1, 20);
    int len=dis(gen);//generate the length of operation

    if(r.phase==4&&r.value==16){
        Rubik temp=r;
        // FRF'UR'F
        f(temp,"FRFFFURRRFdRFFFURRRFddd");
        //d R F' U R' F d'
        pop.push_back(temp);
    }
    r=scramble(r,len);
}
void expansion(){//expand the size of pop to population_size
    int sz=(int)pop.size();
    while((int)pop.size() < population_size){
        uniform_int_distribution<int> dis(0, sz - 1);
        auto r = pop[dis(gen)];
        pop.emplace_back(scramble(r,10));
    }
}
int main(){
    for(int i=0;i<TEST_NUM;i++){
        Rubik r("UUULDFFFLLFFFRBBBLLLRRDLLUBBUDDDLLDDDLLLUuUuFfFfBUURrRrUuUuFDFFUUFfFfFFRrUFfFfFFDDDBUuFfFfFfLLLRRDDFfFF");
        puts("original:");
        r.print();
        // Rubik r("UUULDFFFLLFFFRBBBLLLRRDLLUBBUDDDLLDDDLLLUuUuFuUuFDFFUUFfFfFFRrUFfFfFFDDDBUuFfFfFfLLLRRDDFfFF");

        initialize(r);
        
        for(int phase = 1; phase < 9 ; phase++){
            printf("start phase : %d\n",phase);
            for(int j=0;j<generation;j++){
                vector<Rubik> offspring;
                for(int k=0;k<population_size;k++){
                    Rubik x=tournament_selection(3);
                    assert(x.phase==phase);
                    mutation(x);
                    offspring.emplace_back(x);
                }        
                pop.insert(pop.end(), offspring.begin(), offspring.end());
                sort(pop.begin(), pop.end(),[](const Rubik& a, const Rubik& b) {return a.value > b.value;});
                pop.resize(population_size);
            }
            
            for(auto &x:pop)
                x.phase_check();//check and modify the phase
            auto condition = [&](const auto& x) { return x.phase == phase;};
            pop.erase(remove_if(pop.begin(), pop.end(), condition), pop.end());            
            pop[0].print();
            printf("end phase : %d\n",phase);
            
            //reset the fitness value
            for(auto &x:pop)
                x.fitness();
            if(phase==2){
                population_size=4000;
                generation = 2000;
            }
            if(phase==4)
                break;
            expansion();
        }
    }
}
