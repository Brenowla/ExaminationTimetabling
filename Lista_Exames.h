#ifndef LISTA_EXAMES_H
#define LISTA_EXAMES_H
#include "Exame.h"

#include <stdlib.h>
#include <iostream>
using namespace std;

class Lista_Exames
{
    Exame *exame;
    Lista_Exames *prox;
    int tam;
    int num_alunos;
    int num_alunos_real;
    public:
        Lista_Exames();
        virtual ~Lista_Exames();
        friend class Lista_alunos;
        friend class Tabela_Exames;
        void adicionar(Exame*);
        int buscar(int);
        void remover(int);
        void imprime();
        int* retornaElementos();
};

#endif // LISTA_EXAMES_H
