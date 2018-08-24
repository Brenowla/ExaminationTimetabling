#ifndef PERIODO_H
#define PERIODO_H


class Periodo
{
    int duracao;
    int penalidade;
    int alocados;
    char data[11];
    public:
        Periodo();
        virtual ~Periodo();
        friend class Tabela_Exames;
};

#endif // PERIODO_H
