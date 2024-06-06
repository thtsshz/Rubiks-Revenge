/*
    File Name : rubik.h
    This file contains all the operations of the cube and the
   design of the fitness function.
*/
#pragma once
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;
int rotation[16] = {12, 8,  4, 0, 13, 9,  5, 1,
                    14, 10, 6, 2, 15, 11, 7, 3};  // clockwise rotation
char hash_map[7] = "OGRBWY";
int oppo[7] = {2, 3, 0, 1, 5, 4};
int path[8][4] = {2, 5, 0, 4, 3, 5, 1, 4, 1, 5, 3, 4,
                  0, 5, 2, 4, 0, 3, 2, 1, 1, 2, 3, 0};
int indices[12][4][4] = {
    {{0, 4, 8, 12}, {3, 2, 1, 0}, {15, 11, 7, 3}, {12, 13, 14, 15}},
    {{1, 5, 9, 13}, {7, 6, 5, 4}, {14, 10, 6, 2}, {8, 9, 10, 11}},
    {{12, 8, 4, 0}, {3, 7, 11, 15}, {3, 7, 11, 15}, {3, 7, 11, 15}},
    {{13, 9, 5, 1}, {2, 6, 10, 14}, {2, 6, 10, 14}, {2, 6, 10, 14}},
    {{0, 4, 8, 12}, {0, 4, 8, 12}, {15, 11, 7, 3}, {0, 4, 8, 12}},
    {{1, 5, 9, 13}, {1, 5, 9, 13}, {14, 10, 6, 2}, {1, 5, 9, 13}},
    {{0, 4, 8, 12}, {12, 13, 14, 15}, {15, 11, 7, 3}, {3, 2, 1, 0}},
    {{1, 5, 9, 13}, {8, 9, 10, 11}, {14, 10, 6, 2}, {7, 6, 5, 4}},
    {{0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}},
    {{4, 5, 6, 7}, {4, 5, 6, 7}, {4, 5, 6, 7}, {4, 5, 6, 7}},
    {{12, 13, 14, 15}, {12, 13, 14, 15}, {12, 13, 14, 15}, {12, 13, 14, 15}},
    {{8, 9, 10, 11}, {8, 9, 10, 11}, {8, 9, 10, 11}, {8, 9, 10, 11}}};
int max_fitness[10] = {0, 8, 24, 16, 26, 31, 28, 14, 6, 0};
int weight_l[10] = {0, 4, 4, 6, 14, 4, 10, 4, 7, 0};
int weight_r[10] = {0, 1, 1, 1, 1, 1, 1, 1, 1, 0};
unordered_map<char, string> op_map;
unordered_map<int, char> number_map;

vector<int> tmp;
// maintain a vector<pair<char, int>> (oper, times) seen as stack
// parallel motion(independent)
// operations could be checked in operation()
// WCA:
// https://www.worldcubeassociation.org/regulations/translations/chinese-traditional/#article-12-notation

struct Rubik {
    /* target state:
        G is the abbreviation for GREEN, which is our front face.

          WWWW
          WWWW
          WWWW
          WWWW

        OOOO  GGGG  RRRR  BBBB
        OOOO  GGGG  RRRR  BBBB
    OOOO  GGGG  RRRR  BBBB
    OOOO  GGGG  RRRR  BBBB

                  YYYY
          YYYY
          YYYY
          YYYY

    */
    // 6 faces  [0,1,2,3,4,5] -> [O,G,R,B,W,Y]
    int state[6][16] = {0};
    int phase;
    int value;

    // vector<pair<char, int>> st;
    pair<char, int> st[5];
    int st_ptr = -1;  // index of the last element in the stack
    int wca_ptr = 0;
    int pre_op_cnt = 0;
    int op_cnt = 0;
    // vector<string> wca_op;
    Rubik() {  // initialize with target state
        phase = 1;
        value = 0;
        st_ptr = -1;  // index of the last element in the stack
        wca_ptr = 0;
        op_cnt = pre_op_cnt = 0;
        // st.clear();
        // wca_op.clear();
        for (int i = 0; i < 6; i++)
            for (int j = 0; j < 16; j++) state[i][j] = i;
    }
    Rubik(char *op) {  // initialize with sequence of operations
        phase = 1;
        value = 0;
        st_ptr = -1;  // index of the last element in the stack
        wca_ptr = 0;
        op_cnt = pre_op_cnt = 0;
        // st.clear();
        // wca_op.clear();
        for (int i = 0; i < 6; i++)
            for (int j = 0; j < 16; j++) state[i][j] = i;

        int len = strlen(op);
        for (int i = 0; i < len; i++) {
            char tmp[3];
            if (i + 1 < len && isdigit(op[i + 1])) {
                tmp[0] = op[i];
                tmp[1] = op[i + 1];
                i++;
                tmp[2] = '\0';
            } else {
                tmp[0] = op[i];
                tmp[1] = '\0';
            }
            operation(tmp);
        }
        st_ptr = -1;  // reset the value
        wca_ptr = 0;
        op_cnt = pre_op_cnt = 0;
    }
    bool operator<(const Rubik &other) const {
        // weight_l[] : the weight of the fitness function in each phase
        // weight_r[] : the weight of the step(penalty) in each phase
        int first =
            weight_l[phase] * value - weight_r[phase] * (op_cnt - pre_op_cnt);
        int second = weight_l[phase] * other.value -
                     weight_r[phase] * (other.op_cnt - other.pre_op_cnt);
        return first > second;
    }

    inline char hash(int v) { return hash_map[v]; }
    void colorful_text(char ch) {
        switch (ch) {
            case 'O':
                printf("\033[38;2;255;165;0m%c\033[0m", ch);  // Orange
                break;
            case 'G':
                printf("\033[32m%c\033[0m", ch);  // Green
                break;
            case 'R':
                printf("\033[31m%c\033[0m", ch);  // Red
                break;
            case 'B':
                printf("\033[34m%c\033[0m", ch);  // Blue
                break;
            case 'W':
                printf("\033[37m%c\033[0m", ch);  // White
                break;
            case 'Y':
                printf("\033[33m%c\033[0m", ch);  // Yellow
                break;
            default:
                printf("%c", ch);  // Print as is for other characters
                break;
        }
    }

    void print() {
        // puts("************");
        for (int i = 0; i < 4; i++) {
            printf("      ");
            for (int j = 0; j < 4; j++)
                colorful_text(hash(state[4][i * 4 + j]));
            putchar('\n');
        }
        puts("");
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    colorful_text(hash(state[j][i * 4 + k]));
                }
                printf("  ");
            }
            putchar('\n');
        }
        puts("");
        for (int i = 0; i < 4; i++) {
            printf("      ");
            for (int j = 0; j < 4; j++)
                colorful_text(hash(state[5][i * 4 + j]));
            putchar('\n');
        }
    }
    /*
        Adopt the World Cube Association (WCA) single-turn notation:
        In each operation, we shift the state of four faces once, which can be
       efficiently implemented by swapping the states of 3 faces. For capital
       letters, which represent the outer face turn, the corresponding face must
       be rotated 90 degrees additionally.
    */
    void F() {
        for (int i = 3; i >= 1; i--)
            for (int j = 0; j < 4; j++)
                swap(state[path[0][i]][indices[0][i][j]],
                     state[path[0][i - 1]][indices[0][i - 1][j]]);
        tmp.assign(state[1], state[1] + 16);
        for (int i = 0; i < 16; i++) state[1][i] = tmp[rotation[i]];
    }
    void f() {
        for (int i = 3; i >= 1; i--)
            for (int j = 0; j < 4; j++)
                swap(state[path[0][i]][indices[1][i][j]],
                     state[path[0][i - 1]][indices[1][i - 1][j]]);
    }

    void R() {
        for (int i = 3; i >= 1; i--)
            for (int j = 0; j < 4; j++)
                swap(state[path[1][i]][indices[2][i][j]],
                     state[path[1][i - 1]][indices[2][i - 1][j]]);
        tmp.assign(state[2], state[2] + 16);
        for (int i = 0; i < 16; i++) state[2][i] = tmp[rotation[i]];
    }
    void r() {
        for (int i = 3; i >= 1; i--)
            for (int j = 0; j < 4; j++)
                swap(state[path[1][i]][indices[3][i][j]],
                     state[path[1][i - 1]][indices[3][i - 1][j]]);
    }

    void L() {
        for (int i = 3; i >= 1; i--)
            for (int j = 0; j < 4; j++)
                swap(state[path[2][i]][indices[4][i][j]],
                     state[path[2][i - 1]][indices[4][i - 1][j]]);
        tmp.assign(state[0], state[0] + 16);
        for (int i = 0; i < 16; i++) state[0][i] = tmp[rotation[i]];
    }
    void l() {
        for (int i = 3; i >= 1; i--)
            for (int j = 0; j < 4; j++)
                swap(state[path[2][i]][indices[5][i][j]],
                     state[path[2][i - 1]][indices[5][i - 1][j]]);
    }

    void B() {
        for (int i = 3; i >= 1; i--)
            for (int j = 0; j < 4; j++)
                swap(state[path[3][i]][indices[6][i][j]],
                     state[path[3][i - 1]][indices[6][i - 1][j]]);
        tmp.assign(state[3], state[3] + 16);
        for (int i = 0; i < 16; i++) state[3][i] = tmp[rotation[i]];
    }
    void b() {
        for (int i = 3; i >= 1; i--)
            for (int j = 0; j < 4; j++)
                swap(state[path[3][i]][indices[7][i][j]],
                     state[path[3][i - 1]][indices[7][i - 1][j]]);
    }

    void U() {
        for (int i = 3; i >= 1; i--)
            for (int j = 0; j < 4; j++)
                swap(state[path[4][i]][indices[8][i][j]],
                     state[path[4][i - 1]][indices[8][i - 1][j]]);
        tmp.assign(state[4], state[4] + 16);
        for (int i = 0; i < 16; i++) state[4][i] = tmp[rotation[i]];
    }
    void u() {
        for (int i = 3; i >= 1; i--)
            for (int j = 0; j < 4; j++)
                swap(state[path[4][i]][indices[9][i][j]],
                     state[path[4][i - 1]][indices[9][i - 1][j]]);
    }

    void D() {
        for (int i = 3; i >= 1; i--)
            for (int j = 0; j < 4; j++)
                swap(state[path[5][i]][indices[10][i][j]],
                     state[path[5][i - 1]][indices[10][i - 1][j]]);
        tmp.assign(state[5], state[5] + 16);
        for (int i = 0; i < 16; i++) state[5][i] = tmp[rotation[i]];
    }

    void d() {
        for (int i = 3; i >= 1; i--)
            for (int j = 0; j < 4; j++)
                swap(state[path[5][i]][indices[11][i][j]],
                     state[path[5][i - 1]][indices[11][i - 1][j]]);
    }

    void twopairtowca(pair<char, int> fp, pair<char, int> sp) {
        if (fp.second && sp.second) {
            // string temp = "";
            // temp.push_back(toupper(fp.first));
            // temp.push_back('w');
            // temp.push_back(number_map[min(fp.second, sp.second)]);
            // wca_op.push_back(temp);
            op_cnt++;
        }
        if (fp.second - min(fp.second, sp.second)) {
            // string temp = "";
            // temp.push_back(fp.first);
            // temp.push_back(number_map[fp.second-min(fp.second, sp.second)]);
            // wca_op.push_back(temp);
            op_cnt++;
        } else if (sp.second - min(fp.second, sp.second)) {
            // string temp = "";
            // temp.push_back(fp.first);
            // temp.push_back('w');
            // temp.push_back(number_map[sp.second-min(fp.second, sp.second)]);
            // wca_op.push_back(temp);
            // temp = "";
            // temp.push_back(fp.first);
            // temp.push_back('\'');
            // temp.push_back(number_map[sp.second-min(fp.second, sp.second)]);
            // wca_op.push_back(temp);
            op_cnt += 2;
        }
    }

    void st_to_wca() {
        bool visited[4] = {0};
        sort(st + wca_ptr, st + st_ptr,
             [](const pair<char, int> &a, const pair<char, int> &b) {
                 return a.first < b.first;
             });
        pair<char, int> fp = {'\0', 0};
        pair<char, int> sp = {'\0', 0};

        for (int k = wca_ptr; k < st_ptr; k++) {
            if (visited[k - wca_ptr]) continue;
            visited[k - wca_ptr] = true;
            fp = st[k];

            for (int i = k + 1; i < st_ptr; i++) {
                if (visited[i - k]) continue;
                if (toupper(fp.first) == toupper(st[i].first)) {
                    visited[i - k] = true;
                    sp = st[i];
                    break;
                }
            }
            twopairtowca(fp, sp);
            pair<char, int> fp = {'\0', 0};
            pair<char, int> sp = {'\0', 0};
        }
        // wca_ptr = st_ptr;
        wca_ptr = 0;
        st_ptr = 0;

        return;
    }
    void push_op(const char c) {
        // LR UD FB
        // puts("push_op");
        int i = 0;
        for (i = st_ptr; i >= 0; i--) {
            if (st[i].first == c) {
                st[i].second++;
                if (st[i].second == 4) {
                    // st.erase(st.begin()+i);
                    for (int j = i + 1; j <= st_ptr; j++) {
                        st[j - 1] = st[j];
                    }
                    st_ptr--;
                }
                break;
            } else if (op_map[c].find(st[i].first) == string::npos) {
                st_ptr++;
                if (i == st_ptr - 1) st_to_wca();
                st[st_ptr] = (make_pair(c, 1));
                break;
            }
        }
        if (i == -1) {
            // st_to_wca();
            st_ptr++;
            st[st_ptr] = (make_pair(c, 1));
        }
    }

    // perforn the corresponding operation to current individual
    void operation(const char *str) {
        push_op(str[0]);
        switch (str[0]) {
            case 'L':
                L();
                if (str[1] == '2') L();
                break;
            case 'R':
                R();
                if (str[1] == '2') R();
                break;
            case 'F':
                F();
                if (str[1] == '2') F();
                break;
            case 'B':
                B();
                if (str[1] == '2') B();
                break;
            case 'U':
                U();
                if (str[1] == '2') U();
                break;
            case 'D':
                D();
                if (str[1] == '2') D();
                break;
            case 'l':
                l();
                if (str[1] == '2') l();
                break;
            case 'r':
                r();
                if (str[1] == '2') r();
                break;
            case 'f':
                f();
                if (str[1] == '2') f();
                break;
            case 'b':
                b();
                if (str[1] == '2') b();
                break;
            case 'u':
                u();
                if (str[1] == '2') u();
                break;
            case 'd':
                d();
                if (str[1] == '2') d();
                break;
            default:  // parsing error
                assert(0);
                break;
        }
    }
    // Parity Check(E) on paper
    bool parity_U(int a, int b) {
        char up = hash(a), down = hash(b);
        if (up == 'O' || up == 'R') return 0;
        if (up == 'G' || up == 'B') {
            if (down == 'Y' || down == 'W') return 0;
        }
        return 1;
    }
    bool check(int color, int target) {
        return color == target || color == oppo[target];
    }
    /*
        This function is used to check whether the current individual is able to
       enter the next phase, which is similar but different from the fitness
       below. It is only called when the expected generation number is reached.
    */
    void phase_check() {
        if (phase == 1) {
            bool right = 1;
            for (auto x : {5, 6, 9, 10}) {
                right &= (state[0][x] == 0 || state[0][x] == 2);
                right &= (state[2][x] == 0 || state[2][x] == 2);
            }
            phase += right;
        } else if (phase == 2) {
            bool right = 1;

            for (auto x : {5, 6, 9, 10}) {  // index of center faces
                right &= (state[0][x] == 0 || state[0][x] == 2);
                right &= (state[2][x] == 0 || state[2][x] == 2);

                right &= (state[1][x] == 1 || state[1][x] == 3);
                right &= (state[3][x] == 1 || state[3][x] == 3);

                right &= (state[4][x] == 4 || state[4][x] == 5);
                right &= (state[5][x] == 4 || state[5][x] == 5);
            }
            phase += right;

        } else if (phase == 3) {
            bool right = 1;
            for (auto x : {0, 1}) {
                right &= (state[x][5] == state[x][9]) &&
                         (state[x][5] == x || state[x][5] == 2 + x);
                right &= (state[x][6] == state[x][10]) &&
                         (state[x][6] == x || state[x][10] == 2 + x);
            }
            for (auto x : {2, 3}) {
                right &= (state[x][5] == state[x][9]) &&
                         (state[x][5] == x || state[x][5] == x - 2);
                right &= (state[x][6] == state[x][10]) &&
                         (state[x][6] == x || state[x][10] == x - 2);
            }
            // up face
            bool up_1 = ((state[4][5] == state[4][6]) &&
                         (state[4][5] == 4 || state[4][5] == 5)) &&
                        ((state[4][9] == state[4][10]) &&
                         (state[4][9] == 4 || state[4][9] == 5));
            bool up_2 = ((state[4][5] == state[4][9]) &&
                         (state[4][5] == 4 || state[4][5] == 5)) &&
                        ((state[4][6] == state[4][10]) &&
                         (state[4][6] == 4 || state[4][6] == 5));

            right &= up_1 || up_2;

            // down face
            bool down_1 = ((state[5][5] == state[5][6]) &&
                           (state[5][5] == 4 || state[5][5] == 5)) &&
                          ((state[5][9] == state[5][10]) &&
                           (state[5][9] == 4 || state[5][9] == 5));
            bool down_2 = ((state[5][5] == state[5][9]) &&
                           (state[5][5] == 4 || state[5][5] == 5)) &&
                          ((state[5][6] == state[5][10]) &&
                           (state[5][6] == 4 || state[5][6] == 5));

            right &= down_1 || down_2;

            // middle edge
            right &=
                (state[0][4] == state[0][8]) && (state[3][7] == state[3][11]);

            for (auto x : {0, 1, 2})
                right &= (state[x][7] == state[x][11]) &&
                         (state[x + 1][4] == state[x + 1][8]);
            phase += right;
        } else if (phase == 4) {
            bool right = 1;
            for (int i = 0; i < 6; i++)
                right &= state[i][5] == state[i][6] &&
                         state[i][6] == state[i][9] &&
                         state[i][9] == state[i][10];

            // same as phase3
            right &=
                (state[0][4] == state[0][8]) && (state[3][7] == state[3][11]);

            for (auto x : {0, 1, 2})
                right &= (state[x][7] == state[x][11]) &&
                         (state[x + 1][4] == state[x + 1][8]);
            //
            // up
            right &= (state[0][1] == state[0][2]) && state[4][4] == state[4][8];
            right &=
                (state[1][1] == state[1][2]) && state[4][13] == state[4][14];
            right &=
                (state[2][1] == state[2][2]) && state[4][7] == state[4][11];
            right &= (state[3][1] == state[3][2]) && state[4][1] == state[4][2];
            //
            // down
            right &= state[0][13] == state[0][14] && state[5][4] == state[5][8];
            right &= state[1][13] == state[1][14] && state[5][1] == state[5][2];
            right &=
                state[2][13] == state[2][14] && state[5][7] == state[5][11];
            right &=
                state[3][13] == state[3][14] && state[5][13] == state[5][14];

            //

            phase += right;
        } else if (phase == 5) {
            bool right = 1;
            // //parity check for U faces
            right &= (parity_U(state[4][4], state[0][1]) &&
                      parity_U(state[4][8], state[0][2]));

            right &= (parity_U(state[4][13], state[1][1]) &&
                      parity_U(state[4][14], state[1][2]));

            right &= (parity_U(state[4][11], state[2][1]) &&
                      parity_U(state[4][7], state[2][2]));

            right &= (parity_U(state[4][2], state[3][1]) &&
                      parity_U(state[4][1], state[3][2]));

            // //parity check for D faces
            right &= (parity_U(state[5][4], state[0][14]) &&
                      parity_U(state[5][8], state[0][13]));

            right &= (parity_U(state[5][13], state[3][14]) &&
                      parity_U(state[5][14], state[3][13]));

            right &= (parity_U(state[5][11], state[2][14]) &&
                      parity_U(state[5][7], state[2][13]));

            right &= (parity_U(state[5][2], state[1][14]) &&
                      parity_U(state[5][1], state[1][13]));

            // //parity check for F faces
            right &= (parity_U(state[1][4], state[0][7]) &&
                      parity_U(state[1][8], state[0][11]));

            right &= (parity_U(state[1][7], state[2][4]) &&
                      parity_U(state[1][11], state[2][8]));

            // //parity check for B faces
            right &= (parity_U(state[3][4], state[2][7]) &&
                      parity_U(state[3][8], state[2][11]));

            right &= (parity_U(state[3][7], state[0][4]) &&
                      parity_U(state[3][11], state[0][8]));
            phase += right;

        } else if (phase == 6) {
            bool right = 1;
            right &= (parity_U(state[4][4], state[0][1]) &&
                      parity_U(state[4][8], state[0][2]));

            right &= (parity_U(state[4][13], state[1][1]) &&
                      parity_U(state[4][14], state[1][2]));

            right &= (parity_U(state[4][11], state[2][1]) &&
                      parity_U(state[4][7], state[2][2]));

            right &= (parity_U(state[4][2], state[3][1]) &&
                      parity_U(state[4][1], state[3][2]));

            // //parity check for D faces
            right &= (parity_U(state[5][4], state[0][14]) &&
                      parity_U(state[5][8], state[0][13]));

            right &= (parity_U(state[5][13], state[3][14]) &&
                      parity_U(state[5][14], state[3][13]));

            right &= (parity_U(state[5][11], state[2][14]) &&
                      parity_U(state[5][7], state[2][13]));

            right &= (parity_U(state[5][2], state[1][14]) &&
                      parity_U(state[5][1], state[1][13]));

            // //parity check for F faces
            right &= (parity_U(state[1][4], state[0][7]) &&
                      parity_U(state[1][8], state[0][11]));

            right &= (parity_U(state[1][7], state[2][4]) &&
                      parity_U(state[1][11], state[2][8]));

            // //parity check for B faces
            right &= (parity_U(state[3][4], state[2][7]) &&
                      parity_U(state[3][8], state[2][11]));

            right &= (parity_U(state[3][7], state[0][4]) &&
                      parity_U(state[3][11], state[0][8]));

            for (auto x : {4, 5}) {
                for (int j = 0; j < 16; j++) {
                    right &= (state[x][j] == x || state[x][j] == oppo[x]);
                }
            }
            phase += right;
        } else if (phase == 7) {
            bool right = 1;
            for (int i = 0; i < 6; i++) {
                for (int j = 0; j < 16; j++) {
                    right &= state[i][j] == i || state[i][j] == oppo[i];
                }
            }
            phase += right;
        } else if (phase == 8) {
            bool right = 1;
            for (auto x : {0, 2, 4, 5}) {
                for (int j = 0; j < 16; j++) {
                    right &= state[x][j] == x;
                }
            }
            phase += right;
        } else {
            assert(0);
        }
    }
    /*
        The important function to calculate the fitness value of the current
       individual. To save time, we won't update the fitness value after each
       operation. Make sure to call this function once you need to access the
       value of the Rubik's Cube !!!
    */
    void fitness() {
        value = 0;
        if (phase == 1) {
            for (auto x : {5, 6, 9, 10}) {  // index of center faces
                value += (state[0][x] == 0 || state[0][x] == 2);
                value += (state[2][x] == 0 || state[2][x] == 2);
            }

        } else if (phase == 2) {
            for (auto x : {5, 6, 9, 10}) {
                value += (state[0][x] == 0 || state[0][x] == 2);
                value += (state[2][x] == 0 || state[2][x] == 2);

                value += (state[1][x] == 1 || state[1][x] == 3);
                value += (state[3][x] == 1 || state[3][x] == 3);

                value += (state[4][x] == 4 || state[4][x] == 5);
                value += (state[5][x] == 4 || state[5][x] == 5);
            }
        } else if (phase == 3) {
            for (auto x : {0, 1}) {
                value += (state[x][5] == state[x][9]) &&
                         (state[x][5] == x || state[x][5] == 2 + x);
                value += (state[x][6] == state[x][10]) &&
                         (state[x][6] == x || state[x][10] == 2 + x);
            }
            for (auto x : {2, 3}) {
                value += (state[x][5] == state[x][9]) &&
                         (state[x][5] == x || state[x][5] == x - 2);
                value += (state[x][6] == state[x][10]) &&
                         (state[x][6] == x || state[x][10] == x - 2);
            }
            // up face
            bool up_1 = ((state[4][5] == state[4][6]) &&
                         (state[4][5] == 4 || state[4][5] == 5)) &&
                        ((state[4][9] == state[4][10]) &&
                         (state[4][9] == 4 || state[4][9] == 5));
            bool up_2 = ((state[4][5] == state[4][9]) &&
                         (state[4][5] == 4 || state[4][5] == 5)) &&
                        ((state[4][6] == state[4][10]) &&
                         (state[4][6] == 4 || state[4][6] == 5));

            value += (up_1 || up_2) * 2;

            // down face
            bool down_1 = ((state[5][5] == state[5][6]) &&
                           (state[5][5] == 4 || state[5][5] == 5)) &&
                          ((state[5][9] == state[5][10]) &&
                           (state[5][9] == 4 || state[5][9] == 5));
            bool down_2 = ((state[5][5] == state[5][9]) &&
                           (state[5][5] == 4 || state[5][5] == 5)) &&
                          ((state[5][6] == state[5][10]) &&
                           (state[5][6] == 4 || state[5][6] == 5));

            value += (down_1 || down_2) * 2;

            // middle edge
            value +=
                (state[0][4] == state[0][8]) && (state[3][7] == state[3][11]);

            for (auto x : {0, 1, 2})
                value += (state[x][7] == state[x][11]) &&
                         (state[x + 1][4] == state[x + 1][8]);
        } else if (phase == 4) {
            int tmp;
            for (int i = 0; i < 6; i++)
                value += state[i][5] == state[i][6] &&
                         state[i][6] == state[i][9] &&
                         state[i][9] == state[i][10] && state[i][10] == i;
            tmp = value;
            // same as phase3
            value +=
                (state[0][4] == state[0][8]) && (state[3][7] == state[3][11]);

            for (auto x : {0, 1, 2})
                value += (state[x][7] == state[x][11]) &&
                         (state[x + 1][4] == state[x + 1][8]);
            // up
            // value += (state[0][1]==state[0][2])&& state[4][4]==state[4][8];
            value += (state[1][1] == state[1][2]) &&
                     state[4][13] == state[4][14];  //
            // value += (state[2][1]==state[2][2])&& state[4][7]==state[4][11];
            value += (state[3][1] == state[3][2]) && state[4][1] == state[4][2];

            // down
            value += state[0][13] == state[0][14] && state[5][4] == state[5][8];
            value +=
                state[1][13] == state[1][14] && state[5][1] == state[5][2];  //
            value +=
                state[2][13] == state[2][14] && state[5][7] == state[5][11];
            value +=
                state[3][13] == state[3][14] && state[5][13] == state[5][14];
            // value = (value-tmp)*2 +tmp;
            tmp = value;
            value += 10 * (tmp == 16 && (state[0][1] == state[0][2]) &&
                           state[4][4] == state[4][8] &&
                           (state[2][1] == state[2][2]) &&
                           state[4][7] == state[4][11]);
        } else if (phase == 5) {
            value += (parity_U(state[4][4], state[0][1]) &&
                      parity_U(state[4][8], state[0][2]));

            // value+=(parity_U(state[4][13],state[1][1])&&parity_U(state[4][14],state[1][2]));

            value += (parity_U(state[4][11], state[2][1]) &&
                      parity_U(state[4][7], state[2][2]));

            value += (parity_U(state[4][2], state[3][1]) &&
                      parity_U(state[4][1], state[3][2]));

            // //parity check for D faces
            value += (parity_U(state[5][4], state[0][14]) &&
                      parity_U(state[5][8], state[0][13]));

            value += (parity_U(state[5][13], state[3][14]) &&
                      parity_U(state[5][14], state[3][13]));

            value += (parity_U(state[5][11], state[2][14]) &&
                      parity_U(state[5][7], state[2][13]));

            value += (parity_U(state[5][2], state[1][14]) &&
                      parity_U(state[5][1], state[1][13]));

            // //parity check for F faces
            value += (parity_U(state[1][4], state[0][7]) &&
                      parity_U(state[1][8], state[0][11]));

            value += (parity_U(state[1][7], state[2][4]) &&
                      parity_U(state[1][11], state[2][8]));

            // //parity check for B faces
            value += (parity_U(state[3][4], state[2][7]) &&
                      parity_U(state[3][8], state[2][11]));

            value += (parity_U(state[3][7], state[0][4]) &&
                      parity_U(state[3][11], state[0][8]));
            int tmp = value;
            value += 20 * (tmp == 11 && (parity_U(state[4][13], state[1][1]) &&
                                         parity_U(state[4][14], state[1][2])));

        } else if (phase == 6) {
            value += (parity_U(state[4][4], state[0][1]) &&
                      parity_U(state[4][8], state[0][2]));

            value += (parity_U(state[4][13], state[1][1]) &&
                      parity_U(state[4][14], state[1][2]));

            value += (parity_U(state[4][11], state[2][1]) &&
                      parity_U(state[4][7], state[2][2]));

            value += (parity_U(state[4][2], state[3][1]) &&
                      parity_U(state[4][1], state[3][2]));

            // //parity check for D faces
            value += (parity_U(state[5][4], state[0][14]) &&
                      parity_U(state[5][8], state[0][13]));

            value += (parity_U(state[5][13], state[3][14]) &&
                      parity_U(state[5][14], state[3][13]));

            value += (parity_U(state[5][11], state[2][14]) &&
                      parity_U(state[5][7], state[2][13]));

            value += (parity_U(state[5][2], state[1][14]) &&
                      parity_U(state[5][1], state[1][13]));

            // //parity check for F faces
            value += (parity_U(state[1][4], state[0][7]) &&
                      parity_U(state[1][8], state[0][11]));

            value += (parity_U(state[1][7], state[2][4]) &&
                      parity_U(state[1][11], state[2][8]));

            // //parity check for B faces
            value += (parity_U(state[3][4], state[2][7]) &&
                      parity_U(state[3][8], state[2][11]));

            value += (parity_U(state[3][7], state[0][4]) &&
                      parity_U(state[3][11], state[0][8]));
            // corner
            for (auto x : {0, 3, 12, 15}) {
                for (auto y : {4, 5}) {
                    value += 2 * ((state[y][x] == 4) || (state[y][x] == 5));
                }
            }
            // edge
            for (auto x : {1, 4, 7, 13}) {
                for (auto y : {4, 5}) {
                    if (y == 4 && x == 13) continue;
                    value += (state[y][x] == 4) || (state[y][x] == 5);
                }
            }
            if (value == 35 && ((state[4][13] == 4) || (state[4][13] == 5)))
                value = 45;
        } else if (phase == 7) {
            for (int i = 0; i < 6; i++) {
                int temp = 1;
                for (int j = 0; j < 16; j++) {
                    temp &= state[i][j] == i || state[i][j] == oppo[i];
                }
                value += temp;
            }
            // U
            int cur = value;
            value += check(state[4][0], 4) && check(state[0][0], 0) &&
                     check(state[3][3], 3);
            value += check(state[4][3], 4) && check(state[2][3], 2) &&
                     check(state[3][0], 3);
            value += check(state[1][0], 1) && check(state[0][3], 0) &&
                     check(state[4][12], 4);
            value += check(state[1][3], 1) && check(state[2][0], 2) &&
                     check(state[4][15], 4);

            // D
            value += check(state[5][12], 5) && check(state[0][12], 0) &&
                     check(state[3][15], 3);
            value += check(state[5][15], 5) && check(state[2][15], 2) &&
                     check(state[3][12], 3);

            value += check(state[1][12], 1) && check(state[0][15], 0) &&
                     check(state[5][0], 5);
            value += check(state[1][15], 1) && check(state[2][12], 2) &&
                     check(state[5][3], 5);
        } else if (phase == 8) {
            for (int i = 0; i < 6; i++) {
                int temp = 1;
                for (int j = 0; j < 16; j++) {
                    temp &= (state[i][j] == i);
                }
                value += temp;
            }
        } else {  // never reach
            assert(0);
        }
    }
};

void op_map_init() {
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
    number_map[2] = '2';
    number_map[3] = '\'';
}
void interative_mode(Rubik &obj) {
    puts("---interative_mode---");
    char str[1005];
    while (~scanf(" %s", str)) {
        puts("before:");
        obj.print();
        obj.operation(str);
        puts("after:");
        obj.print();
    }
    puts("---------end---------");
}

void operation_mode(Rubik &obj) {
    puts("Input string of operations seperated by space.");
    char str[1005];
    while (~scanf(" %s", str)) {
        for (int i = 0; str[i]; i++) {
            char tmp[2] = {str[i], '\0'};
            obj.operation(tmp);
        }
        obj.print();
    }
}