#include<stdio.h>
#include<vector>
#include<algorithm>
#include<assert.h>
using namespace std;
// comment
struct Rubik{
    /* target state:
        O is the abbreviation for orange, which is our front face.

          YYYY
          YYYY
          YYYY
          YYYY

    GGGG  OOOO  BBBB  RRRR
    GGGG  OOOO  BBBB  RRRR
    GGGG  OOOO  BBBB  RRRR
    GGGG  OOOO  BBBB  RRRR

          WWWW
          WWWW
          WWWW
          WWWW
    
    */
    // 6 faces  [0,1,2,3,4,5] -> [G,O,B,R,Y,W]
    
    int state[6][16]={0};
    // char hash_map[7] = "GOBRYW";
    char hash_map[7] = "OGRBWY";

    Rubik(){//target state
        for(int i=0;i<6;i++)
            for(int j=0;j<16;j++)
                state[i][j]=i;
    }
    char hash(int v){
        // if(!v)
        //     return 'G';
        // if(v==1)
        //     return 'O';
        // if(v==2)
        //     return 'B';
        // if(v==3)
        //     return 'R';
        // if(v==4)
        //     return 'Y';
        // return 'W';
        return hash_map[v];
    }
    vector<int> tmp;
    void print(){
        puts("************");
        for(int i=0;i<4;i++){
            printf("      ");
            for(int j=0;j<4;j++)
                printf("%c",hash(state[4][i*4+j]));
            putchar('\n');
        }
        puts("");
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                for(int k=0;k<4;k++){
                    printf("%c",hash(state[j][i*4+k]));
                }
                printf("  ");
            }
            putchar('\n');
        }
        puts("");        
        for(int i=0;i<4;i++){
            printf("      ");
            for(int j=0;j<4;j++)
                printf("%c",hash(state[5][i*4+j]));
            putchar('\n');
        }
    }
    // adopt World Cube Association(WCA) Single-Turn notation:
    void F(){
        int path[4]={2,5,0,4}; //bwgy
        int index[4][4]={{0,4,8,12},{0,1,2,3},{3,7,11,15},{12,13,14,15}};

        for(int i=3;i>=1;i--)
            for(int j=0;j<4;j++)
                swap(state[path[i]][index[i][j]],state[path[i-1]][index[i-1][j]]);
        int after[16]={12, 8, 4, 0, 13, 9, 5, 1, 14, 10, 6, 2, 15, 11, 7, 3};
        tmp.assign(state[1],state[1]+16);
        for(int i=0;i<15;i++)
            state[1][i]=tmp[after[i]];
    }
    void f(){
        int path[4]={2,5,0,4}; //bwgy
        int index[4][4]={{1,5,9,13},{4,5,6,7},{2,6,10,14},{8,9,10,11}};

        for(int i=3;i>=1;i--)
            for(int j=0;j<4;j++)
                swap(state[path[i]][index[i][j]],state[path[i-1]][index[i-1][j]]);
    }

    void R(){
        int path[4]={1,4,3,5}; //o y r w
        for(int i=3;i>=1;i--)
            for(int j=0;j<4;j++){
                int offset = path[i]==3?0:3;
                int offset2 = path[i-1]==3?0:3;
                swap(state[path[i]][offset+j*4],state[path[i-1]][offset2+j*4]);
            }
        int after[16]={12, 8, 4, 0, 13, 9, 5, 1, 14, 10, 6, 2, 15, 11, 7, 3};
        tmp.assign(state[2],state[2]+16);
        for(int i=0;i<15;i++)
            state[2][i]=tmp[after[i]];
    }
    void r(){
        int path[4]={1,4,3,5}; //o y r w
        for(int i=3;i>=1;i--)
            for(int j=0;j<4;j++){
            	int offset = path[i]==3?1:2;
                int offset2 = path[i-1]==3?1:2;
                swap(state[path[i]][offset+j*4],state[path[i-1]][offset2+j*4]);
            }
    }

    void L(){
        int path[4]={1,5,3,4}; //o w r y 
        for(int i=3;i>=1;i--)
            for(int j=0;j<4;j++){
                int offset = path[i]==3?3:0;
                int offset2 = path[i-1]==3?3:0;
                swap(state[path[i]][offset+j*4],state[path[i-1]][offset2+j*4]);
            }
        int after[16]={12, 8, 4, 0, 13, 9, 5, 1, 14, 10, 6, 2, 15, 11, 7, 3};
        tmp.assign(state[0],state[0]+16);
        for(int i=0;i<15;i++)
            state[0][i]=tmp[after[i]];
    }
    void l(){
        int path[4]={1,5,3,4}; //o w r y 
        for(int i=3;i>=1;i--)
            for(int j=0;j<4;j++){
                int offset = path[i]==3?2:1;
                int offset2 = path[i-1]==3?2:1;
                swap(state[path[i]][offset+j*4],state[path[i-1]][offset2+j*4]);
            }
    }
    
    void B(){
        int path[4]={0,5,2,4}; //g w b y 
        int index[4][4]={{0,4,8,12},{12,13,14,15},{3,7,11,15},{0,1,2,3}};
        for(int i=3;i>=1;i--)
            for(int j=0;j<4;j++){
                swap(state[path[i]][index[i][j]],state[path[i-1]][index[i-1][j]]);
            }
        int after[16]={12, 8, 4, 0, 13, 9, 5, 1, 14, 10, 6, 2, 15, 11, 7, 3};
        tmp.assign(state[3],state[3]+16);
        for(int i=0;i<15;i++)
            state[3][i]=tmp[after[i]];
    }
    void b(){
        int path[4]={0,5,2,4}; //g w b y 
        int index[4][4]={{1,5,9,13},{8,9,10,11},{2,6,10,14},{4,5,6,7}};
        for(int i=3;i>=1;i--)
            for(int j=0;j<4;j++){
                swap(state[path[i]][index[i][j]],state[path[i-1]][index[i-1][j]]);
            }
    }

    void U(){
        for(int i=0;i<3;i++){
            for(int j=0;j<4;j++)
                swap(state[i][j],state[i+1][j]);
        }
        int after[16]={12, 8, 4, 0, 13, 9, 5, 1, 14, 10, 6, 2, 15, 11, 7, 3};
        tmp.assign(state[4],state[4]+16);
        for(int i=0;i<15;i++)
            state[4][i]=tmp[after[i]];
    }
    void u(){
        for(int i=0;i<3;i++){
            for(int j=0;j<4;j++)
                swap(state[i][4+j],state[i+1][4+j]);
        }    
    }

    void D(){
        for(int i=3;i>=1;i--){
            for(int j=0;j<4;j++)
                swap(state[i][12+j],state[i-1][12+j]);
        }
        int after[16]={12, 8, 4, 0, 13, 9, 5, 1, 14, 10, 6, 2, 15, 11, 7, 3};
        tmp.assign(state[4],state[4]+16);
        for(int i=0;i<15;i++)
            state[4][i]=tmp[after[i]];    
    }

    void d(){
        for(int i=3;i>=1;i--){
            for(int j=0;j<4;j++)
                swap(state[i][8+j],state[i-1][8+j]);
        }
    }
    void operation(char *str){
        switch (str[0]){
            case 'L':
                L();
                if(str[1] =='2') L();
                break;
            case 'R':
                R();
                if(str[1] =='2') R();
                break;
            case 'F':
                F();
                if(str[1] =='2') F();
                break;
            case 'B':
                B();
                if(str[1] =='2') B();
                break;
            case 'U':
                U();
                if(str[1] =='2') U();
                break;
            case 'D':
                D();
                if(str[1] =='2') D();
                break;
            case 'l':
                l();
                if(str[1] =='2') l();
                break;
            case 'r':
                r();
                if(str[1] =='2') r();
                break;
            case 'f':
                f();
                if(str[1] =='2') f();
                break;
            case 'b':
                b();
                if(str[1] =='2') b();
                break;
            case 'u':
                u();
                if(str[1] =='2') u();
                break;
            case 'd':
                d();
                if(str[1] =='2') d();
                break;
            default://parsing error
                assert(0);
            break;
        }
    }
};
void interative_mode(Rubik &obj){
    puts("---interative_mode---");
    char str[10];
    while(~scanf(" %s",str)){
        puts("before:");
        obj.print();
        obj.operation(str);
        puts("after:");
        obj.print();
    }
    puts("---------end---------");
}

void operation_mode(Rubik &obj){
    puts("Input string of operations seperated by space.");
    char str[100];
    scanf("%s",str);
    for(int i=0; str[i];i++){
        obj.operation(&str[i]);
    }
    obj.print();
}
int main(){
	Rubik test;
    puts("Input 'i' to open interactive mode, 'o' to open operation mode");
    char op;
    scanf("%c",&op);
    if(op=='i') interative_mode(test);
    else operation_mode(test);
}
