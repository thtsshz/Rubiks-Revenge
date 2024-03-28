#include<iostream>
#include<vector>
#include<string>
using namespace std;
/*
FRLBUDfrlbud
X2: *2
X': *3
Xw: xX
Xw2: xXxX
Xw': xxxXXX
example: 
Fw' = fffFFF
Fw2 = ffFF
L2 = LL
*/


// https://shengyu7697.github.io/cpp-string-split/
const std::vector<std::string> split(const std::string& str, const std::string& pattern) {
    std::vector<std::string> result;
    std::string::size_type begin, end;

    end = str.find(pattern);
    begin = 0;

    while (end != std::string::npos) {
        if (end - begin != 0) {
            result.push_back(str.substr(begin, end-begin)); 
        }    
        begin = end + pattern.size();
        end = str.find(pattern, begin);
    }

    if (begin != str.length()) {
        result.push_back(str.substr(begin));
    }
    return result;        
}

string transform_oper(string oper){
    string ret ="";
    if(oper.length() == 1){
        ret.append(oper);
    }
    else if(oper[oper.length()-1] == 'w'){
        ret.push_back(oper[0]);
        ret.push_back(tolower(oper[0]));
    }
    else if(oper[oper.length()-1] == '2'){
        string transformed = transform_oper(oper.substr(0, oper.length()-1));
        ret.append(transformed + transformed);
    }    
    else if(oper[oper.length()-1] == '\''){
        string transformed = transform_oper(oper.substr(0, oper.length()-1));
        ret.append(transformed + transformed + transformed);
    }
    return ret;
}
// U' L D F' L2 F' R B' L' R2 D L2 U B2 U D' L2 D' L' Uw2 Fw2 B U2 Rw2 Uw2 F D F2 U2 Fw2 F2 Rw U Fw2 F2 D' B Uw Fw' L' R2 D2 Fw F2
string transformer(string input){
    string pattern = " ";
    string ret = "";
    std::vector<std::string> opers = split(input, pattern);

    for(auto oper : opers){
        ret.append(transform_oper(oper));
    }
    return ret;
};


int main(){
    string input;
    cout << "Enter a sentence: ";
    getline(cin, input);
    cout << transformer(input);
    return 0;
}