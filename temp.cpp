#include <iostream>
#include <vector>
#include <algorithm>

// Structure to represent a process
struct Process {
    int id;
    int arrivalTime;
    int burstTime;
};

// Function to execute SJF scheduling
void sjf(std::vector<Process>& processes) {
    int n = processes.size();

    // Sort the processes by arrival time and burst time
    std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        if (a.burstTime == b.burstTime)
            return a.arrivalTime < b.arrivalTime;
        return a.burstTime < b.burstTime;
    });

    int currentTime = 0;
    double totalWaitingTime = 0;

    std::cout << "Order of execution: ";
    for (const Process& p : processes) {
        if (currentTime < p.arrivalTime) {
            currentTime = p.arrivalTime;
        }

        std::cout << "P" << p.id << " ";

        // Calculate waiting time for the current process
        int waitingTime = currentTime - p.arrivalTime;
        totalWaitingTime += waitingTime;

        // Update the current time
        currentTime += p.burstTime;
    }

    double averageWaitingTime = totalWaitingTime / n;
    std::cout << "\nAverage Waiting Time: " << averageWaitingTime << std::endl;
}

int main() {
    // Define the processes in the requested format
    std::vector<Process> processes = {
        {0, 5, 27},
        {1, 3, 31},
        {2, 4, 18},
        {3, 6, 22},
        {4, 4, 26},
        {5, 5, 43},
        {6, 12, 37},
        {7, 9, 76},
        {8, 4, 41},
        {9, 9, 31},
        {10, 8, 33},
        {11, 12, 41},
        {12, 18, 65},
        {13, 14, 21},
        {14, 4, 61},
        {15, 15, 18},
        {16, 14, 26},
        {17, 5, 31},
        {18, 6, 3},
        {19, 35, 4},
        {20, 5, 45},
        {21, 3, 51},
        {22, 4, 61},
        {23, 5, 54},
        {24, 6, 82},
        {25, 5, 77},
        {26, 16, 24},
        {27, 17, 21},
        {28, 5, 36},
        {29, 16, 26},
        {30, 7, 31},
        {31, 13, 28},
        {32, 11, 21},
        {33, 6, 13},
        {34, 3, 11},
        {35, 4, 11},
        {36, 11, 4},
        {37, 11, 4},
        {38, 5, 4},
        {39, 22, 11},
        {40, 9, 18},
        {41, 14, 24},
        {42, 15, 30},
        {43, 8, 8},
        {44, 4, 10},
        {45, 5, 12},
        {46, 6, 14},
        {47, 7, 18},
        {48, 9, 12},
        {49, 12, 24},
        {50, 15, 30},
        {51, 8, 8},
        {52, 14, 46},
        {53, 17, 41},
        {54, 11, 42},
        {55, 15, 21},
        {56, 4, 32},
        {57, 7, 19},
        {58, 16, 33},
        {59, 10, 0},
        {60, 4, 14},
        {61, 5, 33},
        {62, 6, 51},
        {63, 14, 73},
        {64, 16, 87},
        {65, 6, 0}
    };

    sjf(processes);

    return 0;
}
