//
// Created by samuel on 19/07/18.
//

#include <cassert>
#include "AlocaSalas.h"
#include "StrUtil.hpp"

AlocaSalas::AlocaSalas(Tabela_Exames *_tabela, const vector<int> &_exames) : tabela(_tabela), exames(_exames) {
    mip = lp_create();
    this->numCols = 0;
    this->numDuracoes = 0;

//    cout<<"Exames do periodo "<<periodo<<": ";
    for(const int exame : this->exames) {
        const Exame *e = this->tabela->get_exame(exame);
        const int duracao = e->getDuracao();
        auto it = this->duracoesDistintas.find(duracao);
        if(it == this->duracoesDistintas.end()) {
            this->duracoesDistintas.insert(pair<int, int>(duracao, this->numDuracoes++));
        }
//        cout<<e->get_num()<<" ";
    }
//    cout<<endl;

    assert(this->numDuracoes == this->duracoesDistintas.size());

    criaVarsX();
    criaVarsY();
    criaVarsZ();
    criaVarsW();

    char **svn = StrUtil::convert(nomesVar);
    lp_add_cols( mip, (int)nomesVar.size(), &obj[0], &lb[0], &ub[0], &integer[0], svn );
    delete[] svn[0];
    delete[] svn;

    restricaoAlocaSalaExame();
    restricaoCapacidade();
    restricaoLigaXeY();
    restricaoLigaXeZ();
    restricaoLigaZeW();

//    lp_write_lp(mip, "teste.lp");
}

AlocaSalas::~AlocaSalas() {
    lp_free(&mip);
}

void AlocaSalas::criaVarsX() {
    char nome[256];

    this->iniVarsX = this->numCols;
    this->xIdx = vector<vector<int> >(this->exames.size(), vector<int>(this->tabela->getNumSalas()));

    for(size_t i = 0; i < this->exames.size(); i++) {
        for(int j = 0; j < this->tabela->getNumSalas(); j++) {
            xIdx[i][j] = this->numCols;
            idxX.insert(pair<int, pair<int, int> >(this->numCols, pair<int, int>(i, j)));
            this->numCols++;
            sprintf(nome, "x(%lu,%d)", i, j);
            nomesVar.emplace_back(nome);
            obj.push_back(0.0);
            lb.push_back(0.0);
            ub.push_back(1.0);
            integer.push_back(1);
        }
    }

    this->fimVarsX = this->numCols - 1;
}

void AlocaSalas::criaVarsY() {
    char nome[256];
    const Salas *salas = this->tabela->getSalas();

    this->yIdx = vector<int>(this->tabela->getNumSalas());

    for(int j = 0; j < this->tabela->getNumSalas(); j++) {
        sprintf(nome, "y(%d)", j);
        yIdx[j] = this->numCols++;
        nomesVar.emplace_back(nome);
        obj.push_back(salas[j].getPenalidade());
        lb.push_back(0.0);
        ub.push_back(this->exames.size());
        integer.push_back(1);
    }
}

void AlocaSalas::criaVarsZ() {
    char nome[256];

    this->zIdx = vector<vector<int> >(this->numDuracoes, vector<int>(this->tabela->getNumSalas()));

    for(int k = 0; k < this->numDuracoes; k++) {
        for(int j = 0; j < this->tabela->getNumSalas(); j++) {
            zIdx[k][j] = this->numCols++;
            sprintf(nome, "z(%d,%d)", k, j);
            nomesVar.emplace_back(nome);
            obj.push_back(0.0);
            lb.push_back(0.0);
            ub.push_back(1.0);
            integer.push_back(1);
        }
    }
}

void AlocaSalas::criaVarsW() {
    char nome[256];
    const Salas *salas = this->tabela->getSalas();

    this->wIdx = vector<int>(this->tabela->getNumSalas());

    for(int j = 0; j < this->tabela->getNumSalas(); j++) {
        sprintf(nome, "w(%d)", j);
        wIdx[j] = this->numCols++;
        nomesVar.emplace_back(nome);
        obj.push_back(this->tabela->getPenalDifDuracoes());
        lb.push_back(0.0);
        ub.push_back(this->numDuracoes);
        integer.push_back(1);
    }
}

void AlocaSalas::restricaoAlocaSalaExame() {
    char nome[256];
    int numRows = 0;
    vector<int> inicio, idxs;
    vector<double> coefs, rhs;
    vector<char> sense;
    vector<string> nomes;

    inicio.push_back(0);

    for(size_t i = 0; i < this->exames.size(); i++) {
        for(int j = 0; j < this->tabela->getNumSalas(); j++) {
            idxs.push_back(xIdx[i][j]);
            coefs.push_back(1.0);
        }

        sprintf(nome, "alocSala(%lu)", i);
        nomes.emplace_back(nome);
        sense.push_back('E');
        rhs.push_back(1.0);
        inicio.push_back((int)idxs.size());
        numRows++;
    }

    assert(numRows == inicio.size()-1);
    assert(numRows == sense.size());
    assert(numRows == rhs.size());

    char **svn = StrUtil::convert(nomes);
    lp_add_rows(mip, numRows, &inicio[0], &idxs[0], &coefs[0], &sense[0], &rhs[0], (const char**)svn);

    delete[] svn[0];
    delete[] svn;
}

void AlocaSalas::restricaoLigaXeY() {
    char nome[256];
    int numRows = 0;
    vector<int> inicio, idxs;
    vector<double> coefs, rhs;
    vector<char> sense;
    vector<string> nomes;

    inicio.push_back(0);

    for(int j = 0; j < this->tabela->getNumSalas(); j++) {
        for(size_t i = 0; i < this->exames.size(); i++) {
            const Exame *e = this->tabela->get_exame(this->exames[i]);
            idxs.push_back(xIdx[i][j]);
            coefs.push_back(1.0);
        }

        idxs.push_back(yIdx[j]);
        coefs.push_back(-1.0);

        sprintf(nome, "ligaXeY(%d)", j);
        nomes.emplace_back(nome);
        sense.push_back('E');
        rhs.push_back(0.0);
        inicio.push_back((int)idxs.size());
        numRows++;
    }

    assert(numRows == inicio.size()-1);
    assert(numRows == sense.size());
    assert(numRows == rhs.size());

    char **svn = StrUtil::convert(nomes);
    lp_add_rows(mip, numRows, &inicio[0], &idxs[0], &coefs[0], &sense[0], &rhs[0], (const char**)svn);

    delete[] svn[0];
    delete[] svn;
}

void AlocaSalas::restricaoCapacidade() {
    char nome[256];
    const Salas *salas = this->tabela->getSalas();
    int numRows = 0;
    vector<int> inicio, idxs;
    vector<double> coefs, rhs;
    vector<char> sense;
    vector<string> nomes;

    inicio.push_back(0);

    for(int j = 0; j < this->tabela->getNumSalas(); j++) {
        for(size_t i = 0; i < this->exames.size(); i++) {
            const Exame *e = this->tabela->get_exame(this->exames[i]);
            idxs.push_back(xIdx[i][j]);
            if(e->salaExclusiva())
                coefs.push_back(salas[j].getCapacidade());
            else
                coefs.push_back(e->getNumAlunos());
        }

        sprintf(nome, "capacidadeSala(%d)", j);
        nomes.emplace_back(nome);
        sense.push_back('L');
        rhs.push_back(salas[j].getCapacidade());
        inicio.push_back((int)idxs.size());
        numRows++;
    }

    assert(numRows == inicio.size()-1);
    assert(numRows == sense.size());
    assert(numRows == rhs.size());

    char **svn = StrUtil::convert(nomes);
    lp_add_rows(mip, numRows, &inicio[0], &idxs[0], &coefs[0], &sense[0], &rhs[0], (const char**)svn);

    delete[] svn[0];
    delete[] svn;
}

void AlocaSalas::restricaoLigaXeZ() {
    char nome[256];
    const Salas *salas = this->tabela->getSalas();
    int numRows = 0;
    vector<int> inicio, idxs;
    vector<double> coefs, rhs;
    vector<char> sense;
    vector<string> nomes;

    inicio.push_back(0);

    for(size_t i = 0; i < this->exames.size(); i++) {
        const Exame *e = this->tabela->get_exame(this->exames[i]);
        const int duracao = e->getDuracao();

        for(int j = 0; j < this->tabela->getNumSalas(); j++) {
            auto it = this->duracoesDistintas.find(duracao);
            const int k = it->second;
            assert(it != this->duracoesDistintas.end());
            idxs.push_back(zIdx[k][j]);
            coefs.push_back(1.0);

            idxs.push_back(xIdx[i][j]);
            coefs.push_back(-1.0);

            sprintf(nome, "ligaXeZ(%lu,%d)", i, j);
            nomes.emplace_back(nome);
            sense.push_back('G');
            rhs.push_back(0.0);
            inicio.push_back((int)idxs.size());
            numRows++;
        }
    }

    assert(numRows == inicio.size()-1);
    assert(numRows == sense.size());
    assert(numRows == rhs.size());

    char **svn = StrUtil::convert(nomes);
    lp_add_rows(mip, numRows, &inicio[0], &idxs[0], &coefs[0], &sense[0], &rhs[0], (const char**)svn);

    delete[] svn[0];
    delete[] svn;
}

void AlocaSalas::restricaoLigaZeW() {
    char nome[256];
    int numRows = 0;
    vector<int> inicio, idxs;
    vector<double> coefs, rhs;
    vector<char> sense;
    vector<string> nomes;

    inicio.push_back(0);

    for(int j = 0; j < this->tabela->getNumSalas(); j++) {
        for(int k = 0; k < this->numDuracoes; k++) {
            idxs.push_back(zIdx[k][j]);
            coefs.push_back(1.0);
        }

        idxs.push_back(wIdx[j]);
        coefs.push_back(-1.0);

        sprintf(nome, "ligaZeW(%d)", j);
        nomes.emplace_back(nome);
        sense.push_back('E');
        rhs.push_back(1.0);
        inicio.push_back((int)idxs.size());
        numRows++;
    }

    assert(numRows == inicio.size()-1);
    assert(numRows == sense.size());
    assert(numRows == rhs.size());

    char **svn = StrUtil::convert(nomes);
    lp_add_rows(mip, numRows, &inicio[0], &idxs[0], &coefs[0], &sense[0], &rhs[0], (const char**)svn);

    delete[] svn[0];
    delete[] svn;
}

double AlocaSalas::executaModelo(const int tempoLimite) {
    lp_set_max_seconds(this->mip, tempoLimite);
    lp_set_print_messages(this->mip, 0);

    int status = lp_optimize(this->mip);
//    lp_write_sol(this->mip, "teste.sol");

    if(status == LP_OPTIMAL || status == LP_FEASIBLE) {
        preencheSolucao();
    }
    else if(status == LP_INFEASIBLE || status == LP_INTINFEASIBLE || status == LP_ERROR) {
        fprintf(stderr, "Erro ao executar o modelo! Status %d\n", status);
        exit(1);
    }
    else {
        fprintf(stderr, "Nao executou!\n");
        exit(1);
    }
    return lp_obj_value(mip);
}

void AlocaSalas::preencheSolucao() {
    const double *x = lp_x(this->mip);

    for(int i = this->iniVarsX; i <= this->fimVarsX; i++) {
        if(fabs(x[i]) >= 0.98) {
            auto it = this->idxX.find(i);
            assert(it != this->idxX.end());
            const pair<int, int> idxs = it->second;

            const int posExame = this->exames[idxs.first];
            const int posSala = idxs.second;
            this->tabela->setSalaExame(posExame, posSala);
        }
    }
}
