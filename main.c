#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>

    #define textcolor(cor) ({\
        int iColor;\
        \
        switch(cor) {\
            case 0:\
            iColor=2;\
            break;\
        case 1:\
            iColor=9;\
            break;\
        case 2:\
            iColor=6;\
            break;\
        case 3:\
            iColor=11;\
            break;\
        case 4:\
            iColor=3;\
            break;\
        case 5:\
            iColor=8;\
            break;\
        case 6:\
            iColor=10;\
            break;\
        case 7:\
            iColor=12;\
            break;\
        case 8:\
            iColor=4;\
            break;\
        case 15:\
            iColor=0;\
            break;\
        }\
        \
        HANDLE hl = GetStdHandle(STD_OUTPUT_HANDLE);\
        CONSOLE_SCREEN_BUFFER_INFO bufferInfo;\
        BOOL b = GetConsoleScreenBufferInfo(hl, &bufferInfo);\
        bufferInfo.wAttributes &= 0x00F0;\
        SetConsoleTextAttribute (hl, bufferInfo.wAttributes |= iColor);\
    })

    #define clear() ({\
        system("cls");\
    })

#elif __linux__ || __APPLE__
    #define textcolor(cor) ({\
        switch(cor) {\
            case 0:\
                printf("\033[01;33m");\
                break;\
            case 1:\
                printf("\033[01;35m");\
                break;\
            case 2:\
                printf("\033[01;32m");\
                break;\
            case 3:\
                printf("\033[01;31m");\
                break;\
            case 4:\
                printf("\033[01;34m");\
                break;\
            case 5:\
                printf("\033[22;31m");\
                break;\
            case 6:\
                printf("\033[01;36m");\
                break;\
            case 7:\
                printf("\033[01;30m");\
                break;\
            case 8:\
                printf("\033[22;36m");\
                break;\
            case 15:\
                printf("\033[01;37m");\
                break;\
        }\
    })

    #define clear() ({\
        printf("\033[H\033[J");\
    })
#endif

void aloca_tabuleiro(int **tabuleiro, int altura, int largura);
void gera_tabuleiro(int **tabuleiro, int **tabuleiro_aux, int altura, int largura, int bombas);
void abre_casa(int **tabuleiro, int **tabuleiro_aux, int altura, int largura, int y, int x);
void imprime_tabuleiro(int *tabuleiro, int *tabuleiro_aux, int altura, int largura);

int main(){

    //system("color F0");
    srand(time(NULL));

    int altura, largura, bombas;
    int *tabuleiro=NULL, *tabuleiro_aux=NULL;

    altura=10;
    largura=10;
    bombas=altura*largura/5;

    aloca_tabuleiro(&tabuleiro, altura, largura);
    aloca_tabuleiro(&tabuleiro_aux, altura, largura);
    gera_tabuleiro(&tabuleiro, &tabuleiro_aux, altura, largura, bombas);

    int x,y;
    imprime_tabuleiro(tabuleiro, tabuleiro_aux, altura, largura);
    while(1){

    scanf("%d %d", &y, &x);
    abre_casa(&tabuleiro, &tabuleiro_aux, altura, largura, y, x);

    clear();
    imprime_tabuleiro(tabuleiro, tabuleiro_aux, altura, largura);

    }
}

void aloca_tabuleiro(int **tabuleiro, int altura, int largura){
    if((*tabuleiro=(int*)realloc(*tabuleiro, sizeof(int)*(altura*largura)))==NULL){
        printf("Erro de alocacao!\n");
        exit(1);
    }
}

void gera_tabuleiro(int **tabuleiro, int **tabuleiro_aux, int altura, int largura, int bombas){
    int i, j, y, x, cont;

    for(i=0;i<altura;i++){
        for(j=0;j<largura;j++){
            *(*tabuleiro+j+i*largura)=0;
            *(*tabuleiro_aux+j+i*largura)=0;
        }
    }

    while(bombas>0){
        do{
            y=rand()%altura;
            x=rand()%largura;
        }while(*(*tabuleiro+x+y*largura)==-1);
        *(*tabuleiro+x+y*largura)=-1;
        bombas--;
    }

    for(i=0;i<altura;i++){
        for(j=0;j<largura;j++){
            cont=0;
            if(*(*tabuleiro+j+i*largura)==0){

                if((i-1)>=0){
                    if((j-1)>=0&&*(*tabuleiro+(j-1)+(i-1)*largura)==-1)
                        cont++;
                    if(*(*tabuleiro+j+(i-1)*largura)==-1)
                        cont++;
                    if((j+1)<largura&&*(*tabuleiro+(j+1)+(i-1)*largura)==-1)
                        cont++;
                }

                if((j-1)>=0&&*(*tabuleiro+(j-1)+i*largura)==-1)
                    cont++;
                if((j+1)<largura&&*(*tabuleiro+(j+1)+i*largura)==-1)
                    cont++;

                if((i+1)<altura){
                    if((j-1)>=0&&*(*tabuleiro+(j-1)+(i+1)*largura)==-1)
                        cont++;
                    if(*(*tabuleiro+j+(i+1)*largura)==-1)
                        cont++;
                    if((j+1)<largura&&*(*tabuleiro+(j+1)+(i+1)*largura)==-1)
                        cont++;
                }
                *(*tabuleiro+j+i*largura)=cont;
            }
        }
    }
}

void imprime_tabuleiro(int *tabuleiro, int *tabuleiro_aux, int altura, int largura){
    int i, j;
    textcolor(15);
    printf("   ");
    for(j=0;j<largura;j++)
        printf("%d ", j);
    printf("\n  ");
    for(j=0;j<largura;j++){
        printf("%c%c", '+', '-');
    }
        printf("%c", '+');
    printf("\n");

    for(i=0;i<altura;i++){
        printf("%d %c", i, '|');
        for(j=0;j<largura;j++){
            if(*(tabuleiro_aux+j+i*largura)==1){
                if(*(tabuleiro+j+i*largura)==-1)
                    printf("X%c",'|');
                else{
                    textcolor(*(tabuleiro+j+i*largura));
                    printf("%d", *(tabuleiro+j+i*largura));
                    textcolor(15);
                    printf("%c", '|');
                }
            }
            else{
                printf("%c%c", ' ', '|');
            }
        }
        printf("\n  ");
        for(j=0;j<largura;j++){
            printf("%c%c", '+', '-');
        }
        printf("%c", '+');
        printf("\n");
    }
}

void abre_casa(int **tabuleiro, int **tabuleiro_aux, int altura, int largura, int y, int x){
    if(*(*tabuleiro_aux+x+y*largura)==0){
        *(*tabuleiro_aux+x+y*largura)=1;
        if(*(*tabuleiro+x+y*largura)==0){
            if((x-1)>=0){
                if((y-1)>=0)
                    abre_casa(tabuleiro, tabuleiro_aux, altura, largura, (y-1), (x-1));
                abre_casa(tabuleiro, tabuleiro_aux, altura, largura, (y), (x-1));
                if((y+1)<altura)
                    abre_casa(tabuleiro, tabuleiro_aux, altura, largura, (y+1), (x-1));
            }

            if((y-1)>=0)
                abre_casa(tabuleiro, tabuleiro_aux, altura, largura, (y-1), (x));
            if((y+1)<altura)
                abre_casa(tabuleiro, tabuleiro_aux, altura, largura, (y+1), (x));

            if((x+1)<largura){
                if((y-1)>=0)
                    abre_casa(tabuleiro, tabuleiro_aux, altura, largura, (y-1), (x+1));
                abre_casa(tabuleiro, tabuleiro_aux, altura, largura, (y), (x+1));
                if((y+1)<altura)
                    abre_casa(tabuleiro, tabuleiro_aux, altura, largura, (y+1), (x+1));
            }

        }
        else if(*(*tabuleiro+x+y*largura)==-1){
            imprime_tabuleiro(*tabuleiro, *tabuleiro_aux,altura, largura);
            printf("BUUUUUUUMMMMMMMM!\n");
            exit(1);
        }
    }


}
