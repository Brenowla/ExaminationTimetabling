#include "Tabela_Exames.h"

bool Tabela_Exames::regras(int regra, int maximo ,int *exames_restantes){
    bool cabe_na_sala;
    int exame1, exame2, sala, periodo,periodo2;

    if(regra == 0){
        exame1 = (rand())%max_exames;
        exame2 = (rand())%max_exames;
        if (exame1 == exame2){
            exame2 =  (exame2+1)%max_exames;
        }
        if(horarios[exames[exame1].periodo].duracao >= exames[exame2].duracao && horarios[exames[exame2].periodo].duracao >= exames[exame1].duracao){
            periodo = exames[exame1].periodo;
            remove_exame(exame1,exames_restantes);
            remove_exame(exame2,exames_restantes);
            sala = rand()%max_salas;
            while(salas[sala].capacidade < exames[exame1].num_alunos){
                sala = (sala+1)%max_salas;
            }
            cabe_na_sala = false;
            if(espacos_restantes[exames[exame2].periodo][sala] >= exames[exame1].num_alunos && !exames[exame1].exclusivo){
                cabe_na_sala = true;
            }
            retira_exames_conflituosos(exame1,exames[exame2].periodo,sala,cabe_na_sala,exames_restantes);
            sala = rand()%max_salas;
            while(salas[sala].capacidade < exames[exame2].num_alunos){
                sala = (sala+1)%max_salas;
            }
            cabe_na_sala = false;
            if(espacos_restantes[periodo][sala] >= exames[exame2].num_alunos && !exames[exame2].exclusivo){
                cabe_na_sala = true;
            }
            retira_exames_conflituosos(exame2,periodo,sala,cabe_na_sala,exames_restantes);
            return Gerartabelahorarios(exames_restantes,maximo,false);
        }
    }
    //fim da primeira regra

    //Segunda regra
    else if(regra == 1){
        exame1 = (rand())%max_exames;
        sala = (rand())%max_salas;
        periodo = (rand())%max_horarios;
        if((sala != exames[exame1].sala || periodo != exames[exame1].periodo) && salas[sala].capacidade >= exames[exame1].num_alunos && horarios[periodo].duracao >= exames[exame1].duracao){
            remove_exame(exame1,exames_restantes);
            cabe_na_sala = false;
            if(espacos_restantes[periodo][sala] >= exames[exame1].num_alunos && !exames[exame1].exclusivo){
                cabe_na_sala = true;
            }
            retira_exames_conflituosos(exame1, periodo,sala,cabe_na_sala,exames_restantes);
            return Gerartabelahorarios(exames_restantes,maximo,false);
        }
    }
    //Fim da segunda regra

    //Terceira regra
    else if(regra == 2){
        exame1 = (rand())%max_exames;
        sala = (rand())%max_salas;
        if((sala != exames[exame1].sala && salas[sala].capacidade >= exames[exame1].num_alunos) || max_salas <= 1){
            remove_exame(exame1,exames_restantes);
            cabe_na_sala = false;
            if(espacos_restantes[exames[exame1].periodo][sala] >= exames[exame1].num_alunos && !exames[exame1].exclusivo){
                cabe_na_sala = true;
            }
            retira_exames_conflituosos(exame1, exames[exame1].periodo, sala, cabe_na_sala, exames_restantes);
            return Gerartabelahorarios(exames_restantes,maximo,false);
        }
    }

    //Fim da terceira regra

    //Quarta regra
    else if(regra == 3){
        exame1 = (rand())%max_exames;
        periodo = (rand())%max_horarios;
        if(periodo != exames[exame1].periodo && horarios[periodo].duracao >= exames[exame1].duracao){
            remove_exame(exame1,exames_restantes);
            cabe_na_sala = false;
            if(espacos_restantes[periodo][exames[exame1].sala] >= exames[exame1].num_alunos && !exames[exame1].exclusivo){
                cabe_na_sala = true;
            }
            retira_exames_conflituosos(exame1, periodo, exames[exame1].sala, cabe_na_sala, exames_restantes);
            return Gerartabelahorarios(exames_restantes,maximo,false);
        }
    }
    //Fim da quarta regra

    else if(regra == 4){
        periodo = (rand())%max_horarios;
        periodo2 = (rand())%max_horarios;
        if(periodo != periodo2){
            int *elementos1[max_salas];
            int *elementos2[max_salas];
            int num_elementos1[max_salas];
            int num_elementos2[max_salas];
            for(int l=0;l<max_salas;l++){
                elementos1[l] = tabela_horarios[periodo][l].retornaElementos();
                num_elementos1[l] = tabela_horarios[periodo][l].tam;
                for(int m=0;m<num_elementos1[l];m++){
                    remove_exame(elementos1[l][m],exames_restantes);
                }
                elementos2[l] = tabela_horarios[periodo2][l].retornaElementos();
                num_elementos2[l] = tabela_horarios[periodo2][l].tam;
                for(int m=0;m<num_elementos2[l];m++){
                    remove_exame(elementos2[l][m],exames_restantes);
                }
            }
            for(int l=0;l<max_salas;l++){
                for(int m=0;m<num_elementos1[l];m++){
                    if(horarios[periodo2].duracao >= exames[elementos1[l][m]].duracao){
                        cabe_na_sala = false;
                        if(espacos_restantes[periodo2][l] >= exames[elementos1[l][m]].num_alunos && !exames[elementos1[l][m]].exclusivo){
                            cabe_na_sala = true;
                        }
                        retira_exames_conflituosos(elementos1[l][m], periodo2, l, cabe_na_sala, exames_restantes);
                    }
                }
                for(int m=0;m<num_elementos2[l];m++){
                    if(horarios[periodo].duracao >= exames[elementos2[l][m]].duracao){
                        cabe_na_sala = false;
                        if(espacos_restantes[periodo][l] >= exames[elementos2[l][m]].num_alunos && !exames[elementos2[l][m]].exclusivo){
                            cabe_na_sala = true;
                        }
                        retira_exames_conflituosos(elementos2[l][m], periodo, l, cabe_na_sala, exames_restantes);
                    }
                }
            }
            for(int l=0;l<max_salas;l++){
                delete[] elementos1[l];
                delete[] elementos2[l];
            }
            return Gerartabelahorarios(exames_restantes,maximo,false);
        }
    }
    //Fim da quinta regra
    //return false;
    return regras(((regra+1)%4),maximo,exames_restantes);
}

void Tabela_Exames::busca_solucoes_annealing(int maximo, float temperatura, float delta,int execucoes){
    int i,  regra;
    int exames_restantes[max_exames];
    int erros_consecutivos = 0;
    bool gerou;
    int temperatura_atual;
    melhor_solucao_exames = exames;
    melhor_peso = calcula_Peso();
    int peso_atual;
    int melhor_peso_annealing = melhor_peso;
    melhor_solucao_exames = new Exame[max_exames]();
    Exame *solucao_atual = new Exame[max_exames]();
    for(i = 0; i < max_exames;i++){
        melhor_solucao_exames[i].clone(exames[i]);
        solucao_atual[i].clone(exames[i]);
        exames_restantes[i] = 0;
    }
    while(erros_consecutivos < execucoes){
        temperatura_atual = temperatura;
        for(i = 0; i < max_exames;i++){
            solucao_atual[i].clone(exames[i]);
            exames_restantes[i] = 0;
        }
        while(temperatura_atual > 0.00000001){
                //cout << "Antes" << endl;
                regra = (rand())%5;
                //cout << "Depois" << endl;
                gerou = regras(regra, maximo, exames_restantes);
                if(!gerou){
                    for(i = 0; i< max_exames;i++){
                        if(melhor_solucao_exames[i].sala != exames[i].sala || melhor_solucao_exames[i].periodo != exames[i].periodo || !exames[i].alocado){
                            tabela_horarios[exames[i].periodo][exames[i].sala].remover(i);
                            espacos_restantes[exames[i].periodo][exames[i].sala] = salas[exames[i].sala].capacidade - tabela_horarios[exames[i].periodo][exames[i].sala].num_alunos;
                            tabela_horarios[melhor_solucao_exames[i].periodo][melhor_solucao_exames[i].sala].adicionar(get_exame(i));
                            espacos_restantes[melhor_solucao_exames[i].periodo][melhor_solucao_exames[i].sala] = salas[melhor_solucao_exames[i].sala].capacidade - tabela_horarios[melhor_solucao_exames[i].periodo][melhor_solucao_exames[i].sala].num_alunos;
                            exames[i].clone(melhor_solucao_exames[i]);
                            exames_restantes[i] = 0;
                        }
                    }

                }
                else{
                    temperatura_atual *= delta;
                    peso_atual = calcula_Peso();
                    if(peso_atual <= melhor_peso_annealing || ((rand() / (float)RAND_MAX)<= pow(M_E, -(peso_atual - melhor_peso_annealing) / temperatura))){
                        for(i = 0; i< max_exames;i++){
                            solucao_atual[i].clone(exames[i]);
                        }
                        melhor_peso_annealing = peso_atual;
                    }
                    else{
                        for(i = 0; i< max_exames;i++){
                            tabela_horarios[exames[i].periodo][exames[i].sala].remover(i);
                            espacos_restantes[exames[i].periodo][exames[i].sala] = salas[exames[i].sala].capacidade - tabela_horarios[exames[i].periodo][exames[i].sala].num_alunos;
                            exames[i].clone(solucao_atual[i]);
                            exames_restantes[i] = 0;
                            tabela_horarios[exames[i].periodo][exames[i].sala].adicionar(get_exame(i));
                            espacos_restantes[exames[i].periodo][exames[i].sala] = salas[exames[i].sala].capacidade - tabela_horarios[exames[i].periodo][exames[i].sala].num_alunos;
                        }
                    }
                }
        }
        if(melhor_peso_annealing < melhor_peso){
            for(i = 0; i< max_exames;i++){
                melhor_solucao_exames[i].clone(solucao_atual[i]);
                melhor_peso = melhor_peso_annealing;
            }
            cout << "Melhor peso: " << melhor_peso << endl;
            erros_consecutivos = 0;
        }
        else{
            erros_consecutivos++;
        }
        for(i = 0; i< max_exames;i++){
            if(melhor_solucao_exames[i].sala != exames[i].sala || melhor_solucao_exames[i].periodo != exames[i].periodo){
                tabela_horarios[exames[i].periodo][exames[i].sala].remover(i);
                espacos_restantes[exames[i].periodo][exames[i].sala] = salas[exames[i].sala].capacidade - tabela_horarios[exames[i].periodo][exames[i].sala].num_alunos;
                tabela_horarios[melhor_solucao_exames[i].periodo][melhor_solucao_exames[i].sala].adicionar(get_exame(i));
                espacos_restantes[melhor_solucao_exames[i].periodo][melhor_solucao_exames[i].sala] = salas[melhor_solucao_exames[i].sala].capacidade - tabela_horarios[melhor_solucao_exames[i].periodo][melhor_solucao_exames[i].sala].num_alunos;
                exames[i].clone(melhor_solucao_exames[i]);
            }
        }
    }
}

void Tabela_Exames::busca_solucoes_hill_LAHC(int maximo, int limiar, int tam_max_tentativas){
    int i,  regra;
    clock_t tInicio, tFim, tDecorrido;
    tInicio = time(NULL);
    int num_regras = 4;
    int erros_consecutivos = 0;
    int max_tentativas = limiar;
    int exames_restantes[max_exames];
    int custo_atual;
    int melhor_custo_iteracao;
    bool gerou;
    int execucoes = 0;
    int Tam_max_lista = tam_max_tentativas * 200;
    int tentativas = 0;
    int peso_atual;
    melhor_solucao_exames = exames;
    Exame *solucao_atual = new Exame[max_exames]();
    melhor_peso = calcula_Peso();
    custo_atual = melhor_peso;
    melhor_custo_iteracao = melhor_peso;
    melhor_solucao_exames = new Exame[max_exames]();
    for(i = 0; i < max_exames;i++){
        melhor_solucao_exames[i].clone(exames[i]);
        solucao_atual[i].clone(exames[i]);
        exames_restantes[i] = 0;
    }
    regra = (rand())%num_regras;
    tFim =  time(NULL);
    while((tFim - tInicio) < 3600 && (erros_consecutivos <= execucoes/50 || execucoes < 300)){
            gerou = regras(regra, maximo, exames_restantes);
            if(!gerou){
                for(i = 0; i< max_exames;i++){
                    if(solucao_atual[i].sala != exames[i].sala || solucao_atual[i].periodo != exames[i].periodo || !exames[i].alocado){
                        tabela_horarios[exames[i].periodo][exames[i].sala].remover(i);
                        espacos_restantes[exames[i].periodo][exames[i].sala] = salas[exames[i].sala].capacidade - tabela_horarios[exames[i].periodo][exames[i].sala].num_alunos;
                        tabela_horarios[solucao_atual[i].periodo][solucao_atual[i].sala].adicionar(get_exame(i));
                        espacos_restantes[solucao_atual[i].periodo][solucao_atual[i].sala] = salas[solucao_atual[i].sala].capacidade - tabela_horarios[solucao_atual[i].periodo][solucao_atual[i].sala].num_alunos;
                        exames[i].clone(solucao_atual[i]);
                        exames_restantes[i] = 0;
                    }
                }
            }
            else{
                peso_atual = calcula_Peso();
                if(peso_atual <= melhor_peso){
                    custo_atual = peso_atual;
                    for(i = 0; i< max_exames;i++){
                        solucao_atual[i].clone(exames[i]);
                    }
                    if(peso_atual < melhor_custo_iteracao){
                        for(i = 0; i < max_exames;i++){
                            melhor_solucao_exames[i].clone(exames[i]);
                        }
                        melhor_custo_iteracao = peso_atual;
                    }
                }
                else{
                    for(i = 0; i< max_exames;i++){
                        if(solucao_atual[i].sala != exames[i].sala || solucao_atual[i].periodo != exames[i].periodo){
                            tabela_horarios[exames[i].periodo][exames[i].sala].remover(i);
                            espacos_restantes[exames[i].periodo][exames[i].sala] = salas[exames[i].sala].capacidade - tabela_horarios[exames[i].periodo][exames[i].sala].num_alunos;
                            tabela_horarios[solucao_atual[i].periodo][solucao_atual[i].sala].adicionar(get_exame(i));
                            espacos_restantes[solucao_atual[i].periodo][solucao_atual[i].sala] = salas[solucao_atual[i].sala].capacidade - tabela_horarios[solucao_atual[i].periodo][solucao_atual[i].sala].num_alunos;
                            exames[i].clone(solucao_atual[i]);
                        }
                    }
                }
                tentativas++;
                if(tentativas > max_tentativas){
                    for(i = 0; i < max_exames;i++){
                        if(melhor_solucao_exames[i].sala != exames[i].sala || melhor_solucao_exames[i].periodo != exames[i].periodo){
                            tabela_horarios[exames[i].periodo][exames[i].sala].remover(i);
                            espacos_restantes[exames[i].periodo][exames[i].sala] = salas[exames[i].sala].capacidade - tabela_horarios[exames[i].periodo][exames[i].sala].num_alunos;
                            tabela_horarios[melhor_solucao_exames[i].periodo][melhor_solucao_exames[i].sala].adicionar(get_exame(i));
                            espacos_restantes[melhor_solucao_exames[i].periodo][melhor_solucao_exames[i].sala] = salas[melhor_solucao_exames[i].sala].capacidade - tabela_horarios[melhor_solucao_exames[i].periodo][melhor_solucao_exames[i].sala].num_alunos;
                            exames[i].clone(melhor_solucao_exames[i]);
                            solucao_atual[i].clone(exames[i]);
                        }
                    }
                    tentativas = 0;
                    if(melhor_custo_iteracao < melhor_peso){
                        erros_consecutivos = 0;
                    }
                    else{
                        erros_consecutivos++;
                    }
                    if((melhor_peso-melhor_custo_iteracao) < (melhor_peso/500)){
                        if(max_tentativas < Tam_max_lista){
                            max_tentativas += tam_max_tentativas;
                        }
                    }
                    custo_atual = melhor_custo_iteracao;
                    melhor_peso = custo_atual;
                    execucoes++;
                    //cout << "Novo peso: " << melhor_peso << endl;
                }
                regra = (rand())%num_regras;
            }
        tFim =  time(NULL);
    }
    //printf("Erros consecutivos %d , Tamanho: %d\n",erros_consecutivos,max_tentativas);
    for(i = 0; i< max_exames;i++){
        if(melhor_solucao_exames[i].sala != exames[i].sala || melhor_solucao_exames[i].periodo != exames[i].periodo){
        tabela_horarios[exames[i].periodo][exames[i].sala].remover(i);
        espacos_restantes[exames[i].periodo][exames[i].sala] = salas[exames[i].sala].capacidade - tabela_horarios[exames[i].periodo][exames[i].sala].num_alunos;
        tabela_horarios[melhor_solucao_exames[i].periodo][melhor_solucao_exames[i].sala].adicionar(get_exame(i));
        espacos_restantes[melhor_solucao_exames[i].periodo][melhor_solucao_exames[i].sala] = salas[melhor_solucao_exames[i].sala].capacidade - tabela_horarios[melhor_solucao_exames[i].periodo][melhor_solucao_exames[i].sala].num_alunos;
        exames[i].clone(melhor_solucao_exames[i]);
        }
    }
    delete[] solucao_atual;
}

void Tabela_Exames::busca_solucoes_hill(int maximo){
    int i, j,  regra;
    int exames_restantes[max_exames];
    bool gerou;
    int peso_atual;
    melhor_solucao_exames = exames;
    melhor_peso = calcula_Peso();
    melhor_solucao_exames = new Exame[max_exames]();
    for(i = 0; i < max_exames;i++){
        melhor_solucao_exames[i].clone(exames[i]);
        exames_restantes[i] = 0;
    }
    for(j=0;j<25000;j++){
            regra = 1;//(rand())%5;
            gerou = regras(regra, maximo, exames_restantes);
            if(!gerou){
                for(i = 0; i< max_exames;i++){
                    if(melhor_solucao_exames[i].sala != exames[i].sala || melhor_solucao_exames[i].periodo != exames[i].periodo || !exames[i].alocado){
                            tabela_horarios[exames[i].periodo][exames[i].sala].remover(i);
                            espacos_restantes[exames[i].periodo][exames[i].sala] = salas[exames[i].sala].capacidade - tabela_horarios[exames[i].periodo][exames[i].sala].num_alunos;
                            tabela_horarios[melhor_solucao_exames[i].periodo][melhor_solucao_exames[i].sala].adicionar(get_exame(i));
                            espacos_restantes[melhor_solucao_exames[i].periodo][melhor_solucao_exames[i].sala] = salas[melhor_solucao_exames[i].sala].capacidade - tabela_horarios[melhor_solucao_exames[i].periodo][melhor_solucao_exames[i].sala].num_alunos;
                            exames[i].clone(melhor_solucao_exames[i]);
                            exames_restantes[i] = 0;
                    }
                }
                j--;
            }
            else{
                peso_atual = calcula_Peso();
                if(peso_atual <= melhor_peso){
                    for(i = 0; i< max_exames;i++){
                        melhor_solucao_exames[i].clone(exames[i]);
                    }
                    melhor_peso = peso_atual;
                }
                else{
                    for(i = 0; i < max_exames;i++){
                        if(melhor_solucao_exames[i].sala != exames[i].sala || melhor_solucao_exames[i].periodo != exames[i].periodo){
                            tabela_horarios[exames[i].periodo][exames[i].sala].remover(i);
                            espacos_restantes[exames[i].periodo][exames[i].sala] = salas[exames[i].sala].capacidade - tabela_horarios[exames[i].periodo][exames[i].sala].num_alunos;
                            tabela_horarios[melhor_solucao_exames[i].periodo][melhor_solucao_exames[i].sala].adicionar(get_exame(i));
                            espacos_restantes[melhor_solucao_exames[i].periodo][melhor_solucao_exames[i].sala] = salas[melhor_solucao_exames[i].sala].capacidade - tabela_horarios[melhor_solucao_exames[i].periodo][melhor_solucao_exames[i].sala].num_alunos;
                            exames[i].clone(melhor_solucao_exames[i]);
                        }
                    }
                }
            }
    }
    for(i = 0; i< max_exames;i++){
        if(melhor_solucao_exames[i].sala != exames[i].sala || melhor_solucao_exames[i].periodo != exames[i].periodo){
        tabela_horarios[exames[i].periodo][exames[i].sala].remover(i);
        espacos_restantes[exames[i].periodo][exames[i].sala] = salas[exames[i].sala].capacidade - tabela_horarios[exames[i].periodo][exames[i].sala].num_alunos;
        tabela_horarios[melhor_solucao_exames[i].periodo][melhor_solucao_exames[i].sala].adicionar(get_exame(i));
        espacos_restantes[melhor_solucao_exames[i].periodo][melhor_solucao_exames[i].sala] = salas[melhor_solucao_exames[i].sala].capacidade - tabela_horarios[melhor_solucao_exames[i].periodo][melhor_solucao_exames[i].sala].num_alunos;
        exames[i].clone(melhor_solucao_exames[i]);
        }
    }
}

