#ifndef EXAME_H
#define EXAME_H
#include <stdlib.h>

class Exame
{
    int num;
    int duracao;
    int periodo;
    int sala;
    int num_alunos;
    int num_disponiveis;
    int *horariosdisponiveis;
    bool e_grande,depois,antes,coincidencia,exclusao,alocado,recentemente_retirado,exclusivo;
    public:
        Exame();
        virtual ~Exame();
        friend class Tabela_Exames;
        void inicializar_horarios_disponiveis(int);
        void clone(Exame);
        friend class Lista_Exames;

        //Samuel
        const int getNum() const { return num; }
        const int getDuracao() const { return duracao; }
        const int getNumAlunos() const { return num_alunos; }
        const bool salaExclusiva() const { return exclusivo; }
        const int getPeriodo() const { return periodo; }
        const int getSala() const { return sala; }
};

#endif // EXAME_H
