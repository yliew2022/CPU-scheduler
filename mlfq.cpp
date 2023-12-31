#include <iostream>
#include <vector>
#include <iomanip>
#include <queue>
#include <limits>
#include <algorithm>
#include <math.h>
#include <numeric>
#define MAX limit
using namespace std;

vector<int> arrivalTime;
vector<int> waitingTime;
vector<int> turnAroundtime;
vector<int> responseTime;
vector<int> completeBool;
int timeHolder = 0;
int waitHolder = 0;
int burstHolder;
int second;
int turnAroundHolder = 0;
float responseHolder = 0;
// Get max limit of int
const int limit = numeric_limits<int>::max();

void updateInput(int front, int update, int time, vector<int>bursts, vector<vector<int>>& processVector, vector<pair<int, int>>& input, queue<int>& ready) {
    
    if (bursts[front] < processVector[front].size() - 2) {
            for (auto &input : input) {
                if (input.second == front) {
                    input.first = update + time;
                }
                if (input.first < input.second) {
                    ready.pop();
                    cout << "Popped " << input.first;
                }
            }
    }
}

void processLevel2Temp(int& time, int temp, vector<int>bursts, vector<vector<int>>& processVector, queue<int>& secondQueue, int& level, vector<int>& arrivalTime, int front, int tq1) {
    if (temp > tq1) {
        time += tq1;
        processVector[front][bursts[front]] = temp - tq1;
        secondQueue.push(front);
        level = 2;
        arrivalTime[front] = time;
    }
}

void processLevel3Temp(int& time, int temp, vector<int>bursts, vector<vector<int>>& processVector, queue<int>& secondQueue, int& level, vector<int>& arrivalTime, int front, int tq2) {
    if (temp > tq2) {
        time += tq2;
        processVector[front][bursts[front]] = temp - tq2;
        secondQueue.push(front);
        level = 3;
        arrivalTime[front] = time;
    }
}


void mlfq(vector<vector<int>> processVector, int level, int tq1, int tq2, int time, int  CPUtime, int x, queue<int> ready, vector<int> bursts) {
    // Define tally for results
    
    
    // Response time has to start at -1 otherwise all results will be 0
    
    vector<pair<int, int>> input;
    // Used pair command because it's split between (bursts, io time, bursts, io time, etc).
    // Every number is split into pairs one after the other
    for (const auto& i : processVector) {
        input.emplace_back(MAX, x);
        x++;
    }
    vector<int> arrivalTime(processVector.size(),0);
    vector<int> waitingTime(processVector.size(),0);
    vector<int> responseTime(processVector.size(), -1);
    queue<int> secondQueue;
    queue<int> fcfsQueue;
    input[0].first = 0;
    // While ready queue is not empty create front as the front of queue
    while(!ready.empty() || !secondQueue.empty() || !fcfsQueue.empty()) {
            if(!ready.empty()) {
                int front = ready.front();
                int update = processVector[front][bursts[front] + 1];
                ready.pop();

                waitingTime[front] += time - arrivalTime[front];
                responseTime[front] = (responseTime[front] == -1) ? (time - arrivalTime[front]) : responseTime[front];
                int temp = processVector[front][bursts[front]];
                cout << "\nStats:" << endl;
                cout<<"Current Execution Time: "<< time <<endl;
                cout<<"Next Process: Process "<< front <<endl; 
                if (temp > tq1) {
                    processLevel3Temp(time, temp, bursts, processVector, secondQueue, level, arrivalTime, front, tq1);
                }
                else {
                    time += processVector[front][bursts[front]];
                    updateInput(front, update, time, bursts,processVector, input, ready);
                bursts[front] += 2;    
                } 
            }
            else if (!secondQueue.empty()) {
                int front = secondQueue.front();
                int update = processVector[front][bursts[front] + 1];
                secondQueue.pop();

                int temp = processVector[front][bursts[front]];

                cout << "\nStats:" << endl;
                cout<<"Current Execution Time: "<< time <<endl;
                cout<<"Next Process: Process "<< front <<endl; 
                if (temp > tq2) {
                    processLevel3Temp(time, temp, bursts, processVector, secondQueue, level, arrivalTime, front, tq2);
                }
                else {
                    time += processVector[front][bursts[front]];
                    updateInput(front, update, time, bursts,processVector, input, ready);
                bursts[front] += 2;
                }
            }
            else if (!fcfsQueue.empty()) {
                int front = fcfsQueue.front();
                int update = processVector[front][bursts[front] + 1];
                fcfsQueue.pop();
                waitingTime[front] += time - arrivalTime[front];
                responseTime[front] = (responseTime[front] == -1) ? (time - arrivalTime[front]) : responseTime[front];
                int temp = processVector[front][bursts[front]];
                cout << "\nStats:" << endl;
                cout<<"Current Execution Time: "<< time <<endl;
                cout<<"Next Process: Process "<< front <<endl; 
                time += processVector[front][bursts[front]];
                updateInput(front, update, time, bursts,processVector, input, ready);
                bursts[front] += 2;
                
            }
            sort(input.begin(), input.end(), [](auto &left, auto &right) {
                return left.first < right.first;
            });

            while ((ready.empty() && secondQueue.empty() && fcfsQueue.empty()) && (input[0].first != MAX && input[0].first > time)) {
                time += 1;
                CPUtime += 1;
            }
            for (auto& entry : input) {
                int i = entry.first;
                int process = entry.second;

                if (i <= time) {
                    switch (level) {
                        case 1:
                            ready.push(process);
                            break;
                        case 2:
                            secondQueue.push(process);
                            break;
                        default:
                            fcfsQueue.push(process);
                            arrivalTime[process] = i;
                            entry = make_pair(MAX, process); // Replace the pair with a new value
                            break;
                    }
                }
            }
        }
        vector<int> turnAroundtime(processVector.size(), 0);
        for (int n = 0; n < processVector.size(); n++) {
            int counter = accumulate(processVector[n].begin(), processVector[n].end(), 0);
            turnAroundtime[n] += counter + waitingTime[n];
        }
        timeHolder += time;
        int addIncrement = 0;
        burstHolder = timeHolder - CPUtime;
        cout << "Processes" << "\t" << "Waiting time" << "\t" << "Turn Around Time" << "\t" << "Response Time" << endl;
        for (const auto& i : input) {
            int addIncrement = &i - &input[0];
            waitHolder += waitingTime[addIncrement];
            turnAroundHolder += turnAroundtime[addIncrement];
            responseHolder += responseTime[addIncrement];
            cout << "P" << (addIncrement + 1) << "\t\t" << waitingTime[addIncrement] << "\t\t" << turnAroundtime[addIncrement] << "\t\t\t" << responseTime[addIncrement] << endl;
        }
        //burstHolder += calculateBurst;
        
        
    }

void printResult(int waittime, int turnaround, float responsetime, int time) {
    float calculateCPU = (float)burstHolder / timeHolder * 100;
    cout << "Complete time: " << timeHolder << endl;
    cout << "\nCPU Usage: " << fixed << setprecision(2) << calculateCPU << endl;
    cout << "\nAverage Waiting time: " << waittime/8 << endl;
    cout << "\nAverage Turnaround time: " << turnaround/8 << endl;
    cout << "\nAverage Response time: " << fixed << setprecision(2) << responsetime/8 << "\n" << endl;
}

int main() {
    int proc = 8;
    int x = 0;
    int time = 0;
    int  CPUtime = 0;
    int tq1 = 5;
    int tq2 = 10;
    int level;
    vector<vector<int>> processVector(proc);
    queue <int> pushQueue;
    processVector[0] = {5, 27, 3, 31, 5, 43, 4, 18, 6, 22, 4, 26, 3, 24, 4};
    processVector[1] = {4, 48, 5, 44, 7, 42, 12, 37, 9, 76, 4, 41, 9, 31, 7, 43, 8};
    processVector[2] = {8, 33, 12, 41, 18, 65, 14, 21, 4, 61, 15, 18, 14, 26, 5, 31, 6};
    processVector[3] = {3, 35, 4, 41, 5, 45, 3, 51, 4, 61, 5, 54, 6, 82, 5, 77, 3};
    processVector[4] = {16, 24, 17, 21, 5, 36, 16, 26, 7, 31, 13, 28, 11, 21, 6, 13, 3, 11, 4};
    processVector[5] = {11, 22, 4, 8, 5, 10, 6, 12, 7, 14, 9, 18, 12, 24, 15, 30, 8};
    processVector[6] = {14, 46, 17, 41, 11, 42, 15, 21, 4, 32, 7, 19, 16, 33, 10};
    processVector[7] = {4, 14, 5, 33, 6, 51, 14, 73, 16, 87, 6};
    vector<int> bursts(proc,0);
    if (proc == 8) {
        int x = 0;
        for (auto i : processVector) {
            pushQueue.push(x);
            x++;
        }
    }
    else {
        cout << "Error: Number of processVector is not 8" << endl;
        return 0;
    }

    cout << "\nmlfq\n" << endl;
    mlfq(processVector, level, tq1, tq2, time, CPUtime, x, pushQueue, bursts);
    printResult(waitHolder, turnAroundHolder, responseHolder, timeHolder);
    return 0;
}