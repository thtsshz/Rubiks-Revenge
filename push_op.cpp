// #pragma once
#include<stdio.h>
#include<iostream>
#include<vector>
#include<stack>
#include<unordered_map>
#include<algorithm>
#include<string.h>
#include<string>
#include<assert.h>
#include <ctype.h>
using namespace std;    

vector<pair<char, int>> st;
unordered_map<char, string> op_map;
unordered_map<int, char> number_map;

int op_cnt;
int st_ptr; // index of the last element in the stack
int wca_ptr;
vector<string> wca_op;

void twopairtowca(pair<char,int> fp, pair<char,int> sp){
    if(fp.second && sp.second){
        string temp = "";
        temp.push_back(toupper(fp.first));
        temp.push_back('w');
        temp.push_back(number_map[min(fp.second, sp.second)]);
        wca_op.push_back(temp);
    }
    if(fp.second - min(fp.second, sp.second)){
        string temp = "";
        temp.push_back(fp.first);
        temp.push_back(number_map[fp.second-min(fp.second, sp.second)]);
        wca_op.push_back(temp);
    }
    else if(sp.second - min(fp.second, sp.second)){
        string temp = "";
        temp.push_back(fp.first);
        temp.push_back('w');
        temp.push_back(number_map[sp.second-min(fp.second, sp.second)]);
        wca_op.push_back(temp);
        temp.push_back(fp.first);
        temp.push_back('\'');
        temp.push_back(number_map[sp.second-min(fp.second, sp.second)]);
        wca_op.push_back(temp);
    }
}


void st_to_wca() {
    cout << wca_ptr<<' '<<st_ptr << endl;
    bool visited[4] = {0};
    sort(st.begin() + wca_ptr, st.begin() + st_ptr);
    pair<char, int> fp={'\0', 0};
    pair<char, int> sp={'\0', 0};

    for(int k=wca_ptr; k<st_ptr; k++){
        if(visited[k-wca_ptr]) continue;
        visited[k-wca_ptr] = true;
        fp=st[k];
        cout << "fp: " << fp.first << endl;
        
        for(int i=k+1; i<st_ptr; i++){
            if(visited[i-k]) continue;
            visited[i-k] = true;
            if(toupper(fp.first) == toupper(st[i].first)){
                sp = st[i];
                cout << "sp: " << sp.first << endl;
                break;
            }
        }
        twopairtowca(fp, sp);
        pair<char, int> fp={'\0', 0};
        pair<char, int> sp={'\0', 0};
    }
    wca_ptr = st_ptr;

    return;
}

void push_op(const char c){
    // LR UD FB
    int i=0;
    for(i=st_ptr; i>=0; i--){
        if(st[i].first == c){
            st[i].second++;
            if(st[i].second == 4){
                st.erase(st.begin()+i);
                st_ptr--;
            }
            break;
        }
        else if(op_map[c].find(st[i].first) == string::npos){
            st_ptr++;
            if(i==st_ptr-1)
                st_to_wca();
            st.push_back(make_pair(c, 1));
            break;
        }        
    }
    if(i==-1){
        // st_to_wca();
        st_ptr++;
        st.push_back(make_pair(c, 1));
    }
}
void op_map_init(){
    op_map['L'] = "LlRr";
    op_map['l'] = "LlRr";
    op_map['R'] = "LlRr";
    op_map['r'] = "LlRr";
    op_map['F'] = "FfBb";
    op_map['f'] = "FfBb";
    op_map['B'] = "FfBb";
    op_map['b'] = "FfBb";
    op_map['U'] = "UuDd";
    op_map['u'] = "UuDd";
    op_map['D'] = "UuDd";
    op_map['d'] = "UuDd";

    number_map[1] = '\0';
    number_map[2] =  '2';
    number_map[3] = '\'';
}


int main(){
    string opers;
    while(cin >> opers){
        st_ptr = -1;
        wca_ptr = 0;
        op_map_init();
        st.clear();
        for (auto c:opers)
            push_op(c);
        st_to_wca();
        for(auto p:st){
            printf("%c %d\n", p.first, p.second);
        }
        cout << "total step: " << wca_op.size() << endl;
        for(auto s: wca_op){
            cout << s <<endl;
        }
    }

}