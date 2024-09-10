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
 * Mac: ./escalonador
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
    int timeIn, timeDuration, timeRemaning;
    int startExecution = -1;
    int finishTime = -1;

    // Construtor explícito
    Process(int a, int d, int r) : timeIn(a), timeDuration(d), timeRemaning(r) {}
};

void readInput(vector<Process>& processes, const string& filename) {
    ifstream file(filename);  // Abre o arquivo
    int t_in, duration;

    if (file.is_open()) {
        while (file >> t_in >> duration) {  // Lê os valores do arquivo
            processes.push_back(Process(t_in, duration, duration));
        }
        file.close();  // Fecha o arquivo após a leitura

        // Ordena os processos pelo tempo de chegada
        sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {return a.timeIn < b.timeIn;} );

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
    vector<int> t_return, t_response, t_wait;

    // for com iterator para criar uma váriável temporária para tratar os tempos de proecsso por processo.
    // desse modo não precisamos acessar por indices.
    for (const auto& process : processes) {
        if (currentTime < process.timeIn)
            currentTime = process.timeIn;

        int waitingTime = currentTime - process.timeIn;
        int returnTime = waitingTime + process.timeDuration;
        int responseTime = waitingTime;

        t_return.push_back(returnTime);
        t_response.push_back(responseTime);
        t_wait.push_back(waitingTime);

        currentTime += process.timeDuration;
    }

    //calculando as médias
    std::cout << "FCFS " << fixed << setprecision(1)
              << calculateAverage(t_return) << " "
              << calculateAverage(t_response) << " "
              << calculateAverage(t_wait) << "\n";
}

void sjf(vector<Process> processes) {
    int currentTime = 0;
    vector<int> t_return, t_reponse, t_wait;
    vector<Process> ready;

    while (!processes.empty() || !ready.empty()) {

        for (int i = 0; i < processes.size(); ++i) {
            if (processes[i].timeIn <= currentTime) {

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
            sort(ready.begin(), ready.end(), [](const Process& a, const Process& b) {return a.timeDuration < b.timeDuration;} );

            auto process = ready.front();
            ready.erase(ready.begin());

            if (currentTime < process.timeIn)
                currentTime = process.timeIn;

            int waitingTime = currentTime - process.timeIn;
            int returnTime = waitingTime + process.timeDuration;
            int responseTime = waitingTime;

            t_return.push_back(returnTime);
            t_reponse.push_back(responseTime);
            t_wait.push_back(waitingTime);

            currentTime += process.timeDuration;
        } else {
            currentTime++;
        }
    }

    //calculando as médias
    std::cout << "SJF " << fixed << setprecision(1)
              << calculateAverage(t_return) << " "
              << calculateAverage(t_reponse) << " "
              << calculateAverage(t_wait) << "\n";
}

void roundRobin(vector<Process> processes, int quantum) {
    int currentTime = 0;
    queue<Process*> fila; //processor organizados em forma de fila visto que temos um comportamento FCFS
    vector<int> t_return, t_response, t_wait;

    size_t index = 0;
    while (index < processes.size() || !fila.empty()) {

        //passando os processos para a fila conforme vão chegando
        while (index < processes.size() && processes[index].timeIn <= currentTime) {
            fila.push(&processes[index]);
            index++;
        }

        if (!fila.empty()) {
            Process* process = fila.front();
            fila.pop();

            if (process->startExecution == -1) {
                process->startExecution = currentTime;
            }
            
            int executionTime = min(quantum, process->timeRemaning); // retorna o menor entre os 2 parametros
            process->timeRemaning -= executionTime; // diminui quantum do tempo de execução do processo 
            currentTime += executionTime;
 
            if (process->timeRemaning > 0) {
                //checa novamente se há processos a entrar na fila
                while (index < processes.size() && processes[index].timeIn <= currentTime) {
                    fila.push(&processes[index]);
                    index++;
                }

                fila.push(process); // se ainda tiver tempo de execução vai pro final da fila.
            } else {
                // acabou e calcula todos os tempo solicitados.
                process->finishTime = currentTime;
                int waitingTime = process->finishTime - process->timeIn - process->timeDuration;
                int returnTime = process->finishTime - process->timeIn;
                int responseTime = process->startExecution -process->timeIn;

                t_return.push_back(returnTime);
                t_response.push_back(responseTime);
                t_wait.push_back(waitingTime);
            }
        } else {
            currentTime++;
        }
    }

    //calculando as médias
    std::cout << "RR " << fixed << setprecision(1)
              << calculateAverage(t_return) << " "
              << calculateAverage(t_response) << " "
              << calculateAverage(t_wait) << "\n";
}

int main() {
    vector<Process> processes;
    string fileName = "caso1.txt";
    readInput(processes, fileName);

    fcfs(processes);
    sjf(processes);
    roundRobin(processes, 2);

    return 0;
}
