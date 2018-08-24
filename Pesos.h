#ifndef PESOS_H
#define PESOS_H


class Pesos
{
    int sequencia;
    int mesmo_dia;
    int dist_periodos;
    int mist_horarios;
    int grandes;
    int primeiros;
    int pen_grandes;
    public:
        Pesos();
        virtual ~Pesos();
        friend class Tabela_Exames;
};

#endif // PESOS_H
