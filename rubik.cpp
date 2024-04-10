#include<stdio.h>
#include<vector>
#include<algorithm>
#include<string.h>
#include<assert.h>
using namespace std;    
int rotation[16]={12, 8, 4, 0, 13, 9, 5, 1, 14, 10, 6, 2, 15, 11, 7, 3};// clockwise rotation
char hash_map[7] = "OGRBWY";
int path[8][4]={2,5,0,4,3,5,1,4,1,5,3,4,0,5,2,4,0,3,2,1,1,2,3,0};
int indices[12][4][4]={{{0,4,8,12},{3,2,1,0},{15,11,7,3},{12,13,14,15}},{{1,5,9,13},{7,6,5,4},{14,10,6,2},{8,9,10,11}},{{12,8,4,0},{3,7,11,15},{3,7,11,15},{3,7,11,15}},{{13,9,5,1},
{2,6,10,14},{2,6,10,14},{2,6,10,14}},{{0,4,8,12},{0,4,8,12},{15,11,7,3},{0,4,8,12}},{{1,5,9,13},{1,5,9,13},{14,10,6,2},{1,5,9,13}},{{0,4,8,12},{12,13,14,15},{15,11,7,3},{3,2,1,0}},
{{1,5,9,13},{8,9,10,11},{14,10,6,2},{7,6,5,4}},{{0,1,2,3},{0,1,2,3},{0,1,2,3},{0,1,2,3}},{{4,5,6,7},{4,5,6,7},{4,5,6,7},{4,5,6,7}},{{12,13,14,15},{12,13,14,15},{12,13,14,15},{12,13,14,15}},
{{8,9,10,11},{8,9,10,11},{8,9,10,11},{8,9,10,11}}};
struct Rubik{
    /* target state:
        g is the abbreviation for GREEN, which is our front face.

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
    
    int state[6][16]={0};
    int phase = 1;
    Rubik(){//target state
        for(int i=0;i<6;i++)
            for(int j=0;j<16;j++)
                state[i][j]=i;
    }
    inline char hash(int v){
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
        // int path[4]={2,5,0,4}; //r y o w
        // int indices[4][4]={{0,4,8,12},{3,2,1,0},{15,11,7,3},{12,13,14,15}};

        for(int i=3;i>=1;i--)
            for(int j=0;j<4;j++)
                swap(state[path[0][i]][indices[0][i][j]],state[path[0][i-1]][indices[0][i-1][j]]);
        tmp.assign(state[1],state[1]+16);
        for(int i=0;i<16;i++)
            state[1][i]=tmp[rotation[i]];
    }
    void f(){
        // int path[4]={2,5,0,4}; //r y o w
        // int indices[4][4]={{1,5,9,13},{7,6,5,4},{14,10,6,2},{8,9,10,11}};

        for(int i=3;i>=1;i--)
            for(int j=0;j<4;j++)
                swap(state[path[0][i]][indices[1][i][j]],state[path[0][i-1]][indices[1][i-1][j]]);
    }

    void R(){
        // int path[4]={3,5,1,4}; //b y g w
        // int indices[4][4]={{12,8,4,0},{3,7,11,15},{3,7,11,15},{3,7,11,15}};
		for(int i=3;i>=1;i--)
            for(int j=0;j<4;j++)
                swap(state[path[1][i]][indices[2][i][j]],state[path[1][i-1]][indices[2][i-1][j]]);
		tmp.assign(state[2],state[2]+16);
        for(int i=0;i<16;i++)
            state[2][i]=tmp[rotation[i]];
    }
    void r(){
		// int path[4]={3,5,1,4}; //b y g w
        // int indices[4][4]={{13,9,5,1},{2,6,10,14},{2,6,10,14},{2,6,10,14}};        
		for(int i=3;i>=1;i--)
            for(int j=0;j<4;j++)
                swap(state[path[1][i]][indices[3][i][j]],state[path[1][i-1]][indices[3][i-1][j]]);
    }

    void L(){
        // int path[4]={1,5,3,4}; //g y b w
        // int indices[4][4]={{0,4,8,12},{0,4,8,12},{15,11,7,3},{0,4,8,12}};
        for(int i=3;i>=1;i--)
            for(int j=0;j<4;j++)
                swap(state[path[2][i]][indices[4][i][j]],state[path[2][i-1]][indices[4][i-1][j]]);
        tmp.assign(state[0],state[0]+16);
        for(int i=0;i<16;i++)
            state[0][i]=tmp[rotation[i]];
    }
    void l(){
    	// int path[4]={1,5,3,4}; //g y b w
        // int indices[4][4]={{1,5,9,13},{1,5,9,13},{14,10,6,2},{1,5,9,13}};
        for(int i=3;i>=1;i--)
            for(int j=0;j<4;j++)
                swap(state[path[2][i]][indices[5][i][j]],state[path[2][i-1]][indices[5][i-1][j]]);
    }
    
    void B(){
        // int path[4]={0,5,2,4}; //o y r w 
        // int indices[4][4]={{0,4,8,12},{12,13,14,15},{15,11,7,3},{3,2,1,0}};
        for(int i=3;i>=1;i--)
            for(int j=0;j<4;j++)
                swap(state[path[3][i]][indices[6][i][j]],state[path[3][i-1]][indices[6][i-1][j]]);
        tmp.assign(state[3],state[3]+16);
        for(int i=0;i<16;i++)
            state[3][i]=tmp[rotation[i]];
    }
    void b(){
        // int path[4]={0,5,2,4}; //o y r w 
        // int indices[4][4]={{1,5,9,13},{8,9,10,11},{14,10,6,2},{7,6,5,4}};
        for(int i=3;i>=1;i--)
            for(int j=0;j<4;j++)
                swap(state[path[3][i]][indices[7][i][j]],state[path[3][i-1]][indices[7][i-1][j]]);
    }

    void U(){
        // int path[4]={0,3,2,1}; //o b r g 
        // int indices[4][4]={{0,1,2,3},{0,1,2,3},{0,1,2,3},{0,1,2,3}};
        for(int i=3;i>=1;i--)
            for(int j=0;j<4;j++)
                swap(state[path[4][i]][indices[8][i][j]],state[path[4][i-1]][indices[8][i-1][j]]);
        tmp.assign(state[4],state[4]+16);
        for(int i=0;i<16;i++)
            state[4][i]=tmp[rotation[i]];
    }
    void u(){
        // int path[4]={0,3,2,1}; //o b r g 
        // int indices[4][4]={{4,5,6,7},{4,5,6,7},{4,5,6,7},{4,5,6,7}};
        for(int i=3;i>=1;i--)
            for(int j=0;j<4;j++)
                swap(state[path[4][i]][indices[9][i][j]],state[path[4][i-1]][indices[9][i-1][j]]);   
    }

    void D(){
    	// int path[4]={1,2,3,0}; //g r b o 
        // int indices[4][4]={{12,13,14,15},{12,13,14,15},{12,13,14,15},{12,13,14,15}};
        for(int i=3;i>=1;i--)
            for(int j=0;j<4;j++)
                swap(state[path[5][i]][indices[10][i][j]],state[path[5][i-1]][indices[10][i-1][j]]);
        tmp.assign(state[5],state[5]+16);
        for(int i=0;i<16;i++)
            state[5][i]=tmp[rotation[i]];    
    }

    void d(){
    	// int path[4]={1,2,3,0}; //g r b o 
        // int indices[4][4]={{8,9,10,11},{8,9,10,11},{8,9,10,11},{8,9,10,11}};
        for(int i=3;i>=1;i--)
            for(int j=0;j<4;j++)
                swap(state[path[5][i]][indices[11][i][j]],state[path[5][i-1]][indices[11][i-1][j]]);
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
    bool phase_check(){
        if(phase==1){
            bool right=1;
            for(int i=0;i<3;i+=2){// check orange face and red face
                right&=(state[i][5]==i)&(state[i][6]==i)&(state[i][9]==i)&(state[i][10]==i);
            }
            return right;
        }
    }
};
void interative_mode(Rubik &obj){
    puts("---interative_mode---");
    char str[1005];
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
    char str[1005];
    while(~scanf(" %s",str)){
	   	for(int i=0; str[i];i++){
	    	char tmp[2]={str[i],'\0'};
	    	obj.operation(tmp);
	//    	printf("\n%s %d\n",tmp,strlen(tmp));
	
	//        obj.operation(&str[i]);
	//        printf("\n%s %d\n",str[i],strlen(&str[i]));
	    }
	    obj.print();
	}
    
}
int main(){
	Rubik test;
//	interative_mode(test);
    puts("Input 'i' to open interactive mode, 'o' to open operation mode");
    char op;
    scanf("%c",&op);
    if(op=='i') interative_mode(test);
    else operation_mode(test);
}
