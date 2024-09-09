/*
 * --------------------------------------------------------------
 * Trabalho 1 de Sistemas Operacionais - FCFS | SJF | RR
 * Professor: Fernando Menezes Matos
 * Alunos: João Victor Freir e Matheus de Mendonça
 *
 * Compilação:
 * Win: g++ -o escalonador .\escalonador.cpp
 * Mac: g++ -std=c++11 escalonador.cpp -o escalonador
 * 
 * Execução:
 * Win: .\escalonador.exe 
 * --------------------------------------------------------------
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Process {
    int chegada, duration, t_restante;
    int startExecution = -1;
    int finishTime = -1;

    // Construtor explícito
    Process(int a, int d, int r) : chegada(a), duration(d), t_restante(r) {}
};

void readInput(vector<Process>& processes, const string& filename) {
    ifstream file(filename);  // Abre o arquivo
    int chegada, duration;

    if (file.is_open()) {
        while (file >> chegada >> duration) {  // Lê os valores do arquivo
            processes.push_back(Process(chegada, duration, duration));
        }
        file.close();  // Fecha o arquivo após a leitura

        // Ordena os processos pelo tempo de chegada
        sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
            return a.chegada < b.chegada;
        });
    } else {
        cerr << "Não foi possível abrir o arquivo!" << endl;
    }
}

double calculateAverage(vector<int>& values) {
    if (values.empty()) {
        return 0.0;  // Retorna 0 se a lista estiver vazia
    }

    double sum = 0;
    for (int v : values) sum += v;
    return sum / values.size();
}

void fcfs(const vector<Process>& processes) {
    int currentTime = 0;
    vector<int> t_retorno, t_resposta, t_espera;

    // for com iterator para criar uma váriável temporária para tratar os tempos de proecsso por processo.
    // desse modo não precisamos acessar por indices.
    for (const auto& process : processes) {
        if (currentTime < process.chegada)
            currentTime = process.chegada;

        int waitingTime = currentTime - process.chegada;
        int returnTime = waitingTime + process.duration;
        int responseTime = waitingTime;

        t_retorno.push_back(returnTime);
        t_resposta.push_back(responseTime);
        t_espera.push_back(waitingTime);

        currentTime += process.duration;
    }

    //calculando as médias
    std::cout << "FCFS " << fixed << setprecision(1)
              << calculateAverage(t_retorno) << " "
              << calculateAverage(t_resposta) << " "
              << calculateAverage(t_espera) << "\n";
}

void sjf(vector<Process> processes) {
    int currentTime = 0;
    vector<int> t_retorno, t_reposta, t_espera;
    vector<Process> ready;

    while (!processes.empty() || !ready.empty()) {

        for (int i = 0; i < processes.size(); ++i) {
            if (processes[i].chegada <= currentTime) {

                // se o processo está no seu tempo de entrar na fila de pronto ele é adicionado a ela e tirado da fila de processos.
                ready.push_back(processes[i]);
                processes.erase(processes.begin() + i);

                // como o nº de processos foi diminuido decrementamos 'i' para não pular processos.
                --i;
            }
        }

        if (!ready.empty()) {
            //função da biblioteca algorithm que está ordenando o meu vetor do inicio ao fim em uma ordem crescente
            //levando em consideração a função passada no terceiro argumento.
            sort(ready.begin(), ready.end(), [](const Process& a, const Process& b) {return a.duration < b.duration;} );

            auto process = ready.front();
            ready.erase(ready.begin());

            if (currentTime < process.chegada)
                currentTime = process.chegada;

            int waitingTime = currentTime - process.chegada;
            int returnTime = waitingTime + process.duration;
            int responseTime = waitingTime;

            t_retorno.push_back(returnTime);
            t_reposta.push_back(responseTime);
            t_espera.push_back(waitingTime);

            currentTime += process.duration;
        } else {
            currentTime++;
        }
    }

    //calculando as médias
    std::cout << "SJF " << fixed << setprecision(1)
              << calculateAverage(t_retorno) << " "
              << calculateAverage(t_reposta) << " "
              << calculateAverage(t_espera) << "\n";
}

void roundRobin(vector<Process> processes, int quantum) {
    int currentTime = 0;
    queue<Process*> fila; //processor organizados em forma de fila visto que temos um comportamento FCFS
    vector<int> t_retorno, t_resposta, t_espera;

    size_t index = 0;
    while (index < processes.size() || !fila.empty()) {

        //passando os processos para a fila conforme vão chegando
        while (index < processes.size() && processes[index].chegada <= currentTime) {
            fila.push(&processes[index]);
            index++;
        }

        if (!fila.empty()) {
            Process* process = fila.front();
            fila.pop();

            if (process->startExecution == -1) {
                process->startExecution = currentTime;
            }

            int executionTime = min(quantum, process->t_restante); // retorna o menor entre os 2 parametros
            process->t_restante -= executionTime; // diminui quantum do tempo de execução do processo 
            currentTime += executionTime; 

            if (process->t_restante > 0) {
                fila.push(process); // se ainda tiver tempo de execução vai pro final da fila.
            } else {
                // acabou e calcula todos os tempo solicitados.
                process->finishTime = currentTime;
                int waitingTime = process->finishTime - process->chegada - process->duration;
                int returnTime = process->finishTime - process->chegada;
                int responseTime = process->startExecution - process->chegada;

                t_retorno.push_back(returnTime);
                t_resposta.push_back(responseTime);
                t_espera.push_back(waitingTime);
            }
        } else {
            currentTime++;
        }
    }

    //calculando as médias
    std::cout << "RR " << fixed << setprecision(1)
              << calculateAverage(t_retorno) << " "
              << calculateAverage(t_resposta) << " "
              << calculateAverage(t_espera) << "\n";
}

int main() {
    vector<Process> processes;
    string fileName = "caso2.txt";
    readInput(processes, fileName);

    fcfs(processes);
    sjf(processes);
    roundRobin(processes, 2);

    return 0;
}
