#include "Tabela_Exames.h"


int Tabela_Exames::achar_exame_prioritario(int* exames_nao_alocados,int* restantes){
    int i,menos_disponiveis = max_horarios,exame_escolhido;
    bool primeiro = true;
    menos_disponiveis = 9999999;
    (*restantes) = 0;
    for(i=0;i<max_exames;i++){
        if(exames_nao_alocados[i] >0){
            atualiza_disponibilidade_exame_desalocado(i);
            (*restantes)++;
            if(primeiro){
                exame_escolhido = i;
                menos_disponiveis = exames[i].num_disponiveis;
                primeiro = false;
            }
            else if(exames[i].num_disponiveis < menos_disponiveis && rand()%2 < 1){
                exame_escolhido = i;
                menos_disponiveis = exames[i].num_disponiveis;
            }
        }

    }
    return exame_escolhido;
}

bool Tabela_Exames::Gerartabelahorarios(int *exames_nao_alocados, int maximo, bool primeira_solucao){
    int exame_atual;
    int exames_restantes = 0;
    int execucoes = 0;
    exame_atual = achar_exame_prioritario(exames_nao_alocados, &exames_restantes);
    while(exames_restantes > 0){
        if(maximo>=0 && (exames_restantes>maximo || execucoes>maximo)){
            return false;
        }
        if(exames[exame_atual].num_disponiveis >0){
            aloca_exame_a_horario_pelo_menos_um_disponivel(exame_atual);
            exames_restantes --;
            exames_nao_alocados[exame_atual] =0;
            //atualiza_lista_exames_nao_alocados(exames_nao_alocados,exame_atual);
        }
        else{
            aloca_exame_a_horario_sem_disponiveis(exames_nao_alocados,exame_atual,primeira_solucao);
        }
        exame_atual = achar_exame_prioritario(exames_nao_alocados, &exames_restantes);
        execucoes++;
    }
    return true;
}

void Tabela_Exames::aloca_exame_a_horario_pelo_menos_um_disponivel(int exame_atual){
    int i,j;
    bool cabe = false;
    i = (rand())%(max_horarios);
    while(true){
        if(exames[exame_atual].horariosdisponiveis[i] == 1){
            j = (rand())%(max_salas);
            while(!cabe){
                if(!exames[exame_atual].exclusivo){
                    if(espacos_restantes[i][j] >= exames[exame_atual].num_alunos){
                        cabe = true;
                    }
                }else{
                    if(tabela_horarios[i][j].tam == 0 &&  espacos_restantes[i][j] >= exames[exame_atual].num_alunos){
                        cabe = true;
                    }
                }
                if(!cabe){
                    j = (j+1)%(max_salas);
                }
            }
            tabela_horarios[i][j].adicionar(get_exame(exame_atual));
            if(!exames[exame_atual].exclusivo){
                espacos_restantes[i][j] = salas[j].capacidade - tabela_horarios[i][j].num_alunos;
            }
            else{
                espacos_restantes[i][j] = 0;
            }
            exames[exame_atual].periodo = i;
            exames[exame_atual].sala = j;
            exames[exame_atual].alocado = true;
            return;
        }
        i = (i+1)%(max_horarios);
    }
}

void Tabela_Exames::aloca_exame_a_horario_sem_disponiveis(int *exames_nao_alocados,int exame_atual,bool primeira_solucao){
    int horario_escolhido;
    int sala_escolhida;
    bool tem_salas = false;
    horario_escolhido = rand()%max_horarios;
    while(horarios[horario_escolhido].duracao < exames[exame_atual].duracao){
        horario_escolhido = (horario_escolhido+1)%max_horarios;
    }
    sala_escolhida = rand()%max_salas;
    while(salas[sala_escolhida].capacidade < exames[exame_atual].num_alunos){
        sala_escolhida = (sala_escolhida+1)%max_salas;
    }
    if(espacos_restantes[horario_escolhido][sala_escolhida] >= exames[exame_atual].num_alunos && !exames[exame_atual].exclusivo){
        tem_salas = true;
    }
    retira_exames_conflituosos(exame_atual,horario_escolhido,sala_escolhida,tem_salas,exames_nao_alocados);
}

void Tabela_Exames::retira_exames_conflituosos(int exame_atual,int horario_escolhido,int sala_escolhida,bool tem_salas_que_cabem,int* exames_nao_alocados){
    int *elementos;
    int tamanho, k, i;
    int exame_desalocado;
    if(tem_salas_que_cabem){
        tabela_horarios[horario_escolhido][sala_escolhida].adicionar(get_exame(exame_atual));
        exames[exame_atual].periodo = horario_escolhido;
        exames[exame_atual].sala = sala_escolhida;
        exames[exame_atual].alocado = true;
        exames_nao_alocados[exame_atual] = 0;
    if(exames[exame_atual].exclusivo){
        espacos_restantes[horario_escolhido][sala_escolhida] =0;
    }
    else{
        espacos_restantes[horario_escolhido][sala_escolhida] = salas[sala_escolhida].capacidade - tabela_horarios[horario_escolhido][sala_escolhida].num_alunos;
        }
    }
    else{
        elementos = tabela_horarios[horario_escolhido][sala_escolhida].retornaElementos();
        tamanho = tabela_horarios[horario_escolhido][sala_escolhida].tam;
        for(k=0; k < tamanho ;k++){
            remove_exame(elementos[k],exames_nao_alocados);
            if(!exames[exame_atual].exclusivo && espacos_restantes[horario_escolhido][sala_escolhida] > exames[exame_atual].num_alunos){
                break;
            }
        }
        free(elementos);
        tabela_horarios[horario_escolhido][sala_escolhida].adicionar(get_exame(exame_atual));
        espacos_restantes[horario_escolhido][sala_escolhida] = salas[sala_escolhida].capacidade - tabela_horarios[horario_escolhido][sala_escolhida].num_alunos;
        exames[exame_atual].periodo = horario_escolhido;
        exames[exame_atual].sala = sala_escolhida;
        exames[exame_atual].alocado = true;
        exames_nao_alocados[exame_atual] = 0;
        if(exames[exame_atual].exclusivo){
            espacos_restantes[horario_escolhido][sala_escolhida] = 0;
        }
        else{
            espacos_restantes[horario_escolhido][sala_escolhida] = salas[sala_escolhida].capacidade - tabela_horarios[horario_escolhido][sala_escolhida].num_alunos;
        }
        atualiza_lista_exames_nao_alocados(exames_nao_alocados,exame_atual);
    }
    for(i=0; i<max_salas;i++){
        if(tabela_horarios[horario_escolhido][i].tam > 0){
            elementos = tabela_horarios[horario_escolhido][i].retornaElementos();
            tamanho = tabela_horarios[horario_escolhido][i].tam;
            for(k=0;k<tamanho;k++){
                if(conflitos[exame_atual][elementos[k]] > 0){
                    remove_exame(elementos[k],exames_nao_alocados);
                }
            }
            free(elementos);
        }
    }
     if(exames[exame_atual].coincidencia){
        for (set<int>::iterator it = coincidencia[exame_atual].begin();it != coincidencia[exame_atual].end();it++){
            if(exames[*it].alocado){
                if(exames[exame_atual].periodo != exames[*it].periodo){
                    exame_desalocado = *it;
                    tabela_horarios[exames[exame_desalocado].periodo][exames[exame_desalocado].sala].remover(exame_desalocado);
                    if(exames[exame_desalocado].exclusivo){
                        espacos_restantes[exames[exame_desalocado].periodo][exames[exame_desalocado].sala] = salas[exames[exame_desalocado].sala].capacidade;
                    }
                    else{
                        espacos_restantes[exames[exame_desalocado].periodo][exames[exame_desalocado].sala] = espacos_restantes[exames[exame_desalocado].periodo][exames[exame_desalocado].sala] + exames[exame_desalocado].num_alunos;
                    }
                    exames[exame_desalocado].alocado = false;
                    atualiza_lista_por_exame_desalocado(exames_nao_alocados, exame_desalocado);
                    exames_nao_alocados[exame_desalocado] = 1;
                }
            }
        }
    }
    if(exames[exame_atual].antes){
        for (set<int>::iterator it = antes[exame_atual].begin();it!= antes[exame_atual].end();it++){
            if(exames[*it].periodo <= exames[exame_atual].periodo){
                exame_desalocado = *it;
                tabela_horarios[exames[exame_desalocado].periodo][exames[exame_desalocado].sala].remover(exame_desalocado);
                if(exames[exame_desalocado].exclusivo){
                    espacos_restantes[exames[exame_desalocado].periodo][exames[exame_desalocado].sala] = salas[exames[exame_desalocado].sala].capacidade;
                }
                else{
                    espacos_restantes[exames[exame_desalocado].periodo][exames[exame_desalocado].sala] = espacos_restantes[exames[exame_desalocado].periodo][exames[exame_desalocado].sala] + exames[exame_desalocado].num_alunos;
                }
                exames[exame_desalocado].alocado = false;
                atualiza_lista_por_exame_desalocado(exames_nao_alocados, exame_desalocado);
                exames_nao_alocados[exame_desalocado] = 1;
            }
        }
    }
    if(exames[exame_atual].depois){
        for (set<int>::iterator it = depois[exame_atual].begin();it != depois[exame_atual].end();it++){
            if(exames[*it].periodo >= exames[exame_atual].periodo){
                exame_desalocado = *it;
                tabela_horarios[exames[exame_desalocado].periodo][exames[exame_desalocado].sala].remover(exame_desalocado);
                if(exames[exame_desalocado].exclusivo){
                    espacos_restantes[exames[exame_desalocado].periodo][exames[exame_desalocado].sala] = salas[exames[exame_desalocado].sala].capacidade;
                }
                else{
                    espacos_restantes[exames[exame_desalocado].periodo][exames[exame_desalocado].sala] = espacos_restantes[exames[exame_desalocado].periodo][exames[exame_desalocado].sala] + exames[exame_desalocado].num_alunos;
                }
                exames[exame_desalocado].alocado = false;
                atualiza_lista_por_exame_desalocado(exames_nao_alocados, exame_desalocado);
                exames_nao_alocados[exame_desalocado] = 1;
            }
        }
    }
    if(exames[i].exclusao){
        for (set<int>::iterator it = exclusao[exame_atual].begin();it != exclusao[i].end();it++){
            if(exames[*it].periodo == exames[exame_atual].periodo){
                exame_desalocado = *it;
                tabela_horarios[exames[exame_desalocado].periodo][exames[exame_desalocado].sala].remover(exame_desalocado);
                if(exames[exame_desalocado].exclusivo){
                    espacos_restantes[exames[exame_desalocado].periodo][exames[exame_desalocado].sala] = salas[exames[exame_desalocado].sala].capacidade;
                }
                else{
                    espacos_restantes[exames[exame_desalocado].periodo][exames[exame_desalocado].sala] = espacos_restantes[exames[exame_desalocado].periodo][exames[exame_desalocado].sala] + exames[exame_desalocado].num_alunos;
                }
                exames[exame_desalocado].alocado = false;
                atualiza_lista_por_exame_desalocado(exames_nao_alocados, exame_desalocado);
                exames_nao_alocados[exame_desalocado] = 1;
            }
        }
    }
}

void Tabela_Exames::atualiza_disponibilidade_exame_desalocado(int exame_desalocado){
    int i,j,k;
    int *elementos;
    bool cabe;
    exames[exame_desalocado].num_disponiveis = max_horarios;
    for(i=0;i<max_horarios;i++){
        exames[exame_desalocado].horariosdisponiveis[i] = 1;
    }
    for(i=0;i<max_horarios;i++){
        if(horarios[i].duracao < exames[exame_desalocado].duracao){
            exames[exame_desalocado].horariosdisponiveis[i] =0;
        }
        cabe = false;
        for(j=0;j<max_salas;j++){
            if(tabela_horarios[i][j].tam > 0){
                elementos = tabela_horarios[i][j].retornaElementos();
                for(k=0; k < tabela_horarios[i][j].tam; k++){
                    atualiza_exame_especifico(exame_desalocado,elementos[k]);
                }
                free(elementos);
            }
            if(!cabe){
                if(!exames[exame_desalocado].exclusivo){
                        if(espacos_restantes[i][j] >= exames[exame_desalocado].num_alunos){
                            cabe = true;
                        }
                    }
                else if(tabela_horarios[i][j].tam == 0 && espacos_restantes[i][j] >= exames[exame_desalocado].num_alunos){
                        cabe = true;
                }
            }
        }
        if(!cabe){
           exames[exame_desalocado].horariosdisponiveis[i] = 0;
        }
    }
    exames[exame_desalocado].num_disponiveis = 0;
        for(j=0;j<max_horarios;j++){
            if(exames[exame_desalocado].horariosdisponiveis[j] == 1){
                exames[exame_desalocado].num_disponiveis++;
            }
        }

}

void Tabela_Exames::atualiza_lista_por_exame_desalocado(int *exames_nao_alocados,int exame_desalocado){
    int i,j,k ,l;
    for(i=0; i<max_exames;i++){
        if(exames_nao_alocados[i] > 0){
            if(conflitos[i][exame_desalocado] > 0){
                exames[i].horariosdisponiveis[exames[exame_desalocado].periodo] = 1;
            }
            if(exames[i].coincidencia && exames[exame_desalocado].coincidencia){
                if(coincidencia[i].find(exame_desalocado) != coincidencia[i].end()){
                    for(j=0;j<max_horarios;j++){
                        if(j != exames[exame_desalocado].periodo){
                            exames[i].horariosdisponiveis[j] =1;
                        }
                    }
                }
            }
            if(exames[i].antes && exames[exame_desalocado].depois){
                if(antes[i].find(exame_desalocado) != antes[i].end()){
                    for(j=exames[exame_desalocado].periodo;j<max_horarios;j++){
                        exames[i].horariosdisponiveis[j] =1;

                    }
                }
            }
            if(exames[i].depois && exames[exame_desalocado].antes){
                if(depois[i].find(exame_desalocado) != depois[i].end()){
                    for(j=exames[exame_desalocado].periodo;j>=0;j--){
                        exames[i].horariosdisponiveis[j] =1;

                    }
                }
            }
            if(exames[i].exclusao && exames[exame_desalocado].exclusao){
                if(exclusao[i].find(exame_desalocado) != exclusao[i].end()){
                        exames[i].horariosdisponiveis[exames[exame_desalocado].periodo] =1;
                }
            }
            exames[i].num_disponiveis = 0;
            if(exames[i].coincidencia){
                for (set<int>::iterator it = coincidencia[i].begin();it != coincidencia[i].end();it++){
                    if(exames[*it].alocado){
                        atualiza_exame_especifico(i,*it);
                    }
                }
            }
            if(exames[i].antes){
                for (set<int>::iterator it = antes[i].begin();it != antes[i].end();it++){
                    if(exames[*it].alocado){
                        atualiza_exame_especifico(i,*it);
                    }
                }
            }
            if(exames[i].depois){
               for (set<int>::iterator it = depois[i].begin();it != depois[i].end();it++){
                    if(exames[*it].alocado){
                        atualiza_exame_especifico(i,*it);
                    }
                }
            }
            if(exames[i].exclusao){
               for (set<int>::iterator it = exclusao[i].begin();it != exclusao[i].end();it++){
                    if(exames[*it].alocado){
                        atualiza_exame_especifico(i,*it);
                    }
                }
            }
            for(j=0;j<max_horarios;j++){
                if(horarios[j].duracao < exames[i].duracao){
                    exames[i].horariosdisponiveis[j] = 0;
                }
                for(k=0;k<max_salas;k++){
                    if(exames[i].horariosdisponiveis[j] == 0){
                        break;
                    }
                    if(tabela_horarios[j][k].tam > 0){
                        int *lista_restricoes;
                        lista_restricoes = tabela_horarios[j][k].retornaElementos();
                        for(l=0; l<tabela_horarios[j][k].tam;l++){
                            atualiza_exame_especifico(i,lista_restricoes[l]);
                        }
                        free(lista_restricoes);
                    }
                }
            }
            exames[i].num_disponiveis = 0;
            for(j=0;j<max_horarios;j++){
                if(exames[i].horariosdisponiveis[j] == 1){
                    exames[i].num_disponiveis++;
                }
            }
        }
    }
}


void Tabela_Exames::atualiza_exame_especifico(int exame_atual, int exame_alocado){
    int cabe=0;
    int j;
        if(conflitos[exame_atual][exame_alocado] > 0){
            exames[exame_atual].horariosdisponiveis[exames[exame_alocado].periodo] = 0;
        }
        else{
            for(j=0;j<max_salas;j++){
                if(!exames[exame_atual].exclusivo){
                        if(espacos_restantes[exames[exame_alocado].periodo][j] >= exames[exame_atual].num_alunos){
                            cabe = 1;
                            break;
                        }
                    }
                    else{
                        if(tabela_horarios[exames[exame_alocado].periodo][j].tam == 0 && espacos_restantes[exames[exame_alocado].periodo][j] >= exames[exame_atual].num_alunos){
                            cabe = 1;
                            break;
                        }
                    }
            }
            if(cabe == 0){
                exames[exame_atual].horariosdisponiveis[exames[exame_alocado].periodo] = cabe;
            }
        }
        if(exames[exame_atual].antes && exames[exame_alocado].depois){
            if(antes[exame_atual].find(exame_alocado)!=antes[exame_atual].end()){
                for(j=exames[exame_alocado].periodo;j<max_horarios;j++){
                    exames[exame_atual].horariosdisponiveis[j] = 0;
                }
            }
        }
        else if(exames[exame_atual].depois && exames[exame_alocado].antes){
            if(depois[exame_atual].find(exame_alocado)!=depois[exame_atual].end()){
                for(j=exames[exame_alocado].periodo;j>=0;j--){
                    exames[exame_atual].horariosdisponiveis[j] = 0;
                }
            }
        }else if(exames[exame_atual].exclusao && exames[exame_alocado].exclusao){
            if(exclusao[exame_atual].find(exame_alocado)!=exclusao[exame_atual].end()){
                exames[exame_atual].horariosdisponiveis[exames[exame_alocado].periodo] = 0;
            }
        }else if(exames[exame_atual].coincidencia && exames[exame_alocado].coincidencia){
            if(coincidencia[exame_atual].find(exame_alocado)!=coincidencia[exame_atual].end()){
                for(j=0;j<max_horarios;j++){
                    if(j!= exames[exame_alocado].periodo){
                        exames[exame_atual].horariosdisponiveis[j] = 0;

                    }
                }
            }
        }
}

void Tabela_Exames::atualiza_lista_exames_nao_alocados(int* exames_nao_alocados,int exame_alocado){
    int i,j, cabe;
    for(i=0;i<max_exames;i++){
        cabe=0;
        if(exames_nao_alocados[i] == 1){
            if(conflitos[i][exame_alocado] > 0){
                    exames[i].horariosdisponiveis[exames[exame_alocado].periodo] = 0;
            }
            else{
                for(j=0;j<max_salas;j++){
                    if(!exames[i].exclusivo){
                        if(espacos_restantes[exames[exame_alocado].periodo][j] >= exames[i].num_alunos){
                            cabe = 1;
                            break;
                        }
                    }
                    else{
                        if(tabela_horarios[exames[exame_alocado].periodo][j].tam == 0 && espacos_restantes[exames[exame_alocado].periodo][j] >= exames[i].num_alunos){
                            cabe = 1;
                            break;
                        }
                    }
                }
                if(cabe == 0){
                    exames[i].horariosdisponiveis[exames[exame_alocado].periodo] = cabe;
                }
            }
            if(exames[i].antes && exames[exame_alocado].depois){
                if(antes[i].find(exame_alocado)!=antes[i].end()){
                    for(j=exames[exame_alocado].periodo;j<max_horarios;j++){
                        exames[i].horariosdisponiveis[j] = 0;
                    }
                }
            }else if(exames[i].depois && exames[exame_alocado].antes){
                if(depois[i].find(exame_alocado)!=depois[i].end()){
                    for(j=exames[exame_alocado].periodo;j>=0;j--){
                        exames[i].horariosdisponiveis[j] = 0;
                    }
                }
            }else if(exames[i].exclusao && exames[exame_alocado].exclusao){
                if(exclusao[i].find(exame_alocado)!=exclusao[i].end() ){
                    exames[i].horariosdisponiveis[exames[exame_alocado].periodo] = 0;
                }
            }else if(exames[i].coincidencia && exames[exame_alocado].coincidencia){
                if(coincidencia[i].find(exame_alocado)!=coincidencia[i].end()){
                    for(j=0;j<max_horarios;j++){
                        if(j!= exames[exame_alocado].periodo){
                            exames[i].horariosdisponiveis[j] = 0;
                        }
                    }
                }
            }
            exames[i].num_disponiveis = 0;
            for(j=0;j<max_horarios;j++){
                if(exames[i].horariosdisponiveis[j] == 1){
                    exames[i].num_disponiveis++;
                }
            }
        }
    }
}

