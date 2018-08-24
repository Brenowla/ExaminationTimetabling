#ifndef LISTA_ALUNOS_H
#define LISTA_ALUNOS_H
#include "Lista_Exames.h"
#include "Exame.h"

#include <iostream>
#include <stdlib.h>

using namespace std;


class Lista_alunos
{
    int aluno;
    Lista_Exames *exames;
    Lista_alunos *prox;
    public:
        Lista_Exames* adicionar_aluno(int, Exame*);
        void adicionar_exame(Exame*);
        void imprime_lista();
        Lista_alunos();
        Lista_alunos(int,Exame*);
        virtual ~Lista_alunos();
        friend class Tabela_Exames;

};

#endif // LISTA_ALUNOS_H
