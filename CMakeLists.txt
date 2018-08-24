cmake_minimum_required(VERSION 3.10)
project(ExaminationTimetabling)

set(CMAKE_CXX_STANDARD 14)

set(LP_COMPILE_FLAGS "-DCPX -I/opt/ibm/ILOG/CPLEX_Studio127/cplex/include/ilcplex/")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LP_COMPILE_FLAGS} -fsanitize=address")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LP_COMPILE_FLAGS} -Ofast")

set(LP_LINK_FLAGS "-L/opt/ibm/ILOG/CPLEX_Studio127/cplex/lib/x86-64_linux/static_pic/ -lcplex -lilocplex -lpthread")

add_executable(ExaminationTimetabling main.cpp Arquivo.h Arquivo.cpp Busca_Solucoes.cpp Exame.h Exame.cpp
        Gerador_Horarios.cpp Lista_alunos.h Lista_alunos.cpp Lista_Exames.h Lista_Exames.cpp
        Periodo.h Periodo.cpp Pesos.h Pesos.cpp Salas.h Salas.cpp Tabela_Exames.h Tabela_Exames.cpp
        AlocaSalas.h AlocaSalas.cpp StrUtil.hpp StrUtil.cpp lp.h lp.cpp)

target_link_libraries(ExaminationTimetabling ${LP_LINK_FLAGS})
