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
const int TEST_NUM=250;
const int MAX_SEQUENCE_LENGTH=200;
int population_size = 2500;
int generation = 350;
int random_len[10]={10, 10, 10, 15, 10, 13, 20, 17};
int fail_phase[10]={0};
bool first[10]={0};
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
    pop.clear();
    for(int i=0;i<population_size;i++){
        pop.push_back(scramble(r,10));
    }
    // for(int i=0;i<5;i++)
    //     pop[i].print();
}
Rubik tournament_selection(int sz){
    uniform_int_distribution<int> dis(0, (int)pop.size() - 1);
    vector<Rubik> temp;
    while(sz--)
        temp.push_back(pop[dis(gen)]);

    return *max_element(temp.begin(), temp.end());
    // , [](const Rubik& a, const Rubik& b) {
    //     if(a.value == max_fitness[a.phase] && b.value == a.value){
    //         return a.op_cnt > b.op_cnt;
            
    //     }  
    //     return a.value > b.value; // Compare directly to find the minimum
    // });
}
void f(Rubik &r,const char *str){
    for(int i=0; str[i];i++){
        char tmp[2]={str[i],'\0'};
        r.operation(tmp);
    }
    r.fitness();
}
void mutation(Rubik &r){
    uniform_int_distribution<int> dis(1, random_len[r.phase-1]);
    int len=dis(gen);//generate the length of operation

    if(r.phase==4&&r.value>=14){
        Rubik temp=r,temp2=r;
        // U -> B
        // F -> U
        // R is the same
        // R F' U F
        f(temp,"LFFFUFRLLL");
        // Dd R FFF U RRR F DDDddd
        f(temp2, "LRRRDdRFFFURRRFDDDdddRLLL");        // R U R' F R' F' R
        pop.push_back(temp);
        pop.push_back(temp2);
    }
    if(r.phase==5&&r.value==11){
        Rubik temp=r;
        // r2 B2 U2 l U2 r' U2 r U2 F2 r F2 l' B2 r2
        f(temp,"rrBBUUlUUrrrUUrUUFFrFFlllBBrr");
        pop.push_back(temp);
    }
    if(r.phase==6&&r.value==35){
        Rubik temp=r;
        f(temp,"ULULULUUULLLUUULLL");
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
    if(r.phase==8&&r.value==4){
        Rubik temp=r;

        // (Uu)2 (Ll)2' U2 l2' U2 (Ll)2' (Uu)2 
        f(temp,"UUuuLLllUUllUULLllUUuu");
        pop.push_back(temp);

    }
    r=scramble(r,len);
    // printf("%zu ", r.wca_op.size());
}
void expansion(){//expand the size of pop to population_size
    while((int)pop.size() < population_size){    
        int sz=(int)pop.size();
        uniform_int_distribution<int> dis(0, sz - 1);
        auto r = pop[dis(gen)];
        pop.emplace_back(scramble(r,10));
    }
}
void adjust_parameter(int phase){
    //
    if(phase<3){
        population_size = 2000;
        generation = 100;
        
    }
    // else if(phase==6||phase==5){
    //     population_size = 8000;
    //     generation = 200;

    // }
    else{
        population_size = 5000;
        generation = 100;
    }
}
int main(){
    char operation[MAX_SEQUENCE_LENGTH];
    vector<double> durations;
    vector<vector<int>> step_each_phases(TEST_NUM, vector<int>(9, 0)); 

    FILE *file = fopen("testcase/testcases_op.txt", "r");
    if (file == NULL) {
        perror("Failed to open file");
        return 1;
    }
    double totalDuration = 0.0;
    int total_step=0;
    int total_step_each_phase[9]={0};
    int prev_min_step=0;
    op_map_init();

    for(int i=0;i<1;i++){
        for(int t=0;t<TEST_NUM;t++){        
            auto start = std::chrono::high_resolution_clock::now();

            fgets(operation, MAX_SEQUENCE_LENGTH, file);
            operation[strlen(operation)-1]='\0';
            printf("%d : %s\n",t+1, operation); 
            redo:
            Rubik r(operation);
            if(DISPLAY){
                puts("original:");
                r.print();
            }
            int first_time[10]={0};


            // Rubik r("UUULDFFFLLFFFRBBBLLLRRDLLUBBUDDDLLDDDLLLUuUuFuUuFDFFUUFfFfFFRrUFfFfFFDDDBUuFfFfFfLLLRRDDFfFF");
            initialize(r);
            prev_min_step=0;
            int phase;
            for(phase = 1; phase < 9 ; phase++){
                if(!first_time[phase]){
                    printf("\rsolve phase : %d    ",phase);
                }
                    adjust_parameter(phase);         
                first_time[phase]++;
                if(first_time[phase]>2){//redo
                    puts("\nredo");
                    goto redo;
                }
                // if(DISPLAY)
                    // printf("start phase : %d\n",phase);
                expansion();
                for(int j=0;j<generation;j++){
                    vector<Rubik> offspring;
                    for(int k=0;k<population_size*10;k++){
                        Rubik x=tournament_selection(3);
                        assert(x.phase==phase);
                        mutation(x);
                        offspring.emplace_back(x);
                    }        
                    pop.insert(pop.end(), offspring.begin(), offspring.end());
                    sort(pop.begin(), pop.end());
                    // for(auto x:pop)
                    //     printf("%d\n",x.value);
                    // ,[](const Rubik& a, const Rubik& b) {
                    //     if(a.value == b.value)  return a.op_cnt < b.op_cnt;
                    //     return a.value > b.value;
                    // });
                    // printf("%d ", pop[0].op_cnt);
        //             if(j%50==0&&phase==6){
        //                 printf("%d\n",pop[0].value);
        //                 pop[0].print();
        //                 if(pop[0].value==35){
        //                 puts("test begins");
        //                         Rubik temp=pop[0];
        //                  // DDD RRR D R D F DDD FFF
        // f(temp,"UUULLLULUFUUUFFF");
        //                 temp.print();
        //                 puts("test ends"); 
        //                  printf("%d\n",pop[1].value);
                               
        //                 pop[1].print();

        //                 puts("test begins");

        //                         temp=pop[1];
        //                  // DDD RRR D R D F DDD FFF
        // f(temp,"UUULLLULUFUUUFFF");
        //                 temp.print();
        //                 puts("test ends");
        //                 }
                    // }
                    pop.resize(population_size);
                    
                    
                }
                auto restore = pop;
                for(auto &x:pop)
                    x.phase_check();//check and modify the phase
                auto condition = [&](const auto& x) { return x.phase == phase;};
                pop.erase(remove_if(pop.begin(), pop.end(), condition), pop.end()); 
                if(!pop.size()){
                    pop = restore;
                    // printf("redo on phase : %d\n",phase);// should be restart
                    // printf("fitness: %d %d\n",(int)pop[0].value, (int)pop[1].value);        
                    // pop[0].print();
                //     if(phase==4){
                //         Rubik temp=r,temp2=r;
                //         // U -> B
                //         // F -> U
                //         // R is the same
                //         // R F' U F
                //         f(temp,"LFFFUFRLLL");
                //         // Dd R FFF U RRR F DDDddd
                //         f(temp2, "LRRRDdRFFFURRRFDDDdddRLLL");        // R U R' F R' F' R
                //         puts("^^^^^^^^^^^^^^^^^^^^^^^");
                //         temp2.print();
                //         temp.print();
                //         puts("^^^^^^^^^^^^^^^^^^^^^^^");

                //     }
                //     if(r.phase==8){
                //         Rubik temp=pop[0];

                //         // (Uu)2 (Ll)2' U2 l2' U2 (Ll)2' (Uu)2 
                //         puts("^^^^^^^^^^^^^^^^^^^^^^^");
                //         f(temp,"UUuuLLllUUllUULLllUUuu");
                //         temp.print();
                //         puts("^^^^^^^^^^^^^^^^^^^^^^^");

                //     }
                //     population_size+=2000;
                //     generation+=100;
                    phase--;
                    continue;
                //     // break;
                }
                // if(DISPLAY){
                //     printf("remain: %d\n",(int)pop.size());        
                //     printf("fitness: %d\n",(int)pop[0].value);        
                //     pop[0].print();
                //     printf("end phase : %d\n",phase);
                // }

                //reset the fitness value
                int min_step = 1000;
                for(auto x:pop)
                    min_step = min(min_step,x.op_cnt);
                printf("min steps: %d\n", min_step);
                total_step_each_phase[phase]+=(min_step-prev_min_step);
                step_each_phases[t][phase]=min_step-prev_min_step;
                prev_min_step = min_step;
                
                if(phase<8){
                    for(auto &x:pop)
                        x.fitness();
                    for(auto &x:pop)
                        x.pre_op_cnt=x.op_cnt;
                }
                else{
                    total_step+=min_step;
                }
                
            }
            printf("\033[0;32m");
            puts("\rfinish                                 ");
            printf("\033[0m"); // Reset color
            // printf("%d\n",phase);
            // fail_phase[phase]++;
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;
            durations.push_back(duration.count());
            totalDuration += duration.count();
            printf("Execution time: %f seconds\n", duration.count());

        }

    }
    double averageDuration = totalDuration/(double)TEST_NUM;
    printf("Execution time: %f seconds for %d testcases\n", totalDuration,TEST_NUM);
    printf("Average execution time: %f seconds\n", averageDuration);
    double squaredDifferencesSum = 0.0;
    for (double duration : durations) {
        squaredDifferencesSum += (duration - averageDuration) * (duration - averageDuration);
    }
    printf("Standard deviation: %f seconds\n", sqrt(squaredDifferencesSum / TEST_NUM));
    printf("Average steps : %d\n", total_step/TEST_NUM);
    // calculate standard deviation of steps each phases 
    for(int phase_num=1; phase_num<=8; phase_num++){
        double average = total_step_each_phase[phase_num]/(double)TEST_NUM;
        double squaredDifferencesSum = 0;
        for (int i=0;i<TEST_NUM;i++) {
            squaredDifferencesSum += (step_each_phases[i][phase_num] - average) * (step_each_phases[i][phase_num] - average);
        }
        printf("Standard deviation of step of phase %d: %f\n", phase_num, sqrt(squaredDifferencesSum / TEST_NUM));
        printf("Average step of phase %d: %d\n", phase_num, total_step_each_phase[phase_num]/TEST_NUM);

    }
    // printf("fail phase : %d\n", fail_phase

        

    // for(int i=1;i<10;i++){
    //     printf("%d : %d\n",i,fail_phase[i]);
    // }
}