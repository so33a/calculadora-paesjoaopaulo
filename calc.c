#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node * link;

struct node {
    int valor;
    link next;
};

struct pilha
{
    link t;
};

/* Define um novo tipo de dado chamado Pilha que é um ponteiro para "struct pilha". */
typedef struct pilha * Pilha;

link createNode(int valor, link next){
    link novo = malloc(sizeof * novo);
    novo->valor = valor;
    novo->next = next;
    return novo;
}

int pilhaVazia(Pilha p){
    return (p->t == NULL);
}

/* Aloca espaço para armazenar uma nova Pilha */
Pilha novaPilha () {
    Pilha p = malloc(sizeof(*p));
    if (p == NULL)
        {
            printf("Algum erro aconteceu !\n");
            exit(-1);
        }
    p->t = 0; /* devemos inicializar o topo com 0 */
    return p;
}

/* Operação de inserir novo elemento na pilha */
void push (Pilha p, int valor) {
    p->t = createNode(valor, p->t);
}

/* Operação de remover um elemento da pilha */
link pop (Pilha p) {
    link topo = NULL;
    topo = p->t;
    p->t = topo->next;
    return topo;
}

/* Libera memória de uma dada pilha p */
void destroiPilha (Pilha p)
{
    while(!pilhaVazia(p))
        pop(p);
    free(p);
}

/* Operação para pegar o elemento do topo da pilha */
int topo (Pilha p) {
    return p->t->valor;
}

/* Transforma a notação infixa para a notação posfixa */
int infixoParaPosfixo (char * entrada, char * saida, int n)
{
    Pilha p = novaPilha();
    int i;
    int j = 0;
    link sharp;
    int c;
    push(p, '(');
    for (i = 0; entrada[i] != '\0' &&  i < n; i++)
        {
            if((j-1 > 0) && (saida[j-1] != ' '))
                saida[j++]  = ' ';
            if(entrada[i] == '(') {
                push(p, entrada[i]);
            } else if(entrada[i] == ')'){
                while (1) {
                    sharp = pop(p);
                    c = sharp->valor;
                    if(c == '(') break;
                    saida[j++] = c;
                    saida[j++] = ' ';
                }
            } else if((entrada[i] == '+') || (entrada[i] == '-')){
                while (1) {
                    c = topo(p);
                    if(c == '(') break;
                    pop(p);
                    saida[j++] = c;
                    saida[j++] = ' ';
                }
                push(p, entrada[i]);
            } else if((entrada[i] == '*') || (entrada[i] == '/')) {
                while (1) {
                    c = topo(p);
                    if(c == '(' || c == '+' || c == '-'  ) break;
                    pop(p);
                    saida[j++] = c;
                    saida[j++] = ' ';
                }
                push(p, entrada[i]);
            } else if (entrada[i] >= '0' && entrada[i] <= '9') {
                while (entrada[i] >= '0' && entrada[i] <= '9') {
                    saida[j++] = entrada[i];
                    i++;
                }
                saida[j++] = ' ';
                i--;
            }

        }
    while (1) {
        sharp = pop(p);
        c = sharp->valor;
        if(c == '(') break;
        saida[j++] = c;
        saida[j++] = ' ';
    }
    saida[j] = '\0';
    destroiPilha(p);
    return 0;
}

int bemEncaixado (char* s) {
    Pilha p = novaPilha();
    int i;
    int resultado = 1;
    for(i = 0; s[i] != '\0'; i++) {
        if(s[i] == '(') {
            push(p, 1);
        } else if (s[i] == ')') {
            if(p->t <= 0) {
                resultado = 0;
                break;
            }
            pop(p);
        }
    }
    if(p->t > 0)
        resultado = 0;
    destroiPilha(p);
    return resultado;
}

int calcula ( char * s ) {
    int i = 0;
    int numero = 0;
    Pilha p = novaPilha();
    int resultado ;
    link a,b;
    while  (s[i] != '\0') {
        if(s[i] >= '0' && s[i] <= '9' ) {
            sscanf(&s[i], "%d", & numero);
            while(s[i] >= '0' && s[i] <= '9')
                i++;
            i--;
            push(p, numero);
        } else if (s[i] == '+') {
            b = pop(p);
            a = pop(p);
            push (p, a->valor + b->valor);
        } else if (s[i] == '-') {
            b = pop(p);
            a = pop(p);
            push (p, a->valor - b->valor);
        } else if (s[i] == '*') {
            b = pop(p);
            a = pop(p);
            push (p, a->valor * b->valor);
        } else if (s[i] == '/') {
            b = pop(p);
            a = pop(p);
            push (p, a->valor/b->valor);
        }
        i++;
    }

    resultado = topo(p);
    destroiPilha(p);
    return resultado;
}

/* Exemplo de utilização */
int main () {
    char infixo[255] ;
    char posfixo[255];
    
    printf("Ex. 2 - %d\n", calcula("2 3 + 4 * 1 +")); //(2 + 3) * 4 + 1
    printf("Ex. 3 - %d\n", calcula("5 2 3 4 + + *")); // ((4 + 3) +  2) * 5 

    printf("Sou uma calculadora de inteiros implementado com pilha!\n");
    printf("Digite quit para sair !\n");
    printf ("> ");
    while(fgets(infixo, 255, stdin) != NULL) {
        if(strcmp(infixo, "quit\n") == 0)  {
            printf ("morri !\n");
            return 0;
        }
        if(bemEncaixado(infixo)) {
            infixoParaPosfixo(infixo, posfixo, 255);
            printf("%d\n", calcula(posfixo));
        } else
            printf ("Erro nos parenteses\n");
        printf ("> ");
    }
    return 0;
}
