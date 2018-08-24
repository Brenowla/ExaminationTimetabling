#include "Exame.h"

Exame::Exame()
{
    duracao = 0;
    periodo = 0;
    sala = 0;
    coincidencia = false;
    depois = false;
    antes = false;
    exclusao =false;
    alocado = false;
    exclusivo = false;
    e_grande = false;
    recentemente_retirado = false;
}

void Exame::inicializar_horarios_disponiveis(int numHorarios){
    horariosdisponiveis = (int*)malloc(numHorarios*sizeof(int*));
    int i;
    for (i=0;i<numHorarios;i++){
        horariosdisponiveis[i] = 1;
    }
}

void Exame::clone(Exame exame){
    num = exame.num;
    duracao = exame.duracao;
    periodo = exame.periodo;
    sala = exame.sala;
    num_alunos = exame.num_alunos;
    num_disponiveis = exame.num_disponiveis;
    e_grande = exame.e_grande;
    depois = exame.depois;
    antes = exame.antes;
    coincidencia = exame.coincidencia;
    exclusao = exame.exclusao;
    alocado = exame.alocado;
    exclusivo = exame.exclusivo;
}

Exame::~Exame()
{

}
