#include "Arquivo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ler_dados(Tabela_Exames *tabela, char *nome_do_arquivo){
    //Variáveis
    FILE *dataset;
    Lista_alunos *lista_alunos;
    Lista_Exames *exam_aux;
    Exame *exam;
    char aux[20],aux2[20];
    lista_alunos = new Lista_alunos();
    int i,j,k,num,num2,num3,cont=0;
    char buffer[8000];
    //Função
    for (i=0; nome_do_arquivo[i]!='\0';i++){
        aux[i] = nome_do_arquivo[i];
    }
    aux[i]='\0';
    dataset = fopen(aux,"rt");
    fgets(buffer,2000,dataset);
    for(i=0; buffer[i] != ':';i++);
    i++;
    for(j=0; buffer[i] != ']';i++,j++){
        aux[j] = buffer[i];
    }
    aux[j] = '\0';
    num = atoi(aux);
    tabela->inicializa(num);
    for(k =0; k < num;k++){ //Ler exames e alunos
        cont = 0;
        fgets(buffer,8000,dataset);
        for(j=0,i=0; buffer[i] != ',';i++,j++){
            aux[j] = buffer[i];
        }
        aux[j] = '\0';
        num2 = atoi(aux);
        tabela->adicionar_Exame(k,num2);
        do{
            cont++;
            i+=2;
            for(j=0; buffer[i] != ',' && buffer[i] != '\0';i++,j++){
                aux[j] = buffer[i];
            }
            aux[j] = '\0';
            num2 = atoi(aux);
            exam = tabela->get_exame(k);
            exam_aux = lista_alunos->adicionar_aluno(num2,exam);
            if(exam_aux != NULL){
                tabela->atualiza_conflitos(k,exam_aux);
            }
        }while(buffer[i] != '\0');
        tabela->adicionar_alunosExame(k,cont);
    }
    fgets(buffer,2000,dataset);
    for(i=0; buffer[i] != ':';i++);
    i++;
    for(j=0; buffer[i] != ']';i++,j++){
        aux[j] = buffer[i];
    }
    aux[j] = '\0';
    num = atoi(aux);
    tabela->inicializa_periodos(num);
    for(k =0; k < num;k++){ //Ler os horários
        fgets(buffer,2000,dataset);
        for(j=0;buffer[j] != ',';j++){
            aux2[j] = buffer[j];
        }
        //cout << buffer << endl;
        aux2[j] = '\0';
        for(j=0,i=22; buffer[i] != ',';i++,j++){
            aux[j] = buffer[i];
        }
        aux[j] = '\0';
        num2 = atoi(aux);
        i+=2;
        for(j=0; buffer[i] != '\0';i++,j++){
            aux[j] = buffer[i];
        }
        aux[j] = '\0';
        num3 = atoi(aux);
        tabela->adicionar_Horarios(k,num2,num3,aux2);
    }
    fgets(buffer,2000,dataset);
    for(i=0; buffer[i] != ':';i++);
    i++;
    for(j=0; buffer[i] != ']';i++,j++){
        aux[j] = buffer[i];
    }
    aux[j] = '\0';
    num = atoi(aux);
    tabela->inicializa_salas(num);
    for(k =0; k < num;k++){
        fgets(buffer,2000,dataset);
        for(j=0,i=0; buffer[i] != ',';i++,j++){
            aux[j] = buffer[i];
        }
        aux[j] = '\0';
        num2 = atoi(aux);
        i+=2;
        for(j=0; buffer[i] != '\0';i++,j++){
            aux[j] = buffer[i];
        }
        aux[j] = '\0';
        num3 = atoi(aux);
        tabela->adicionar_Salas(k,num2,num3);
    }
    fgets(buffer,2000,dataset);
    while(true){
        fgets(buffer,2000,dataset);
        if(buffer[0] == '[') break;
        for(j=0,i=0; buffer[i] != ',';i++,j++){
            aux[j] = buffer[i];
        }
        aux[j] = '\0';
        num2 = atoi(aux);
        i+= 2;
        for(j=0; buffer[i] != ',';i++,j++){
            aux2[j] = buffer[i];
        }
        aux2[j] = '\0';
        i+= 2;
        for(j=0; buffer[i] != '\0';i++,j++){
            aux[j] = buffer[i];
        }
        aux[j] = '\0';
        num3 = atoi(aux);
        if(strcmp(aux2,"AFTER") ==0){
            tabela->adicionar_Depois(num2,num3);
        }
        else if(strcmp(aux2,"EXAM_COINCIDENCE") ==0){
            tabela->adicionar_Coincidencia(num2,num3);
        }
        else if(strcmp(aux2,"EXCLUSION") ==0){
            tabela->adicionar_Exclusao(num2,num3);
        }
    }
    while(true){
        fgets(buffer,2000,dataset);
        if(buffer[0] == '[') break;
        for(j=0,i=0; buffer[i] != ',';i++,j++){
            aux[j] = buffer[i];
        }
        aux[j] = '\0';
        num2 = atoi(aux);
        tabela->adicionar_exclusividade(num2);
    }
    fgets(buffer,2000,dataset);
    int num4,num5,num6,num7;
    for(j=0,i=11; buffer[i] != '\0';i++,j++){
        aux[j] = buffer[i];
    }
    aux[j] = '\0';
    num = atoi(aux);
    fgets(buffer,2000,dataset);
    for(j=0,i=11; buffer[i] != '\0';i++,j++){
        aux[j] = buffer[i];
    }
    aux[j] = '\0';
    num2 = atoi(aux);
    fgets(buffer,2000,dataset);
    for(j=0,i=14; buffer[i] != '\0';i++,j++){
        aux[j] = buffer[i];
    }
    aux[j] = '\0';
    num3 = atoi(aux);
    fgets(buffer,2000,dataset);
    for(j=0,i=18; buffer[i] != '\0';i++,j++){
        aux[j] = buffer[i];
    }
    aux[j] = '\0';
    num4 = atoi(aux);
    fgets(buffer,2000,dataset);
    for(j=0,i=10; buffer[i] != ',';i++,j++){
        aux[j] = buffer[i];
    }
    aux[j] = '\0';
    num5 = atoi(aux);
    i++;
    for(j=0; buffer[i] != ',';i++,j++){
        aux[j] = buffer[i];
    }
    aux[j] = '\0';
    num6 = atoi(aux);
    i++;
    for(j=0; buffer[i] != '\0';i++,j++){
        aux[j] = buffer[i];
    }
    aux[j] = '\0';
    num7 = atoi(aux);
    tabela->inicializa_pesos(num,num2,num3,num4,num5,num6,num7);
}
