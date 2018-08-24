#ifndef TABELA_EXAMES_H
#define TABELA_EXAMES_H
#include "Exame.h"
#include "Lista_Exames.h"
#include "Periodo.h"
#include "Salas.h"
#include "Pesos.h"

#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <set>
#include <string.h>
#include <cmath>

using namespace std;

class Tabela_Exames
{
    int **conflitos;
    int conflitos_totais;
    set<int> *exclusao;
    set<int> *antes;
    set<int> *depois;
    set<int> *coincidencia;
    Exame *exames;
    Exame *melhor_solucao_exames;
    int max_exames;
    Periodo *horarios;
    int max_horarios;
    Salas *salas;
    int max_salas;
    Pesos pesos;
    Lista_Exames **tabela_horarios;
    int **espacos_restantes;
    int penalidade_total;
    int melhor_peso;

    //Samuel
    int somaPenalidadeSalas;

    public:
        int p;
        Tabela_Exames();
        void saida();
        void inicializa(int);
        void inicializa_periodos(int);
        void inicializa_salas(int);
        void inicializa_pesos(int,int,int,int,int,int,int);
        void inicializa_tabela_horarios();
        void adicionar_Exame(int,int);
        void adicionar_Horarios(int,int,int,char*);
        void adicionar_Salas(int,int,int);
        void adicionar_Exclusao(int,int);
        void adicionar_Depois(int,int);
        void adicionar_Coincidencia(int,int);
        void adicionar_exclusividade(int);
        void adicionar_alunosExame(int,int);
        void aloca_exame_a_horario_pelo_menos_um_disponivel(int);
        void aloca_exame_a_horario_sem_disponiveis(int *,int,bool);
        void retira_exames_conflituosos(int,int,int,bool,int*);
        void atualiza_disponibilidade_exame_desalocado(int);
        void atualiza_lista_exames_nao_alocados(int*,int);
        void atualiza_exame_especifico(int,int);
        void atualiza_lista_por_exame_desalocado(int*, int);
        void atualiza_conflitos(int, Lista_Exames*);
        void remove_exame(int , int *);
        void reseta_alocados();
        int calcula_Peso();
        bool conferir_solucao();
        virtual ~Tabela_Exames();
        void imprime_conflitos();
        void imprime_periodos();
        void imprime_salas();
        void imprime_pesos();
        void imprime_restricoes();
        void imprime_tabelahorarios();
        void imprime_disponibilidade();
        Exame* get_exame(int);
        int getMax_Exames();
        int get_melhor_peso();
        void atualiza_disponibilidade();
        bool Gerartabelahorarios(int*,int,bool);
        void busca_solucoes_hill_LAHC(int,int,int);
        void busca_solucoes_hill(int);
        void busca_solucoes_annealing(int,float,float,int);
        bool regras(int,int,int*);
        int achar_exame_prioritario(int*,int*);

        //Samuel
        const Salas* getSalas() const { return salas; }
        const int getNumSalas() const { return max_salas; }
        const int getPenalDifDuracoes() const { return pesos.mist_horarios; }
        const int getNumPeriodos() const { return max_horarios; }
        const int getSomaPenalSalas() const { return somaPenalidadeSalas; }
        void setSalaExame(const int, const int);
};

#endif // TABELA_EXAMES_H
