#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>

#define TK_int 1
#define TK_float 2
#define TK_do 3
#define TK_while 4
#define TK_if 5
#define TK_else 6
#define TK_For 7
#define TK_Break 8
#define TK_Continue 9
#define TK_id 10
#define TK_divisao 11
#define TK_resto 12
#define TK_Abre_Chaves 13
#define TK_Fecha_Chaves 14
#define TK_Fim_Arquivo 15
#define TK_Atrib 16
#define TK_Const_Int 17
#define TK_Mais 18
#define TK_Menos 19
#define TK_Mult 20
#define TK_Abre_Par 21
#define TK_Fecha_Par 22
#define TK_virgula 23
#define TK_pv 24
#define TK_Maior 25
#define TK_Menor 26
#define TK_Menor_Igual 27
#define TK_Maior_Igual 28
#define TK_Igual 29
#define TK_Diferente 30
#define TK_e 31
#define TK_ou 32
#define TK_nao 33


/***********************************************************************************/
/*                                                                                 */
/*                              *TABELA DE SÍMBOLOS *                               */
/*                                                                                 */
/**********************************************************************************/


struct variavel {
    char id[20];
    char tipo;
};

struct TabSimb{
    struct variavel conteudo;
    struct TabSimb *prox;
};
TabSimb *inicio=NULL;
char nm_id[20];


void insere(variavel var){
    TabSimb *p ;
    p =  (TabSimb *) malloc(sizeof(TabSimb));
    if (p==NULL)
    {
        printf("Erro de alocacao\n");
    }
    p->conteudo = var;
    p->prox = NULL;
    if (inicio==NULL)
        inicio = p;
    else {
        p->prox = inicio;
        inicio  = p;
    }
}

TabSimb * busca(variavel var, TabSimb *tabela){
        if (tabela == NULL)  return NULL;
        if (strcmp(tabela->conteudo.id ,var.id) == 0)  return tabela;
        return busca (var, tabela->prox);
    }

void adicionavar(variavel var){
    TabSimb *variavel;
    variavel = busca(var,inicio);
    if(variavel == NULL){
        insere(var);
    }else{
        variavel->conteudo = var;
    }
}

int buscavar(){
    variavel var;
    strcpy(var.id,nm_id);
    if(busca(var,inicio) == NULL){
        return 0;
    }else{
        return 1;
    }
}





/***********************************************************************************/
/*                                                                                 */
/*  *INÍCIO DO LÉXICO - Não entre a não ser que tenha interesse pessoal em léxicos  */
/*                                                                                 */
/**********************************************************************************/

int linlex=0,collex=1;

char tokens[][20]={"",
                   "TK_int",
                   "TK_float",
                   "TK_do",
                   "TK_while",
                   "TK_if",
                   "TK_else",
                   "TK_for",
                   "TK_id",
                   "TK_divisao",
                   "TK_resto",
                   "TK_Abre_Chaves",
                   "TK_Fecha_Chaves",
                   "TK_Fim_Arquivo",
                   "TK_Atrib",
                   "TK_Const_Int",
                   "TK_Mais",
                   "TK_Menos",
                   "TK_Mult",
                   "TK_Abre_Par",
                   "TK_Fecha_Par",
                   "TK_virgula",
                   "TK_pv",
                   "TK_Maior",
                   "TK_Menor",
                   "TK_Menor_Igual",
                   "TK_Maior_Igual",
                   "TK_Igual",
                   "TK_Diferente",
                   "TK_e",
                   "TK_ou",
                   "TK_nao"
};

char reservadas[][20]={"","int","float","do","while","if","else","for","fim"};

FILE *arqin;
int token;
char lex[20];



// vari �veis do marca - restaura

int tokenant;
long posarq;
char lexant[100];
char cant;
int marcou=0;

void marca_pos()
{
    marcou=1;
    tokenant=token;
    posarq=ftell(arqin);
    strcpy(lexant,lex);
}

void restaura()
{
    fseek(arqin,posarq,SEEK_SET);
    token=tokenant;
    strcpy(lex,lexant);
}

char le_char()
{char c;
    if (fread(&c,1,1,arqin)==0) return -1;
    if (c=='\n') {linlex++;collex=1;}
    else collex++;
    return c;
};

int pal_res(char lex[])
{
    int tk=0;
    while (strcmp(reservadas[tk],"fim")!=0)
    {
        if (strcmp(lex,reservadas[tk])==0) return tk;
        tk++;
    }
    strcpy(nm_id,lex);
    return TK_id;
}


int le_token()
{
    static int pos=0;
    static int estado=0;
    static char c='\0';
    while(1)
    {
        switch (estado)
        {
            case 0:if (c==',') {c=le_char();return TK_virgula;}
                if (c=='+') {c=le_char();return TK_Mais;}
                if (c=='-') {c=le_char();return TK_Menos;}
                if (c=='*') {c=le_char();return TK_Mult;}
                if (c=='{') {c=le_char();return TK_Abre_Chaves;}
                if (c=='}') {c=le_char();return TK_Fecha_Chaves;}
                if (c==';') {c=le_char();strcpy(lex,";");return TK_pv;}
                if (c=='/') {c=le_char();strcpy(lex,"/");return TK_divisao;}
                if (c=='%') {c=le_char();strcpy(lex,"%");return TK_resto;}
                if (c=='(') {c=le_char();return TK_Abre_Par;}
                if (c==')') {c=le_char();return TK_Fecha_Par;}
                if (c=='=')
                {
                    c=le_char();
                    if (c=='=')
                    {
                        c=le_char();
                        return TK_Igual;
                    }
                    strcpy(lex,"=");
                    return TK_Atrib;
                }
                if (c=='<')
                {
                    c=le_char();
                    if (c=='=')
                    {
                        c=le_char();
                        return TK_Menor_Igual;
                    }
                    return TK_Menor;
                }
                if (c=='>')
                {
                    c=le_char();
                    if (c=='=')
                    {
                        c=le_char();
                        return TK_Maior_Igual;
                    }
                    return TK_Maior;
                }
                if (c=='!')
                {
                    c=le_char();
                    if (c=='=')
                    {
                        c=le_char();
                        return TK_Diferente;
                    }
                    return TK_nao;
                }
                if (c=='&')
                {
                    c=le_char();
                    if (c=='&')
                    {
                        c=le_char();
                        return TK_e;
                    }
                }
                if (c=='|')
                {
                    c=le_char();
                    if (c=='|')
                    {
                        c=le_char();
                        return TK_ou;
                    }
                }

                if (c>='a' && c<='z' || c>='A' && c<='Z' || c=='_')
                {
                    lex[0]=c;
                    c=le_char();
                    pos=1;
                    estado=1;
                    break;
                }
                if (c>='0' && c<='9')
                {
                    lex[0]=c;
                    c=le_char();
                    pos=1;
                    estado=2;
                    break;
                }
                if (c==-1) return TK_Fim_Arquivo;
                if (c=='\n'||c=='\r'||c=='\t'||c=='\0'||c==' '){c=le_char();break;}
            case 1:if (c>='a' && c<='z' || c>='A' && c<='Z' || c=='_' || c>='0' && c<='9')
                {
                    lex[pos++]=c;
                    c=le_char();
                    break;
                }
                else
                {
                    estado=0;
                    lex[pos]='\0';
                    return pal_res(lex);
                }
            case 2:if (c>='0' && c<='9')
                {
                    lex[pos++]=c;
                    c=le_char();
                    break;
                }
                else
                {
                    estado=0;
                    lex[pos]='\0';
                    return TK_Const_Int;
                }
        }
    }
}

/********************/
/*                  */
/*  FIM DO L�XICO   */
/*                  */
/********************/

#define MAX_COD 1000

void mostra_t()
{
    printf("%s lex=%s na lin %d, col %d\n",tokens[token],lex,linlex,collex);
}

void geralabel(char label[])
{
    static int numlabel=0;
    sprintf(label,"LB%03d",numlabel++);
}


void geratemp(char temp[])
{
    static int numtemp=0;
    sprintf(temp,"T%03d",numtemp++);
}

/****************/
/*              */
/*  EXPRESS�ES  */
/*              */
/****************/

int T(char T_p[MAX_COD],char T_c[MAX_COD]);
int E(char E_p[MAX_COD],char E_c[MAX_COD]);
int R(char R_hp[MAX_COD],char R_sp[MAX_COD],char R_hc[MAX_COD],char R_sc[MAX_COD]);
int F(char F_p[MAX_COD],char F_c[MAX_COD]);
int S(char S_hp[MAX_COD],char S_sp[MAX_COD],char S_hc[MAX_COD],char S_sc[MAX_COD]);
int Rel(char Rel_p[MAX_COD],char Rel_c[MAX_COD]);

int A(char A_p[MAX_COD],char A_c[MAX_COD])
{
    char A1_p[MAX_COD],A1_c[MAX_COD],Rel_p[MAX_COD],Rel_c[MAX_COD];
    char id[100];
    if (!Rel(Rel_p,Rel_c)) return 0;
    if (token!=TK_Atrib){strcpy(A_p,Rel_p);strcpy(A_c,Rel_c);return 1;}
    token=le_token(); // consome o sinal de atribui��o
    if (!A(A1_p,A1_c)) return 0;
    sprintf(A_c,"%s\t%s=%s\n",A1_c,Rel_p,A1_p);
    strcpy(A_p,Rel_p);
    return 1;
}

int Rel(char Rel_p[MAX_COD],char Rel_c[MAX_COD])
{
    char E1_c[MAX_COD],E2_c[MAX_COD],R_sc[MAX_COD],E1_p[MAX_COD],E2_p[MAX_COD],R_sp[MAX_COD];
    if (E(E1_p,E1_c))
    {
        char op[10];
        if (token==TK_Maior) strcpy(op,">");
        else if (token==TK_Menor) strcpy(op,"<");
        else if (token==TK_Igual) strcpy(op,"=");
        else if (token==TK_Diferente) strcpy(op,"<>");
        else if (token==TK_Maior_Igual) strcpy(op,">=");
        else if (token==TK_Menor_Igual) strcpy(op,"<=");
        if (token==TK_Maior||token==TK_Menor||token==TK_Igual||token==TK_Diferente||token==TK_Maior_Igual||token==TK_Menor_Igual)
        {
            token=le_token();
            if (E(E2_p,E2_c))
            {
                geratemp(Rel_p);
                sprintf(Rel_c,"%s%s\t%s = %s%s%s\n",E1_c,E2_c,Rel_p,E1_p,op,E2_p);
                return 1;
            }
            return 0;
        }
        else
        {
            strcpy(Rel_c,E1_c);
            strcpy(Rel_p,E1_p);
            return 1;
        }
    }
    return 0;
}

int E(char E_p[MAX_COD],char E_c[MAX_COD])
{
    char T_p[MAX_COD],T_c[MAX_COD],R_hp[MAX_COD],R_sp[MAX_COD],R_hc[MAX_COD],R_sc[MAX_COD];
    if (T(T_p,T_c))
    {
        strcpy(R_hc,T_c);
        strcpy(R_hp,T_p);
        if (R(R_hp,R_sp,R_hc,R_sc))
        {
            strcpy(E_c,R_sc);
            strcpy(E_p,R_sp);
            return 1;
        }
    }
    return 0;
}

int R(char R_hp[MAX_COD],char R_sp[MAX_COD],char R_hc[MAX_COD],char R_sc[MAX_COD])
{
    char T_c[MAX_COD],R1_hc[MAX_COD],R1_sc[MAX_COD],T_p[MAX_COD],R1_hp[MAX_COD],R1_sp[MAX_COD];
    if (token==TK_Mais)
    {
        token=le_token();
        if (T(T_p,T_c))
        {
            geratemp(R1_hp);
            sprintf(R1_hc,"%s%s\t%s=%s+%s\n",R_hc,T_c,R1_hp,R_hp,T_p);
            if (R(R1_hp,R1_sp,R1_hc,R1_sc))
            {
                strcpy(R_sp,R1_sp);
                strcpy(R_sc,R1_sc);
                return 1;
            }
        }
        return 0;
    }
    if (token==TK_Menos)
    {
        token=le_token();
        if (T(T_p,T_c))
        {
            geratemp(R1_hp);
            sprintf(R1_hc,"%s%s\t%s=%s-%s\n",R_hc,T_c,R1_hp,R_hp,T_p);
            if (R(R1_hp,R1_sp,R1_hc,R1_sc))
            {
                strcpy(R_sp,R1_sp);
                strcpy(R_sc,R1_sc);
                return 1;
            }
        }
        return 0;
    }
    strcpy(R_sp,R_hp);
    strcpy(R_sc,R_hc);
    return 1;
}

int T(char T_p[MAX_COD],char T_c[MAX_COD])
{
    char F_c[MAX_COD],F_p[MAX_COD],S_hp[MAX_COD],S_sp[MAX_COD],S_hc[MAX_COD],S_sc[MAX_COD];
    if (F(F_p,F_c))
    {
        strcpy(S_hc,F_c);
        strcpy(S_hp,F_p);
        if (S(S_hp,S_sp,S_hc,S_sc))
        {
            strcpy(T_c,S_sc);
            strcpy(T_p,S_sp);
            return 1;
        }
    }
    return 0;
}

int S(char S_hp[MAX_COD],char S_sp[MAX_COD],char S_hc[MAX_COD],char S_sc[MAX_COD])
{
    char F_c[MAX_COD],S1_hc[MAX_COD],S1_sc[MAX_COD],F_p[MAX_COD],S1_hp[MAX_COD],S1_sp[MAX_COD];
    if (token==TK_Mult)
    {
        token=le_token();
        if (F(F_p,F_c))
        {
            geratemp(S1_hp);
            sprintf(S1_hc,"%s%s\t%s=%s*%s\n",S_hc,F_c,S1_hp,S_hp,F_p);
            if (S(S1_hp,S1_sp,S1_hc,S1_sc))
            {
                strcpy(S_sp,S1_sp);
                strcpy(S_sc,S1_sc);
                return 1;
            }
        }
        return 0;
    }
    if (token==TK_divisao)
    {
        token=le_token();
        if (F(F_p,F_c))
        {
            geratemp(S1_hp);
            sprintf(S1_hc,"%s%s\t%s=%s/%s\n",S_hc,F_c,S1_hp,S_hp,F_p);
            if (S(S1_hp,S1_sp,S1_hc,S1_sc))
            {
                strcpy(S_sp,S1_sp);
                strcpy(S_sc,S1_sc);
                return 1;
            }
        }
        return 0;
    }
    if (token==TK_resto)
    {
        token=le_token();
        if (F(F_p,F_c))
        {
            geratemp(S1_hp);
            sprintf(S1_hc,"%s%s\t%s=%s%%%s\n",S_hc,F_c,S1_hp,S_hp,F_p);
            if (S(S1_hp,S1_sp,S1_hc,S1_sc))
            {
                strcpy(S_sp,S1_sp);
                strcpy(S_sc,S1_sc);
                return 1;
            }
        }
        return 0;
    }

    strcpy(S_sp,S_hp);
    strcpy(S_sc,S_hc);
    return 1;
}

int F(char F_p[MAX_COD],char F_c[MAX_COD])
{

    if (token==TK_Const_Int)
    {
        geratemp(F_p);
        sprintf(F_c,"\t%s = %s\n",F_p,lex);
        token=le_token();
        return 1;
    }
    if (token==TK_id)
    {
        strcpy(F_c,"");
        strcpy(F_p,lex);
        token=le_token();
        return 1;
    }
    if (token==TK_Abre_Par)
    {
        char E_c[MAX_COD],E_p[MAX_COD];
        token=le_token();
        if (E(E_p,E_c))
            if (token==TK_Fecha_Par)
            {
                token=le_token();
                strcpy(F_c,E_c);
                strcpy(F_p,E_p);
                return 1;
            }
            else
            {
                printf("Erro!!! Esperava fecha par�nteses\n");
                return 0;
            }
    }

    return 0;
}

/**************/
/*            */
/*  COMANDOS  */
/*            */
/**************/

int Com(char Com_c[MAX_COD]);
int Com_Exp(char Com_c[MAX_COD]);

int Com_Composto(char Comp_c[])
{
    char Com_C[MAX_COD];
    token=le_token(); // consome o abre_chaves
    strcpy(Comp_c,"");
    while (token!=TK_Fecha_Chaves)
    {
        if (!Com(Com_C)) return 0;
        strcat(Comp_c,Com_C);
    }
    token=le_token(); // consome o fecha_chaves
    return 1;
}

int Com_if(char if_c[])
{
    char Rel_c[MAX_COD],Rel_p[MAX_COD],Com1_c[MAX_COD];

    char labelelse[10],labelfim[10];
    geralabel(labelelse);
    geralabel(labelfim);
    token=le_token();
    if (token==TK_Abre_Par){
        token=le_token();
        if (Rel(Rel_p,Rel_c))
            if (token==TK_Fecha_Par) {
                token = le_token();
                char Com1_c[MAX_COD];
                if (Com(Com1_c)) {
                    if (token == TK_else) {
                        token = le_token();
                        char Com2_c[MAX_COD];
                        if (Com(Com2_c)) {
                            sprintf(if_c, "%s\tif %s==0 goto %s\n%s\tgoto %s\n%s:\n%s%s:\n", Rel_c, Rel_p, labelelse,Com1_c, labelfim, labelelse, Com2_c, labelfim);
                            return 1;
                        } else {
                            printf("Erro no comando do else\n");
                            return 0;
                        }
                    } else {
                        printf("Esperava palavra else\n");
                        return 0;
                    }
                } else {
                    printf("Erro na expressao do if \n");
                    return 0;
                }
            }else{
                printf("Esperava fecha parenteses\n");
                return 0;
            }

        }else{
            printf("Erro na expressao do if \n");
            return 0;
        }

            printf("Esperava abre parenteses\n");
            return 0;
}

int Com_while(char while_c[]){
    char Rel_c[MAX_COD],Rel_p[MAX_COD];
    char labelinicio[10],labelfim[10];

    geralabel(labelinicio);
    geralabel(labelfim);

    token=le_token();
    if (token==TK_Abre_Par) {
        token = le_token();
        if (Rel(Rel_p,Rel_c))
            if (token==TK_Fecha_Par){
                token=le_token();
                char Com1_c[MAX_COD];
                    if (Com(Com1_c))
                        sprintf(while_c, "%s:%s\tif %s==1 goto %s\n%s\tgoto %s \n%s:\n",labelinicio,Rel_c, Rel_p, labelfim,Com1_c, labelinicio, labelfim);
                        return 1;
            }else{
                printf("Esperava fecha parenteses\n");
                return 0;
            }
    }else{
        printf("Esperava abre parenteses\n");
        return 0;
    }

}

int Com_do(char do_c[]){
    char Rel_c[MAX_COD],Rel_p[MAX_COD];
    char labelinicio[10];

    geralabel(labelinicio);

    token=le_token();
    char Com1_c[MAX_COD];
    if (Com(Com1_c)){
        if (token==TK_while) {
            token = le_token();
            if (token == TK_Abre_Par) {
                token = le_token();
                if (Rel(Rel_p, Rel_c))
                    if (token == TK_Fecha_Par) {
                        token = le_token();
                        sprintf(do_c, "%s:%s %s \tif %s==1 goto %s\n",labelinicio,Com1_c,Rel_c, Rel_p,labelinicio);
                        return 1;
                    }else{
                        printf("Esperava fecha parenteses\n");
                        return 0;
                    }
            }else{
                printf("Esperava abre parenteses\n");
                return 0;
            }
        }else{
            printf("Erro na expressao while \n");
            return 0;
        }
    }else{
        printf("Erro na expressao do_while \n");
        return 0;
    }
    ;
}

int Com_for(char for_c[]){
    char Rel_c[MAX_COD],Rel_p[MAX_COD],exp1_c[MAX_COD],exp2_c[MAX_COD];

    char labellaco[10],labelfim[10],labelcontinue[10];

    geralabel(labellaco);
    geralabel(labelfim);
    geralabel(labelcontinue);
    token = le_token();
    if (token==TK_Abre_Par) {
        token = le_token();
        if(Com_Exp(exp1_c)){
            if (Rel(Rel_p,Rel_c))   {
                if (token==TK_pv){
                    token = le_token();
                    if(Com_Exp(exp2_c)){
                        if (token==TK_Fecha_Par) {
                            token = le_token();
                            char Com1_c[MAX_COD];
                            if (Com(Com1_c)) {
                                sprintf(for_c,"%s %s:%s\tif %s==1 goto %s\n%s %s goto %s \n%s:",exp1_c,labellaco,Rel_c,Rel_p,labelfim,Com1_c,exp2_c,labellaco,labelfim);
                                return 1;
                            }else{
                                printf("Erro no comando do for\n");
                                return 0;
                            }
                        }else{
                            printf("Esperava fecha parênteses\n");
                            return 0;
                        }
                    }else{
                        printf("Erro no comando do for\n");
                        return 0;
                    }
                }else{
                    printf("Erro no comando do for\n");
                    return 0;
                }
            }else{
                printf("Erro no comando do for\n");
                return 0;
            }
        }else{
            printf("Erro no comando do for\n");
            return 0;
        }
    }else{
        printf("Esperava abre parenteses\n");
        return 0;
    }

}

int Com_Exp(char Com_c[MAX_COD])
{
    if(buscavar() == 1) {
        char id[10];
            char E_c[MAX_COD],E_p[MAX_COD];
            if (A(E_p,E_c))
            {
            if (token==TK_pv)
            {
                token=le_token();
                sprintf(Com_c,"%s",E_c);
                return 1;
            }
            else
            {
                printf("Faltou ponto-e-virgula apos atribuicao\n");
                return 0;
            }
        }
    }else{
        printf("Variavel nao declarada %s\n",nm_id);
        return 0;
    }
}

int Com_tipo(char Com_c[MAX_COD]){
    struct variavel var;
    if(token == TK_int){
    var.tipo = 'I';
    }else{
    var.tipo = 'F';
    }
    token=le_token();
    if (token==TK_id)
    {
        strcpy(var.id,nm_id);
        token=le_token();
    }else{
        return 0;
    }
    adicionavar(var);

    if (token==TK_pv)
    {
        strcpy(Com_c,"");
        token=le_token();
        return 1;
    }
    else
    {
        printf("Faltou ponto-e-virgula apos atribuicao\n");
        return 0;
    }
}

int Com(char Com_c[])
{
    if (token==TK_id)return Com_Exp(Com_c);
    else if (token==TK_Abre_Chaves) return Com_Composto(Com_c);
    else if (token==TK_int || token==TK_float) return Com_tipo(Com_c);
    else if (token==TK_if) return Com_if(Com_c);
    else if (token==TK_while) return Com_while(Com_c);
    else if (token==TK_do) return Com_do(Com_c);
    else if (token==TK_For) return Com_for(Com_c);
    else if (token==TK_pv){
        token=le_token();// comando vazio
        strcpy(Com_c,"");
        return 1;
    }
    else{
        return 0;
    }

}

int main()
{
    FILE *arqout;
    char Com_C[MAX_COD];
    if ((arqin=fopen("c:\\teste\\prog.cpp","rt"))==NULL)
    {
        printf("Erro na abertura do arquivo");
        exit(0);
    }
    if ((arqout=fopen("c:\\teste\\saida.kvmp","wt"))==NULL)
    {
        printf("Erro na abertura do arquivo de saida");
        exit(0);
    }
    token=le_token();
    while (token!=TK_Fim_Arquivo)
    {
        if (Com(Com_C)==0){
            printf("Erro no comando!!!\n");
           break;
        }
        else {
            fprintf(arqout,"%s",Com_C);
            printf("%s",Com_C);
        }

    }
    fclose(arqin);
    fclose(arqout);
}