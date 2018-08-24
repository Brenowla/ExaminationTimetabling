#include "Lista_alunos.h"
#include <stdio.h>

Lista_alunos::Lista_alunos()
{
    aluno = -1;
    exames = NULL;
    prox = NULL;
}

Lista_alunos::Lista_alunos(int num_aluno, Exame* num_exame)
{
    aluno = num_aluno;
    exames = new Lista_Exames();
    exames->exame = num_exame;
    exames->prox = NULL;
}


Lista_Exames* Lista_alunos::adicionar_aluno(int num_aluno,Exame* num_exame){
    Lista_alunos *novo,*atual, *ant;
    ant = NULL;
    atual = prox;
    while(atual != NULL && atual->aluno < num_aluno){
        ant = atual;
        atual = atual->prox;
    }
    if(atual == NULL){
        novo = new Lista_alunos(num_aluno,num_exame);
        novo->prox = NULL;
        if(ant != NULL){
            ant->prox = novo;
        }
        else{
            prox = novo;
        }
    }
    else if(atual->aluno == num_aluno){
        atual->adicionar_exame(num_exame);
        return atual->exames;
    }
    else{
        novo = new Lista_alunos(num_aluno,num_exame);
        if(ant != NULL){
            ant ->prox = novo;
            novo -> prox = atual;
        }
        else{
            prox = novo;
            novo -> prox = atual;
        }
    }
    return NULL;
}

void Lista_alunos::adicionar_exame(Exame* num_exame){
    Lista_Exames *novo,*atual;
    novo = new Lista_Exames();
    novo->exame = num_exame;
    novo->prox = NULL;
    atual = exames;
    while(atual->prox != NULL){
        atual = atual->prox;
    }
    atual->prox = novo;
}

void Lista_alunos::imprime_lista(){
    Lista_alunos *atual;
    Lista_Exames *atu;
    atual = prox;
    while(atual != NULL){
        atu = atual->exames;
        while(atu != NULL){
            atu = atu->prox;
        }
        atual = atual->prox;
    }
}

Lista_alunos::~Lista_alunos()
{
    Lista_alunos *aux, *ant;
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
