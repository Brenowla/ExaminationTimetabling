#include "Tabela_Exames.h"

Tabela_Exames::Tabela_Exames()
{
    //ctor
}

void Tabela_Exames::inicializa(int num_exames){
    int i,j;
    penalidade_total = 0;
    max_exames = num_exames;
    conflitos = (int**) malloc(num_exames*sizeof(int**));
    conflitos_totais = 0;
    exclusao = new set<int>[num_exames];
    depois = new set<int>[num_exames];
    coincidencia = new set<int>[num_exames];
    antes = new set<int>[num_exames];
    for(i = 0; i<num_exames;i++){
        conflitos[i] = (int*) malloc(num_exames*sizeof(int*));
    }
    for(i = 0; i<num_exames;i++){
        for(j = 0; j<num_exames;j++){
            conflitos[i][j] = 0;
        }
    }
    exames = new Exame[num_exames]();
}

void Tabela_Exames::inicializa_periodos(int num_periodos){
    horarios = new Periodo[num_periodos]();
    max_horarios = num_periodos;
    int i;
    for(i=0;i<max_exames;i++){
        exames[i].inicializar_horarios_disponiveis(num_periodos);
        exames[i].num_disponiveis = max_horarios;
    }
}

void Tabela_Exames::inicializa_salas(int num_salas){
    salas = new Salas[num_salas]();
    max_salas = num_salas;
    somaPenalidadeSalas = 0;
}

void Tabela_Exames::inicializa_pesos(int seq, int dia, int per, int mix, int g, int maxi, int penm ){
    pesos.sequencia = seq;
    pesos.mesmo_dia =  dia;
    pesos.dist_periodos = per;
    pesos.mist_horarios = mix;
    pesos.grandes = g;
    int i,j,k;
    int exames_grandes[pesos.grandes];
    int tamanhos[pesos.grandes];
    exames_grandes[0] = 0;
    tamanhos[0] = exames[0].num_alunos;
    for(i=1;i<pesos.grandes;i++){
        for(j=0;j<i;j++){
            if(exames[i].num_alunos < tamanhos[j]){
                break;
            }
        }
        for(k = i; k>j; k--){
            tamanhos[k] = tamanhos[k-1];
            exames_grandes[k] = exames_grandes[k-1];
        }
        tamanhos[j] = exames[i].num_alunos;
        exames_grandes[j] = i;
    }
    for(i=i; i<max_exames;i++){
        for(j=0;j<pesos.grandes-1;j++){
            if(exames[i].num_alunos > tamanhos[j] && exames[i].num_alunos <= tamanhos[j+1]){
                break;
            }
        }
        if(exames[i].num_alunos < tamanhos[j]){
            j++;
        }
        if(j < pesos.grandes){
            for(k=0;k<j;k++){
                tamanhos[k] = tamanhos[k+1];
                exames_grandes[k] = exames_grandes[k+1];
            }
            tamanhos[j] = exames[i].num_alunos;
            exames_grandes[j] = i;
        }
    }
    for(i=0;i<pesos.grandes;i++){
        exames[exames_grandes[i]].e_grande = true;
    }
    pesos.primeiros = maxi;
    pesos.pen_grandes = penm;
}

void Tabela_Exames::inicializa_tabela_horarios(){
    int i,j;
    tabela_horarios = new Lista_Exames*[max_horarios];
    espacos_restantes = (int**) malloc (max_horarios*sizeof(int**));
    for(i = 0; i<max_horarios;i++){
        tabela_horarios[i] = new Lista_Exames[max_salas];
        espacos_restantes[i] =  (int*) malloc (max_salas*sizeof(int*));
    }
    for(i = 0; i<max_horarios;i++){
        for(j=0 ; j< max_salas; j++){
            espacos_restantes[i][j] = salas[j].capacidade;
        }
    }
}

void Tabela_Exames::adicionar_Exame(int num, int duracao){
    exames[num].duracao = duracao;
    exames[num].num = num;
}

void Tabela_Exames::adicionar_Horarios(int num, int dur, int pen, char *data){
    horarios[num].duracao = dur;
    horarios[num].penalidade = pen;
    strcpy(horarios[num].data,data);
}

void Tabela_Exames::adicionar_Salas(int num, int cap, int pen){
    salas[num].capacidade = cap;
    salas[num].penalidade = pen;
    somaPenalidadeSalas += pen;
}

void Tabela_Exames::atualiza_conflitos(int atual, Lista_Exames* exam_con){
    Lista_Exames *aux;
    aux = exam_con;
    while(aux != NULL){
        if(atual != aux->exame->getNum()){
            conflitos[atual][aux->exame->getNum()] ++;
            conflitos[aux->exame->getNum()][atual] ++;
            if(conflitos[atual][aux->exame->getNum()] == 1){
                conflitos_totais++;
            }
        }
        aux = aux->prox;
    }
}

void Tabela_Exames::reseta_alocados(){
    int i;
    for(i=0;i< max_horarios;i++){
        horarios[i].alocados =0;
    }
    for(i=0;i< max_salas;i++){
        salas[i].alocados =0;
    }
}

void Tabela_Exames::adicionar_Exclusao(int exam1, int exam2){
    exclusao[exam1].insert(exam2);
    exclusao[exam2].insert(exam1);
    exames[exam1].exclusao = true;
    exames[exam2].exclusao = true;
}

void Tabela_Exames::adicionar_Coincidencia(int exam1,int exam2){
    coincidencia[exam1].insert(exam2);
    coincidencia[exam2].insert(exam1);
    exames[exam1].coincidencia = true;
    exames[exam2].coincidencia = true;
}

void Tabela_Exames::adicionar_Depois(int exam1,int exam2){
    depois[exam1].insert(exam2);
    antes [exam2].insert(exam1);
    exames[exam1].depois = true;
    exames[exam2].antes = true;
}

void Tabela_Exames::adicionar_exclusividade(int exam){
    exames[exam].exclusivo = true;
}

void Tabela_Exames::adicionar_alunosExame(int exam,int num_alunos){
    exames[exam].num_alunos = num_alunos;
}


void Tabela_Exames::remove_exame(int exame_desalocado, int *exames_nao_alocados){
    tabela_horarios[exames[exame_desalocado].periodo][exames[exame_desalocado].sala].remover(exame_desalocado);
    espacos_restantes[exames[exame_desalocado].periodo][exames[exame_desalocado].sala] = salas[exames[exame_desalocado].sala].capacidade - tabela_horarios[exames[exame_desalocado].periodo][exames[exame_desalocado].sala].num_alunos;
    exames[exame_desalocado].alocado = false;
    atualiza_lista_por_exame_desalocado(exames_nao_alocados, exame_desalocado);
    exames_nao_alocados[exame_desalocado] = 1;
}

int Tabela_Exames::calcula_Peso(){
    int peso =0;
    int i ,j;
    set<int> hor_dif[max_horarios][max_salas];
    reseta_alocados();
    for(i=0;i<max_exames;i++){
        horarios[exames[i].periodo].alocados++;
        salas[exames[i].sala].alocados++;
        for(j=i+1;j<max_exames;j++){
            if(conflitos[i][j] > 0){
                if(strcmp(horarios[exames[i].periodo].data,horarios[exames[j].periodo].data) == 0){
                    if((exames[i].periodo - exames[j].periodo == 1 || exames[i].periodo - exames[j].periodo == -1)){
                        peso += (pesos.sequencia * conflitos[i][j]);
                    }
                    else {
                        peso += pesos.mesmo_dia * conflitos[i][j];
                    }
                }
                if(exames[i].periodo - exames[j].periodo <= pesos.dist_periodos && exames[i].periodo - exames[j].periodo >= -1*pesos.dist_periodos){
                    peso += conflitos[i][j];
                }
            }
        }
        hor_dif[exames[i].periodo][exames[i].sala].insert(exames[i].duracao);
        if(exames[i].e_grande && exames[i].periodo>=max_horarios-pesos.primeiros){
            peso += pesos.pen_grandes;
        }

    }
    this->p = 0;
    for(i=0;i<max_horarios;i++){
        peso += horarios[i].alocados * horarios[i].penalidade;
        for(j=0;j<max_salas;j++){
            if(hor_dif[i][j].size() > 0){
                peso += (hor_dif[i][j].size()-1) * pesos.mist_horarios;
                p+=(hor_dif[i][j].size()-1) * pesos.mist_horarios;
            }
        }
    }
    for(i=0;i<max_salas;i++){
        peso += salas[i].penalidade * salas[i].alocados;
        p+=salas[i].penalidade * salas[i].alocados;
    }
    return peso;
}



bool Tabela_Exames::conferir_solucao(){
    int i ,j;
    set<int> hor_dif[max_horarios][max_salas];
    for(i=0;i<max_exames;i++){
        horarios[exames[i].periodo].alocados++;
        salas[exames[i].sala].alocados++;
        for(j=i+1;j<max_exames;j++){
            if(conflitos[i][j] > 0){
                if(exames[i].periodo == exames[j].periodo){
                    cout << "Conflito" << endl;
                    return false;
                }
            }
            if(exames[i].exclusivo){
                if(exames[i].periodo == exames[j].periodo && exames[i].sala == exames[j].sala){
                    cout << "Exclusividade" << endl;
                    return false;
                }
            }
        }
        if(exames[i].duracao > horarios[exames[i].periodo].duracao){
            cout << "Duracao" << endl;
            return false;
        }
        if(tabela_horarios[exames[i].periodo][exames[i].sala].num_alunos_real > salas[exames[i].sala].capacidade){
            cout << "Capacidade" << endl;
            return false;
        }
        if (exames[i].antes){
            for(set<int>::iterator it = antes[i].begin(); it!=antes[i].end();it++){
                if(exames[i].periodo >= exames[*it].periodo){
                    cout << "Antes" << endl;
                    return false;
                }
            }
        }
        if (exames[i].depois){
            for(set<int>::iterator it = depois[i].begin(); it != depois[i].end();it++){
                if(exames[i].periodo <= exames[*it].periodo){
                    cout << "Depois" << endl;
                    return false;
                }
            }
        }
        if(exames[i].coincidencia){
            for(set<int>::iterator it = coincidencia[i].begin(); it != coincidencia[i].end();it++){
                if(exames[i].periodo != exames[*it].periodo){
                    cout << "Coincidencia" << endl;
                    return false;
                }
            }
        }
        if(exames[i].exclusao){
            for(set<int>::iterator it = exclusao[i].begin(); it != exclusao[i].end();it++){
                if(exames[i].periodo == exames[*it].periodo){
                    cout << "Exclusao" << endl;
                    return false;
                }
            }
        }
    }
    return true;
}

void Tabela_Exames::imprime_conflitos(){
    cout << endl;
    int i,j;
    for(i=0; i< max_exames;i++){
        for(j=0; j< max_exames;j++){
            cout << conflitos [i][j] << " | ";
        }
        cout << endl;
    }
}

void Tabela_Exames::imprime_periodos(){
    cout << endl << "Horarios" << endl;
    int i;
    for(i=0; i< max_horarios;i++){
        cout << horarios[i].duracao << " | " << horarios[i].penalidade << endl;
    }
}

void Tabela_Exames::imprime_salas(){
    cout << endl << "Salas: " << max_salas << endl;
    int i;
    for(i=0; i< max_salas;i++){
        cout << salas[i].capacidade << " | " << salas[i].penalidade << endl;
    }
}

void Tabela_Exames::imprime_pesos(){
    cout << pesos.sequencia << " | " << pesos.mesmo_dia << " | " << pesos.dist_periodos << " | " << pesos.mist_horarios << " | " << pesos.grandes << " | " << pesos.primeiros << " | " << pesos.pen_grandes << endl;
}

void Tabela_Exames::imprime_restricoes(){
    cout << "Chegou aqui" << endl;
    cout << endl;
    int i;
    for(i=0; i< max_exames;i++){
        cout << "Exclusoes " << i << " : "  << endl;
        //exclusao [i].imprime();
        cout << endl;
    }
     for(i=0; i< max_exames;i++){
        cout << "Depois " << i << " : "  << endl;
        //depois [i].imprime();
        cout << endl;
    }
     for(i=0; i< max_exames;i++){
        cout << "Antes " << i << " : "  << endl;
        //antes [i].imprime();
        cout << endl;
    }
     for(i=0; i< max_exames;i++){
        cout << "Coincidencia " << i << " : "  << endl;
        //coincidencia [i].imprime();
        cout << endl;
    }
}


void Tabela_Exames::imprime_tabelahorarios(){
    int i,j;
    for (i=0;i<max_horarios;i++){
        for(j=0;j<max_salas;j++){
            cout << "|";
            tabela_horarios[i][j].imprime();
        }
        cout << endl;
    }
    cout << endl << endl;
}

void Tabela_Exames::imprime_disponibilidade(){
    int i,j;
    for(i =0;i<max_exames;i++){
        cout << "Horarios do exame: " << i << "Disponiveis " << exames[i].num_disponiveis << endl;
        for(j=0;j<max_horarios;j++){
            cout << exames[i].horariosdisponiveis[j] << " | ";
        }
        cout << endl << endl;
    }
}

Exame* Tabela_Exames::get_exame(int exam){
    return &exames[exam];
}

int Tabela_Exames::getMax_Exames(){
    return max_exames;
}

int Tabela_Exames::get_melhor_peso(){
    return melhor_peso;
}

void Tabela_Exames::atualiza_disponibilidade(){
    int i,j;
    for(i =0;i<max_exames;i++){
        for(j=0; j<max_horarios;j++){
            exames[i].horariosdisponiveis[j] =1;
            exames[i].num_disponiveis ++;
        }
    }
    for(i =0;i<max_exames;i++){//atualiza a lista de exames disponiveis para cada
        if(exames[i].antes){
            exames[i].horariosdisponiveis[max_horarios-1] =0;
            exames[i].num_disponiveis --;
        }
        if(exames[i].depois){
            exames[i].horariosdisponiveis[0] = 0;
            exames[i].num_disponiveis --;
        }
        for(j=0; j<max_horarios;j++){
            if(exames[i].duracao > horarios[j].duracao && exames[i].horariosdisponiveis[j] == 1){
                exames[i].horariosdisponiveis[j] =0;
                exames[i].num_disponiveis --;
            }
        }
    }
}

void Tabela_Exames::saida(){
    FILE *Saida;
    Saida = fopen("Saida.sln","w");
    for(int i = 0; i < max_exames;i++){
        fprintf(Saida,"%d, %d\r\n",exames[i].periodo,exames[i].sala);
    }
}

Tabela_Exames::~Tabela_Exames()
{
    int i;
    for(i = 0; i<max_exames;i++){
        delete[] conflitos[i];
    }
    for(i = 0; i<max_horarios;i++){
        delete[] espacos_restantes[i];
    }
    delete [] conflitos;
    delete [] espacos_restantes;
}

void Tabela_Exames::setSalaExame(const int e, const int s) {
    this->exames[e].sala = s;
}