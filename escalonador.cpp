#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Process {
    int chegada, duration, remainingTime;
    int startExecution = -1;
    int finishTime = -1;

    // Construtor explícito
    Process(int a, int d, int r) : chegada(a), duration(d), remainingTime(r) {}
};

void readInput(vector<Process>& processes, const string& filename) {
    ifstream file(filename);  // Abre o arquivo
    int chegada, duration;

    if (file.is_open()) {
        while (file >> chegada >> duration) {  // Lê os valores do arquivo
            processes.push_back(Process(chegada, duration, duration));
        }
        file.close();  // Fecha o arquivo após a leitura
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
    vector<int> returnTimes, responseTimes, waitingTimes;

    // for com iterator para criar uma váriável temporária para tratar os tempos de proecsso por processo.
    // desse modo não precisamos acessar por indices.
    for (const auto& process : processes) {
        if (currentTime < process.chegada)
            currentTime = process.chegada;

        int waitingTime = currentTime - process.chegada;
        int returnTime = waitingTime + process.duration;
        int responseTime = waitingTime;

        returnTimes.push_back(returnTime);
        responseTimes.push_back(responseTime);
        waitingTimes.push_back(waitingTime);

        currentTime += process.duration;
    }

    std::cout << "FCFS " << fixed << setprecision(1)
              << calculateAverage(returnTimes) << " "
              << calculateAverage(responseTimes) << " "
              << calculateAverage(waitingTimes) << "\n";
}

void sjf(vector<Process> processes) {
    int currentTime = 0;
    vector<int> returnTimes, responseTimes, waitingTimes;
    vector<Process> ready;

    while (!processes.empty() || !ready.empty()) {
        for (int i = 0; i < processes.size(); ++i) {
        if (processes[i].chegada <= currentTime) {
            // Adiciona o processo à lista 'ready'
            ready.push_back(processes[i]);

            // Remove o processo da lista 'processes'
            processes.erase(processes.begin() + i);

            // Como o vetor foi reduzido de tamanho, ajustamos o índice
            --i;  // Decrementa o índice para evitar pular o próximo elemento
        }
        }

        if (!ready.empty()) {
            sort(ready.begin(), ready.end(), [](const Process& a, const Process& b) {
                return a.duration < b.duration;
            });

            auto process = ready.front();
            ready.erase(ready.begin());

            if (currentTime < process.chegada)
                currentTime = process.chegada;

            int waitingTime = currentTime - process.chegada;
            int returnTime = waitingTime + process.duration;
            int responseTime = waitingTime;

            returnTimes.push_back(returnTime);
            responseTimes.push_back(responseTime);
            waitingTimes.push_back(waitingTime);

            currentTime += process.duration;
        } else {
            currentTime++;
        }
    }

    std::cout << "SJF " << fixed << setprecision(1)
              << calculateAverage(returnTimes) << " "
              << calculateAverage(responseTimes) << " "
              << calculateAverage(waitingTimes) << "\n";
}

void roundRobin(vector<Process> processes, int quantum) {
    int currentTime = 0;
    queue<Process*> queue;
    vector<int> returnTimes, responseTimes, waitingTimes;

    size_t index = 0;
    while (index < processes.size() || !queue.empty()) {
        while (index < processes.size() && processes[index].chegada <= currentTime) {
            queue.push(&processes[index]);
            index++;
        }

        if (!queue.empty()) {
            Process* process = queue.front();
            queue.pop();

            if (process->startExecution == -1) {
                process->startExecution = currentTime;
            }

            int executionTime = min(quantum, process->remainingTime);
            process->remainingTime -= executionTime;
            currentTime += executionTime;

            if (process->remainingTime > 0) {
                queue.push(process);
            } else {
                process->finishTime = currentTime;
                int waitingTime = process->finishTime - process->chegada - process->duration;
                int returnTime = process->finishTime - process->chegada;
                int responseTime = process->startExecution - process->chegada;

                returnTimes.push_back(returnTime);
                responseTimes.push_back(responseTime);
                waitingTimes.push_back(waitingTime);
            }
        } else {
            currentTime++;
        }
    }

    std::cout << "RR " << fixed << setprecision(1)
              << calculateAverage(returnTimes) << " "
              << calculateAverage(responseTimes) << " "
              << calculateAverage(waitingTimes) << "\n";
}

void printProcesses(const vector<Process>& processes) {
    for (const auto& process : processes) {
        std::cout << "Arrival: " << process.chegada 
                  << ", Duration: " << process.duration 
                  << ", Remaining: " << process.remainingTime << "\n";
    }
}

int main() {
    vector<Process> processes;
    string fileName = "caso2.txt";
    readInput(processes, fileName);

    printProcesses(processes);


    fcfs(processes);
    sjf(processes);
    roundRobin(processes, 2);

    return 0;
}
