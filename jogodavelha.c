#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int tab[3][3] = {0};
int rodada=1;
int primeiro;
int ultima_jogada[2];


//            lcd/012/sub/yx
int coordenada[3][3][3][2] = {
    {//linha
        {//linha0
            {0, 0}, 
            {0, 1}, 
            {0, 2}
        },
        {//linha1
            {1, 0}, 
            {1, 1}, 
            {1, 2}
        },
        {//linha2
            {2, 0}, 
            {2, 1}, 
            {2, 2}
        }
    },
    {//coluna
        {//coluna0
            {0, 0}, 
            {1, 0}, 
            {2, 0}
        },
        {//coluna1
            {0, 1}, 
            {1, 1}, 
            {2, 1}
        },
        {//coluna2
            {0, 2}, 
            {1, 2}, 
            {2, 2}
        },
    },
    {//diagonal
        {//d1
            {0, 0},
            {1, 1},
            {2, 2}
        },
        {//d2
            {0, 2},
            {1, 1},
            {2, 0}
        },
        {0}
    }
};

//canto, borda
//    cb/c/yx
int d[2][4][2] = {
    {//canto
        {0, 0},
        {0, 2},
        {2, 0},
        {2, 2}
    },
    {//borda
        {0, 1},
        {1, 0},
        {1, 2},
        {2, 1},
    }
};


//-------------------
void ult_jog(int y, int x){
    ultima_jogada[0] = y;
    ultima_jogada[1] = x;
}

void coord_dir(int *y, int *x, int cb, int p){
    (*y) = d[cb][p][0];
    (*x) = d[cb][p][1];
}

void coord_lcd(int *y, int *x, int lcd, int num, int c){
    (*y) = coordenada[lcd][num][c][0];
    (*x) = coordenada[lcd][num][c][1];
}

void checar_dir(int lcd, int num, int *vazio, int *humano, int *maquina, int p[]){
    for(int i=0; i<3; i++){
        int y, x; coord_lcd(&y, &x, lcd, num, i);
        
        switch(tab[y][x]){
            case 0: (*vazio)++; p[0]=y; p[1]=x; break;
            case 1: (*humano)++; break;
            case 2: (*maquina)++; break;
        }
    }
}

int dois_pra_um(int alvo){
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            if(i == 2 && i == j)break;

            int vazio=0, humano=0, maquina=0, pos[2] = {0};

            checar_dir(i, j, &vazio, &humano, &maquina, pos);

            if(vazio == 1){
                if(alvo == 1 && humano == 2){
                    tab[pos[0]][pos[1]] = 2;
                    return 1;
                }
                if(alvo == 2 && maquina == 2){
                    tab[pos[0]][pos[1]] = 2;
                    return 1;
                }

            }

        }
    }
    return 0;
}

void algoritmo_da_maquina(){
    if(rodada > 1){
        if(dois_pra_um(2)) return;//chance de vitoria
        if(dois_pra_um(1)) return;//defesa
    }

    if(primeiro){
        if(rodada == 1){//marca em qualquer canto
            int a = rand()%4, y, x;

            coord_dir(&y, &x, 0, a);
            tab[y][x] = 2;

            ult_jog(y, x);
            return;
        }
        else if(rodada == 2){
            if(tab[1][1] == 1){//se o humano marcou no centro
                int alternativa = rand()%2;

                if(alternativa){//maquina marca no canto oposto
                    int y, x;

                    ultima_jogada[0] == 0 ? (y=2) : (y=0);
                    ultima_jogada[1] == 0 ? (x=2) : (x=0);
                    
                    ult_jog(y, x);
                    tab[y][x] = 2;
                    return;
                }
                else{//maquina marca numa borda (sem tocar no primeiro)
                    do{
                        int y, x;
                        int a = rand()%4;

                        y = d[1][a][0]; x = d[1][a][1];

                        if(y != ultima_jogada[0] && x != ultima_jogada[1]){
                            tab[y][x] = 2;
                            ult_jog(y, x);
                            return;
                        }

                    }while(1);
                }
            }
            else{//marcou no canto ou na borda
                do{
                    int a = rand()%4;
                    int y, x;
                    coord_dir(&y, &x, 0, a);

                    if(tab[y][x] == 0){

                        tab[y][x] = 2;//temporario
                        for(int i=0; i<3; i++){
                            for(int j=0; j<3; j++){
                                if(i == 2 && i == j) break;
                                
                                int v=0, h=0, m=0, pos[2]={0};
                                checar_dir(i, j, &v, &h, &m, pos);//ve se ta X - X em algum lugar

                                if(m == 2 && v == 1){
                                    return;
                                }
                            }
                        }
                        tab[y][x] = 0;
                    }
                }while(1);
            }
        }
    }
    else{
        if(rodada == 1){
            if(tab[1][1] == 1){//humano pos no centro
                int a = rand()%4;//por no canto
                int y = d[0][a][0];
                int x = d[0][a][1];
                tab[y][x] = 2;
                ult_jog(y, x);
                return;
            }
            else{//humano pos num canto
                tab[1][1] = 2;
                ult_jog(1, 1);
                return;
            }
        }
        else if(rodada == 2){
            for(int i=0; i<2; i++){//se existe uma linha X O X
                int v=0, h=0, m=0, pos[2]={0};
                checar_dir(i, 1, &v, &h, &m, pos);
                
                if(h == 2 && m == 1){//por no canto
                    int a = rand()%4;
                    int y = d[0][a][0];
                    int x = d[0][a][1];
                    tab[y][x] = 2;
                    ult_jog(y, x);
                    return;
                }
            }
        }
    }

    for(int i=0; i<3; i++){//se nao passou em nenhum dos cenarios, coloca na minima possibilidade de vitoria
        for(int j=0; j<3; j++){
            if(i == 2 && i == j) break;
            int v=0, h=0, m=0, pos[2]={0};
            checar_dir(i, j, &v, &h, &m, pos);
            if(v==2 && m == 1){
                tab[pos[0]][pos[1]] = 2;
                ult_jog(pos[0], pos[1]);
                return;
            }
        }
    }

    do{//se nao for NENHUM desses, coloca num aleatorio
        int y = rand()%3;
        int x = rand()%3;
        if(tab[y][x] == 0){
            tab[y][x] = 2;
            break;
        }
    }while(1);
}

//-------
int velha(){
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            if(tab[i][j] == 0) return 0;
        }
    }
    return 1;
}

int acabou(){
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            if(i==2 && i == j)break;
            int v=0, h=0, m=0, pos[2] = {0};

            checar_dir(i, j, &v, &h, &m, pos);
            
            if(m==3)return 2;
            if(h==3)return 1;
        }
    }
    return 0;
}

//
void imprimir(){
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            switch(tab[i][j]){
                case 0: printf(" "); break;
                case 1: printf("X"); break;
                case 2: printf("O"); break;
            }
            printf(" | ");
        }
        printf("\n");
    }
}

int main(){
    srand(time(NULL));
    primeiro = rand()%2;
    int a;

    do{
        if(primeiro == 1){ 
            algoritmo_da_maquina();
            if(velha()){
                a=0;
                break;
            }
            if(a = acabou()){
                break;
            }
        }

        printf("\n");
        imprimir();

        printf("Coloque a coordenada (y, x): ");
        int x, y, a; scanf("%d %d", &y, &x);

        tab[y][x] = 1;
        if(velha()){
            a=0;
            break;
        }
        if(a = acabou()){
            break;
        }

        if(primeiro == 0){ 
            algoritmo_da_maquina();
            if(velha()){
                a=0;
                break;
            }
            if(a = acabou()){
                break;
            }
        }

        rodada++;
    }while(1);

    if(a == 1)printf("Humano ganhou\n");
    else if(a == 2) printf("Maquina ganhou\n");
    else printf("Deu velha\n");
    imprimir();

    return 0;
}

