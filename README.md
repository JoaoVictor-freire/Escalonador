# Escalonador
--------------------------------------------------------------
Trabalho 1 de Sistemas Operacionais - FCFS | SJF | RR
Professor: Fernando Menezes Matos
Alunos: João Victor Freir e Matheus de Mendonça

Descrição:
Este programa implementa três algoritmos de escalonamento de processos:
FCFS (First Come First Serve)
SJF (Shortest Job First)
RR (Round Robin com quantum ajustável)
O programa lê a lista de processos, cada um com seu tempo de chegada e duração,
e calcula as médias de tempo de retorno, tempo de resposta e tempo de espera
para cada algoritmo de escalonamento.

Entrada:
Lista de processos, onde cada linha contém o tempo de chegada e a duração de cada processo.

Saída:
FCFS: Média de tempo de retorno, resposta e espera.
SJF: Média de tempo de retorno, resposta e espera.
RR: Média de tempo de retorno, resposta e espera (com quantum ajustável).

Compilação:
Win: g++ -o escalonador .\escalonador.cpp
Mac: g++ -std=c++11 escalonador.cpp -o escalonador
 
Execução:
Win: .\escalonador.exe 
Mac: ./escalonador
--------------------------------------------------------------
