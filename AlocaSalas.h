//
// Created by samuel on 19/07/18.
//

#ifndef EXAMINATIONTIMETABLING_ALOCASALAS_H
#define EXAMINATIONTIMETABLING_ALOCASALAS_H

#include <vector>
#include <map>
#include "Tabela_Exames.h"

extern "C" {
    #include "lp.h"
}

class AlocaSalas {
public:
    AlocaSalas(Tabela_Exames *_tabela, const vector<int> &_exames);
    ~AlocaSalas();

    double executaModelo(const int tempoLimite = 60);

private:
    Tabela_Exames *tabela;
    vector<int> exames;
    int numDuracoes;
    map<int, int> duracoesDistintas;

    LinearProgram *mip;
    vector<vector<int> > xIdx; //xij = 1 se o exame i é alocado na sala j
    map<int, pair<int, int> > idxX;
    int iniVarsX, fimVarsX;
    vector<int> yIdx; //yj = número de exames escalonados na sala j
    vector<vector<int> > zIdx; //zkj = 1 se uma duração k é escalonada na sala j
    vector<int> wIdx; //wj = número de duracoes distintas alocadas na sala j
    int numCols;

    vector<string> nomesVar;
    vector<double> obj;
    vector<double> lb;
    vector<double> ub;
    vector<char> integer;

    void criaVarsX();
    void criaVarsY();
    void criaVarsZ();
    void criaVarsW();

    void restricaoAlocaSalaExame();
    void restricaoLigaXeY();
    void restricaoCapacidade();
    void restricaoLigaXeZ();
    void restricaoLigaZeW();

    void preencheSolucao();
};


#endif //EXAMINATIONTIMETABLING_ALOCASALAS_H
