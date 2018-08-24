#include "Lista_Exames.h"


Lista_Exames::Lista_Exames()
{
    tam = 0;
    num_alunos = 0;
    num_alunos_real = 0;
    prox = NULL;
}

void Lista_Exames::adicionar(Exame* exam){
    Lista_Exames *aux, *ant;
    aux = new Lista_Exames();
    aux->exame = exam;
    aux->prox = NULL;
    if(prox==NULL){
       prox = aux;
    }
    else{
        ant = prox;
        while(ant->prox!= NULL){
            ant = ant->prox;
        }
        ant->prox = aux;
    }
    tam++;
    if(exam->exclusivo){
        num_alunos += 99999;
    }
    else{
        num_alunos += exam->num_alunos;
    }
    num_alunos_real += exam->num_alunos;
}

int Lista_Exames::buscar(int exam){
    Lista_Exames *aux;
    aux = prox;
    while(aux!= NULL){
        if(aux->exame->num == exam){
            return 1;
        }
        aux = aux->prox;
    }
    return 0;
}

void Lista_Exames::imprime(){
    Lista_Exames *aux;
    int tam = 0;
    aux = prox;
    while (aux!= NULL){
        cout << aux->exame->num << "|";
        tam += aux->exame->num_alunos;
        aux = aux->prox;
    }
    cout << "Tam: " << tam;
    cout << endl;
}

int* Lista_Exames::retornaElementos(){
    int *elementos;
    int i = 0;
    Lista_Exames *aux;
    elementos = (int*)malloc(tam*sizeof(int*));
    aux = prox;
    i = 0;
    while(aux!= NULL){
        elementos[i] = aux->exame->num;
        aux = aux->prox;
        i++;
    }
    return elementos;
}

void Lista_Exames::remover(int exame){
    Lista_Exames *aux, *ant;
    aux = prox;
    //imprime();
    //cout << "chegou aqui " << tamanho() << endl;
    if(aux != NULL){
        if(aux->exame->num == exame){
            //cout<< "entrou no if: " << aux->exame->num << endl;
            if(aux->exame->exclusivo){
                num_alunos -= 99999;
            }
            else{
                num_alunos -= aux->exame->num_alunos;
            }
            num_alunos_real -= aux->exame->num_alunos;
            prox =  prox->prox;
            free(aux);
            tam--;
            return;
        }
        ant = aux;
        aux = aux->prox;
        while(aux!= NULL){
            //cout<< "entrou no else: " << aux->exame->num << endl;
            if(aux->exame->num == exame){
                if(aux->exame->exclusivo){
                    num_alunos -= 99999;
                }
                else{
                    num_alunos -= aux->exame->num_alunos;
                }
                num_alunos_real -= aux->exame->num_alunos;
                ant->prox = aux->prox;
                free(aux);
                tam--;
                return;
            }
            ant = aux;
            aux = aux->prox;
        }
    }
}

Lista_Exames::~Lista_Exames()
{
    Lista_Exames *aux, *ant;
    aux = prox;
    if(aux != NULL){
        ant = aux;
        aux = aux->prox;
        while(aux!= NULL){
            free(ant);
            ant = aux;
            aux = aux->prox;
        }
        free(ant);
    }
}
