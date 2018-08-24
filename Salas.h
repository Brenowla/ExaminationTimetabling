#ifndef SALAS_H
#define SALAS_H


class Salas
{
    int capacidade;
    int penalidade;
    int alocados;
    public:
        Salas();
        virtual ~Salas();
        friend class Tabela_Exames;

        int getCapacidade() const { return capacidade; }
        int getPenalidade() const { return penalidade; }
};

#endif // SALAS_H
