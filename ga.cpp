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
#include <stdlib.h>
#include <string.h>

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
const bool DISPLAY=false;
const int TEST_NUM=1;
const int MAX_SEQUENCE_LENGTH=200;
int population_size = 50000;
int generation = 350;
int random_len[10]={10, 10, 10, 15, 10, 13, 20, 17};
int fail_phase[10]={0};
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
    uniform_int_distribution<int> dis(1, random_len[r.phase-1]);
    int len=dis(gen);//generate the length of operation

    if(r.phase==4&&r.value==16){
        Rubik temp=r;
        // FRF'UR'F
        f(temp,"FRFFFURRRFdRFFFURRRFddd");
        //d R F' U R' F d'
        pop.push_back(temp);
    }
    if(r.phase==7&&(r.value==10000||r.value==10002)){
        Rubik temp=r;
        //(Uu)2 (Ll)2 U2 l2 U2 (Ll)2 (Uu)2 F' U' F U F R' F2 U F U F' U' F R 
        // UUuu  LLll UU ll UU LLll  UUuu  FFF UUU FUFRRR FF U F U FFF UUU F R
        f(temp,"UUuuLLllUUllUULLllUUuuFFFUUUFUFRRRFFUFUFFFUUUFR");
        pop.push_back(temp);
        temp=r;
        //r2 B2 U2 l U2 r' U2 r U2 F2 r F2 l' B2 r2
        //rr BB UU l UU rrr UU r UU FF r FF lll BB rr
        // f(temp,"rrBBUUlUUrrrUUrUUFFrFFlllBBrr");
        // pop.push_back(temp);
        temp=r;
        //L2 D (Ff)2 (Ll)2 F2 l2 F2 (Ll)2 (Ff)2 D' L2 
        //LL F FFff LLll FF ll FF LLll FFff DDD LL
        // f(temp,"LLFFFffLLllFFllFFLLllFFffDDDLL");
        // pop.push_back(temp);
        temp=r;
        // (Uu)2 (Ll)2' U2 l2' U2 (Ll)2' (Uu)2 
        // UUuu LLll UU ll UU LLll UUuu
        f(temp,"UUuuLLllUUllUULLllUUuu");
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
    char operation[MAX_SEQUENCE_LENGTH];
    FILE *file = fopen("testcase/testcases_op.txt", "r");
    if (file == NULL) {
        perror("Failed to open file");
        return 1;
    }
    for(int i=0;i<TEST_NUM;i++){
        for(int t=0;t<50;t++){
            pop.clear();
            fgets(operation, MAX_SEQUENCE_LENGTH, file);
            operation[strlen(operation)-1]='\0'; 
            puts(operation);
            Rubik r(operation);
            if(DISPLAY){
                puts("original:");
                r.print();
            }
            
            jump:

            // Rubik r("UUULDFFFLLFFFRBBBLLLRRDLLUBBUDDDLLDDDLLLUuUuFuUuFDFFUUFfFfFFRrUFfFfFFDDDBUuFfFfFfLLLRRDDFfFF");
            initialize(r);
            population_size=10000;
            generation = 200;            
            int phase;
            for(phase = 1; phase < 9 ; phase++){
                if(DISPLAY)
                    printf("start phase : %d\n",phase);

                for(int j=0;j<generation;j++){
                    vector<Rubik> offspring;
                    for(int k=0;k<population_size*5;k++){
                        Rubik x=tournament_selection(3);
                        assert(x.phase==phase);
                        mutation(x);
                        offspring.emplace_back(x);
                    }        
                    pop.insert(pop.end(), offspring.begin(), offspring.end());
                    sort(pop.begin(), pop.end(),[](const Rubik& a, const Rubik& b) {return a.value > b.value;});
                    pop.resize(population_size);
                    // if(j%50==0&&phase==7){
                    //     printf("fitness : %d\n",pop[0].value);
                    //     pop[0].print();

                    // }
                }
                
                for(auto &x:pop)
                    x.phase_check();//check and modify the phase
                auto condition = [&](const auto& x) { return x.phase == phase;};
                pop.erase(remove_if(pop.begin(), pop.end(), condition), pop.end()); 
                if(!pop.size()){
                    // printf("redo on phase : %d\n",phase);// should be restart
                    // goto jump;
                    break;
                }
                if(DISPLAY){
                    printf("remain: %d\n",(int)pop.size());        
                    printf("fitness: %d\n",(int)pop[0].value);        
                    pop[0].print();
                    printf("end phase : %d\n",phase);
                }
                
                //reset the fitness value
                if(phase<8){
                    for(auto &x:pop)
                        x.fitness();
                }
                
                if(phase==3){

                    population_size+=5000;
                    generation*=2;
                }
                if(phase==6){
                    generation*=2;
                }
                // if(phase==4){
                //     population_size*=2;
                //     generation*=2;

                // }
                // if(phase==6)
                //     break;
                if(phase<8)
                    expansion();
                
            }
            printf("%d\n",phase);
            fail_phase[phase]++;
        }

    }
    for(int i=1;i<10;i++){
        printf("%d : %d\n",i,fail_phase[i]);
    }
}
