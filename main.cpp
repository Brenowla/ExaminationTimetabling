#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Arquivo.h"
#include "Tabela_Exames.h"
#include "AlocaSalas.h"

using namespace std;

int main(int argc, char *argv[ ])
{
    clock_t tInicio;
    double tDecorrido;
    //FILE *saida;
    //saida = fopen("resultado.csv","a");
    tInicio = clock();
    srand(time(NULL));
    Tabela_Exames *tabela;
    tabela = new Tabela_Exames();
    ler_dados(tabela,argv[1]);
    tabela->inicializa_tabela_horarios();
    int exames_nao_alocados[tabela->getMax_Exames()];
    int i;
    for(i =0;i<tabela->getMax_Exames();i++){
        exames_nao_alocados[i]=1;
    }
    tabela->atualiza_disponibilidade();
    tabela->Gerartabelahorarios(exames_nao_alocados,-1,true);
    tabela->busca_solucoes_hill_LAHC(atoi(argv[2]),atoi(argv[3]),atoi(argv[4]));

    int melhorPeso = tabela->get_melhor_peso();

    if(!tabela->conferir_solucao()){
        cout << "Solucao invalida" << endl;
    }

    tDecorrido = ((double)(clock()-tInicio)) / ((double)CLOCKS_PER_SEC);

    if( (tDecorrido < 3600.0) && (tabela->getNumSalas() > 1) && (tabela->getSomaPenalSalas() + tabela->getPenalDifDuracoes() > 0)) {
        vector<vector<int> > examesPorPeriodo((tabela->getNumPeriodos()));
        for(int i = 0; i < tabela->getMax_Exames(); i++) {
            const Exame *e = tabela->get_exame(i);
            const int horario = e->getPeriodo();
            assert(i == e->getNum());
            examesPorPeriodo[horario].push_back(i);
        }

        double t = 0.0;
        for (int k = 0; k < tabela->getNumPeriodos(); k++) {

            tDecorrido = ((double)(clock()-tInicio)) / ((double)CLOCKS_PER_SEC);
            if(tDecorrido > 3600.0)
                break;

            if(examesPorPeriodo[k].empty())
                continue;

            AlocaSalas aloca(tabela, examesPorPeriodo[k]);
            t += aloca.executaModelo();
        }

        const int pesoNovo = tabela->calcula_Peso();
        assert(pesoNovo <= melhorPeso);
        melhorPeso = pesoNovo;

        if(!tabela->conferir_solucao()){
            cout << "Solucao invalida" << endl;
        }
    }

    tDecorrido = ((double)(clock()-tInicio)) / ((double)CLOCKS_PER_SEC);
    printf("%s;%d;%d;%d;%.2lf;%d\n",argv[1],atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),tDecorrido,melhorPeso);
    //tabela->saida();
    //fprintf(saida,"%s;%d;%d;%d;%ld;%d\n",argv[1],atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),tDecorrido,tabela->get_melhor_peso());
    //fclose(saida);

    return 0;
}
