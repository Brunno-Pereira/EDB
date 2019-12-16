#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct arv2 {
    int num, alt_dir, alt_esq;
    char cpf[15];
    char nome[20];
    char celular[15];
    struct arv2 *dir;
    struct arv2 *esq;
}AVL;

typedef struct arv {
    int num, quantidade;
    char descricao[40];
    float preco;
    struct arv *esq;
    struct arv *dir;
}Arvore;

AVL *rotacao_esquerda(AVL *aux) {
    AVL *aux1, *aux2;
    aux1 = aux->dir;
    aux2 = aux1->esq;
    aux->dir = aux2;
    aux1->esq = aux;

    if(aux->dir==NULL) {
        aux->alt_dir= 0;
    }else if(aux->dir->alt_esq > aux->dir->alt_dir) {
        aux->alt_dir= aux->dir->alt_esq + 1;
    }else{
        aux->alt_dir= aux->dir->alt_dir + 1;
    }

    if(aux1->esq->alt_esq > aux1->esq->alt_dir) {
        aux1->alt_esq = aux1->esq->alt_esq + 1;
    }else{
        aux1->alt_esq= aux1->esq->alt_dir + 1;
    }
    return aux1;
}

AVL *rotacao_direita(AVL *aux) {
     AVL *aux1, *aux2;
     aux1= aux->esq;
     aux2= aux1->dir;
     aux->esq = aux2;
     aux1->dir = aux;

     if(aux->esq == NULL){
         aux->alt_esq = 0;
     }else if(aux->esq->alt_esq > aux->esq->alt_dir){
         aux->alt_esq = aux->esq->alt_esq + 1;
     }else{
         aux->alt_esq = aux->esq->alt_dir + 1;
     }

     if(aux1->dir->alt_esq > aux1->dir->alt_dir){
         aux1->alt_dir = aux->dir->alt_esq + 1;
     }else{
         aux1->alt_dir  =aux->dir->alt_dir + 1;
     }
     return aux1;
}

AVL *balanceamento(AVL *aux){
    int d, df;
    d = aux->alt_dir - aux->alt_esq;
    if(d==2){
        df = aux->dir->alt_dir - aux->dir->alt_esq;
        if(df >= 0){
            aux = rotacao_esquerda(aux);
        }else{
            aux->dir =  rotacao_direita(aux->dir);
            aux = rotacao_esquerda(aux);
        }
    }else if(df==-2){
        df = aux->esq->alt_dir - aux->esq->alt_esq;
        if(df<=0){
            aux = rotacao_direita(aux);
        }else{
            aux->esq= rotacao_esquerda(aux->esq);
            aux  = rotacao_direita(aux);
        }
    }
    return aux;
}

AVL *inserir1(AVL *aux, int num, char nome[], char cpf[], char celular[]){
    AVL *novo;
    if(aux == NULL){
        novo = (AVL*)malloc(sizeof(AVL));
        novo->num = num;
        strcpy(novo->nome,nome);
        strcpy(novo->cpf,cpf);
        strcpy(novo->celular,celular);
        novo->alt_dir=0;
        novo->alt_esq=0;
        novo->dir=NULL;
        novo->esq=NULL;
        aux= novo;
    }else if(num < aux->num){
        aux->esq = inserir1(aux->esq, num, nome, cpf, celular);
        if(aux->esq->alt_dir > aux->esq->alt_esq){
            aux->alt_esq = aux->esq->alt_dir + 1;
        }else{
            aux->alt_esq = aux->esq->alt_esq + 1;
            aux = balanceamento(aux);
        }
    }else{
        aux->dir = inserir1(aux->dir, num, nome, cpf, celular);
        if(aux->dir->alt_dir > aux->dir->alt_esq){
            aux->alt_dir = aux->dir->alt_dir + 1;
        }else{
            aux->alt_dir = aux->dir->alt_esq + 1;
            aux = balanceamento(aux);
        }
    }
    return aux;
}

int consultarAVL(AVL *aux, int num, int achou){
    if(aux != NULL && achou == 0){
        achou = 1;
    }else if(num < aux->num){
        achou = consultarAVL(aux->esq, num, achou);
    }else{
        achou = consultarAVL(aux->dir, num, achou);
    }
    return achou;
}

void mostrarOrdemAVL(AVL *aux){
    if(aux != NULL){
        mostrarOrdemAVL(aux->esq);
        printf("id: %d nome: %s cpf: %s celular: %s \n", aux->num ,aux->nome, aux->cpf, aux->celular);
        mostrarOrdemAVL(aux->dir);
    }
}

void mostrarPreOrdemAVL(AVL *aux){
   if(aux != NULL){
        printf("id: %d nome: %s cpf: %s celular: %s \n", aux->num ,aux->nome, aux->cpf, aux->celular);
        mostrarPreOrdemAVL(aux->esq);
        mostrarPreOrdemAVL(aux->dir);
   }
}

void mostrarPosOrdemAVL(AVL *aux){
    if(aux != NULL){
        mostrarPosOrdemAVL(aux->esq);
        mostrarPosOrdemAVL(aux->dir);
        printf("id: %d nome: %s cpf: %s celular: %s \n", aux->num ,aux->nome, aux->cpf, aux->celular);
    }
}

void consultarArvorePreOrdem(Arvore *aux){
   if(aux != NULL){
        printf("codigo: %d preco: %f quatidade: %d descricao: %s\n", aux->num, aux->preco, aux->quantidade, aux->descricao);
        consultarArvorePreOrdem(aux->esq);
        consultarArvorePreOrdem(aux->dir);
   }
}

AVL *remover(AVL *aux, int numero){
    AVL *p, *p2;

    if(aux->num == numero){
        if(aux->esq==aux->dir){
            printf("nao possui filhos (folha)\n");
            free(aux);
            return aux;
        }else if(aux->dir == NULL){
            p= aux->esq;
            free(aux);
            return p;
        }else if(aux->esq == NULL){
            p= aux->dir;
            free(aux);
            return p;
        }else{
            p = aux->dir;
            p2 = aux->dir;
            while(p->esq != NULL)
            {
                p = p->esq;
            }
            p->esq= aux-> esq;
            free(aux);
            return p2;
        }
    }else if(aux->num<numero){
        aux->dir= remover(aux->dir, numero);
    }else{
        aux->esq= remover(aux->esq, numero);
    }
    return aux;
}

AVL *atualiza(AVL *aux){
    if(aux != NULL){
        aux->esq= atualiza(aux->esq);
        if(aux->esq == NULL){
            aux->alt_esq = 0;
        }else if(aux->esq->alt_esq > aux->esq->alt_dir){
            aux->alt_esq = aux->esq->alt_esq + 1;
        }else{
            aux->alt_esq = aux->esq->alt_dir + 1;
        }

        aux->dir = atualiza(aux->dir);
        if(aux->dir == NULL){
            aux->alt_dir = 0;
        }else if(aux->dir->alt_esq > aux->dir->alt_dir){
            aux->alt_dir = aux->dir->alt_esq + 1;
        }else{
            aux->alt_dir = aux->dir->alt_dir + 1;
        }
        aux = balanceamento(aux);
    }
    return aux;
}

AVL *desalocar(AVL *aux){
    if(aux != NULL){
        aux->esq= desalocar(aux->esq);
        aux->dir= desalocar(aux->dir);
        free(aux);
    }
    return NULL;
}

///

Arvore *inserir(Arvore *aux, int nome, float preco, int quant, char descri[]){

    if(aux == NULL){
        aux = (Arvore*)malloc(sizeof(Arvore));
        aux->num=nome;
        aux->preco = preco;
        aux->quantidade = quant;
        strcpy(aux->descricao,descri);
        aux->esq=NULL;
        aux->dir=NULL;
    }else if(nome < aux->num) {
        aux->esq=inserir(aux->esq, nome, preco, quant, descri);
    }else{
        aux->dir= inserir(aux->dir, nome, preco, quant, descri);
    }
    return aux;
}

int consultarNo(Arvore *aux, int nome, int achou){
    if(aux != NULL && achou==0){
        if(aux->num == nome) {
            achou=1;
        }else if(nome<aux->num){
            achou= consultarNo(aux->esq, nome, achou);
        }else {
            achou= consultarNo(aux->dir, nome, achou);
        }
    }
    return achou;
}

void consultarArvoreOrdem(Arvore *aux){
    if(aux != NULL){
        consultarArvoreOrdem(aux->esq);
        printf("codigo: %d preco: %f quatidade: %d descricao: %s\n", aux->num, aux->preco, aux->quantidade, aux->descricao);
        consultarArvoreOrdem(aux->dir);
    }


}

Arvore *remover1(Arvore *aux, int numero){
    Arvore *p, *p2;

    if(aux->num == numero){
        if(aux->esq==aux->dir){
            printf("Acabou\n");
            free(aux);
            return aux;
        }else if(aux->dir == NULL){
            p= aux->esq;
            free(aux);
            return p;
        }else if(aux->esq == NULL){
            p= aux->dir;
            free(aux);
            return p;
        }else{
            p = aux->dir;
            p2 = aux->dir;
            while(p->esq != NULL){
                p = p->esq;
            }
            p->esq= aux-> esq;
            free(aux);
            return p2;
        }
    }else if(aux->num<numero){
        aux->dir= remover1(aux->dir, numero);
    }else{
        aux->esq= remover1(aux->esq, numero);
    }
    return aux;
}

Arvore *desalocar1(Arvore *aux){
    if(aux != NULL){
        aux->esq= desalocar1(aux->esq);
        aux->dir= desalocar1(aux->dir);
        free(aux);
    }
    return NULL;
}

void entradaProduto(Arvore *aux){
    int numero, quanti;
    printf("Codigo do produto: ");
    scanf("%i", &numero);
    printf("Quantidade: ");
    scanf("%i", &quanti);
    if(aux->num==numero){
        aux->quantidade = aux->quantidade + quanti;
    }else if (aux->num<numero){
        entradaProduto(aux->esq);
    }else{
        entradaProduto(aux->dir);
    }
}

void saidaProduto(Arvore *aux){
    int numero, quanti;
    printf("Codigo: ");
    scanf("%i", &numero);
    printf("Quantidade: ");
    scanf("%i", &quanti);
    if(aux->num==numero){
        aux->quantidade = aux->quantidade - quanti;

        if(aux->quantidade <= 0){
            aux->quantidade = 0;
        }
    }else if (aux->num<numero){
        saidaProduto(aux->esq);
    }else{
        saidaProduto(aux->dir);
    }
}

void alterarProduto(Arvore *aux){
    int numero, numero2, quant;
    float preco;
    char descri[20];
    
    printf("Codigo: ");
    scanf("%i", &numero);
    
    if(aux->num==numero){
        printf("Novo codigo: ");
        scanf("%i", &numero2);
        aux->num=numero2;
        printf("Novo preco: ");
        scanf("%f", &preco);
        aux->preco=preco;
        printf("Nova quantidade: ");
        scanf("%i", &quant);
        aux->quantidade = quant;
        printf("Nova descricao: ");
        
        fflush(stdin);
        gets(descri);
        strcpy(aux->descricao,descri);
    }else if (aux->num<numero){
        alterarProduto(aux->esq);
    }else{
        alterarProduto(aux->dir);
    }
}

int main(){
    AVL *raiz=NULL;
    Arvore *raiz1 =NULL;
    AVL *aux;
    Arvore *aux1;
    int op, numero, achou, achou1, op2, quant;
    float preco;
    char nome[30], cpf[15], celular[15], descri[40];

    do{
        system("cls");
        printf("1-Cadastrar cliente\n2-Mostrar arvore(ordem)\n3-mostrar arvore(pos ordem)\n4-Remover cliente\n5-Esvaziar\n");
        printf("6-Inserir produto\n7-Buscar produto\n8-Exibir produto (pre ordem)\n9-Excluir produto\n10-Operacao de Entrada\n11-Operacao De Saida\n12-Alterar produto\n");
        scanf("%d", &op);
        system("cls");

        switch(op){
        case 1:
            printf("Id do cliente: ");
            scanf("%d", &numero);
            printf("Nome do cliente \n");
            fflush(stdin);
            gets(nome);
            printf("CPF: ");
            fflush(stdin);
            gets(cpf);
            printf("Numero do celular: ");
            fflush(stdin);
            gets(celular);
            
            raiz = inserir1(raiz, numero, nome, cpf, celular);
            if(raiz != NULL){
                printf("Feito!\n");
            }

            break;
            
        case 2:
            if(raiz==NULL){
                printf("Arvore vazia.\n");
            }else{
                printf("Clientes:\n");
                mostrarOrdemAVL(raiz);
            }
            break;
            
        case 3:
            if(raiz==NULL){
                printf("Arvore vazia.\n");
            }else{
                printf("Clientes:\n");
                mostrarPosOrdemAVL(raiz);
            }
            break;
            
        case 4:
            if(raiz==NULL){
                printf("Arvore vazia!");
            }else{
                printf("Id do cliente: ");
                scanf("%d", &numero);
                achou=0;
                achou=consultarAVL(raiz, numero, achou);
                if(achou==0){
                    printf("ID nao encontrado!\n");
                }else{
                    raiz = remover(raiz, numero);
                    printf("Numero removido\n");
                }
            }
            break;
            
        case 5:
            if(raiz==NULL){
                printf("Arvore vazia!\n");
            }else{
                raiz= desalocar(raiz);
				printf("Esvaziado!\n");
            }
            break;
            
        case 6:
            printf("Codigo: ");
            scanf("%d", &numero);
            printf("Preco: ");
            scanf("%f", &preco);
            printf("Quantidade: ");
            scanf("%i", &quant);
            printf("Descricao: ");
            fflush(stdin);
            gets(descri);
            
            raiz1 = inserir(raiz1, numero, preco, quant, descri);
            if(raiz1 != NULL){
                printf("Numero inserido!\n");
            }

            break;
            
        case 7:
            if(raiz1==NULL){
                printf("Arvore vazia\n");
            }else{
                printf("Codigo: ");
                scanf("%d", &numero);
                achou1=0;
                achou1 = consultarNo(raiz1, numero, achou1);
            	if(achou1==0){
               		printf("Produto inexistente\n");	
	            }else{
        	        printf("Codigo: %d\nPreco: %f\nQuantidade: %d\nDescricao: %s\n", aux1->num, aux1->preco, aux1->quantidade, aux1->descricao);
            	}
            }
            break;
                        
        case 8:
            if(raiz1==NULL){
                printf("Arvore vazia \n");
            }else{
                printf("Produtos mostrados em pre ordem\n");
                consultarArvorePreOrdem(raiz1);

            }
            break;
                        
        case 9:
            if(raiz1==NULL){
                printf("Arvore vazia\n");
            }else{
                printf("Codigo: ");
                scanf("%d", &numero);
                achou1=0;
                achou1= consultarNo(raiz1, numero, achou1);
                if(achou1==0){
                    printf("Produto inexistente\n");
                }else{
                    raiz1 = remover1(raiz1, numero);
                    printf("Produto removido\n");
                }
            }
            break;
        case 10:
            if(raiz1==NULL){
                printf("Arvore vazia \n");
            }else{
                entradaProduto(raiz1);
            }
            break;
            
        case 11:
            if(raiz1==NULL){
                printf("Arvore vazia \n");
            }else{
                saidaProduto(raiz1);
            }
            break;
            
        case 12:
            if(raiz1==NULL){
                printf("Arvore vazia \n");
            }else{
                alterarProduto(raiz1);
            }
            break;
        default:
            printf("Opcao invalida\n");
            break;
        }
        printf("Digite '0' para sair, outra numero para retornar\n");
        scanf("%d", &op2);

    }while(op2== 0);

    return 0;
}
