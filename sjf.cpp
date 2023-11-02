#include <iostream>
#include <vector>
#include <iomanip>
#include <queue>
#include <limits>
#include <algorithm>
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
const int limit = std::numeric_limits<int>::max();
using shortest = std::pair<int, int>;

class comparisonJob {
public:
    bool operator() (const pair<int, int>& a, const pair<int, int>& b) {
        if (a.first != b.first) {
            return a.first > b.first; // Smaller burst time has higher priority
        }
        return a.second > b.second; // Lower process ID has higher priority
    }
};

void sjf(vector<vector<int>> processVector, int time, int  CPUtime, int x, vector<int> bursts, priority_queue<shortest, vector<shortest>, greater<shortest>> ready) {
    vector<pair<int, int>> input;
    for (const auto& i : processVector) {
        input.emplace_back(MAX, x);
        x++;
    }

    vector<int> arrivalTime(processVector.size(),0);
    vector<int> waitingTime(processVector.size(),0);
    vector<int> responseTime(processVector.size(), -1);
    input[0].first = 0;
    while(!ready.empty()) {
        int front;
        tie(ignore, front) = ready.top();
        ready.pop();
        int update = processVector[front][bursts[front] + 1];

        if(responseTime[front] == -1) {
            responseTime[front] = time - arrivalTime[front];
        }
        waitingTime[front] += time - arrivalTime[front];
        time += processVector[front][bursts[front]];
        if(ready.empty()){
            while(input[0].first != MAX && input[0].first >= time) {
                time += 1;
                CPUtime += 1;
            }
        }
        if(bursts[front] < processVector[front].size() - 2){
            for(int n = 0; n < processVector.size(); n++){
                if(input[n].second == front){
                    input[n].first = update + time;
                    if (input[n].first < input[n].second) {
                        ready.pop();
                        cout << "Popped" << input[n].first;
                    }
                }
            }
        }

        cout << "\nStats:" << endl;
        cout<<"Current Execution Time: "<< time <<endl;
        cout<<"Next Process: Process "<< front <<endl; 

        int y = 0;
        
        for (auto& input : input) {
            if (input.first <= time) {
                ready.push([&]() {
                    return std::make_pair(processVector[input.second][bursts[input.second]], input.second);
                }());
                if (input.first < input.second) {
                    ready.pop();
                    cout << "Popped" << input.first;
                }
                arrivalTime[input.second] = input.first;
                input.first = MAX;
            }
        }   
        
        bursts[front] += 2;
        sort(input.begin(), input.end(), [](auto &left, auto &right) {
            return left.first < right.first;
        });
        second = input[y].second;
    }  
    vector<int> turnAroundtime(processVector.size(), 0);
    for(int n = 0; n < processVector.size(); n++){
        int counter = 0;
            for(int k = 0; k < processVector[n].size(); k++){
                counter += processVector[n][k];
            }
        turnAroundtime[n] += counter + waitingTime[n];
    }
    timeHolder += time;
    int addIncrement = 0;
    burstHolder = time - CPUtime;
    cout << "Processes" << "\t" << "Waiting time" << "\t" << "Turn Around Time" << "\t" << "Response Time" << endl;
    for (auto i : input) {
        waitHolder += waitingTime[addIncrement];
        turnAroundHolder += turnAroundtime[addIncrement];
        responseHolder += responseTime[addIncrement];
        
        cout << "P" << (addIncrement + 1) << "\t\t" << waitingTime[addIncrement] << "\t\t" << turnAroundtime[addIncrement] << "\t\t\t" << responseTime[addIncrement] << endl;
        addIncrement++;
    }
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
    vector<vector<int>> processVector(proc);
    priority_queue<shortest, vector<shortest>, greater<shortest>> pushQueue;
    processVector[0] = {5, 27, 3, 31, 5, 43, 4, 18, 6, 22, 4, 26, 3, 24, 4};
    processVector[1] = {4, 48, 5, 44, 7, 42, 12, 37, 9, 76, 4, 41, 9, 31, 7, 43, 8};
    processVector[2] = {8, 33, 12, 41, 18, 65, 14, 21, 4, 61, 15, 18, 14, 26, 5, 31, 6};
    processVector[3] = {3, 35, 4, 41, 5, 45, 3, 51, 4, 61, 5, 54, 6, 82, 5, 77, 3};
    processVector[4] = {16, 24, 17, 21, 5, 36, 16, 26, 7, 31, 13, 28, 11, 21, 6, 13, 3, 11, 4};
    processVector[5] = {11, 22, 4, 8, 5, 10, 6, 12, 7, 14, 9, 18, 12, 24, 15, 30, 8};
    processVector[6] = {14, 46, 17, 41, 11, 42, 15, 21, 4, 32, 7, 19, 16, 33, 10};
    processVector[7] = {4, 14, 5, 33, 6, 51, 14, 73, 16, 87, 6};
    vector<int> bursts(proc,0);int value = processVector[x][bursts[x]];
    priority_queue<pair<int, int>, vector<pair<int, int>>, comparisonJob> priority_queue;
    /*for (int i = 0; i < processVector.size(); i++) {
        pushQueue.push(make_pair(processVector[i][bursts[i]], i));
    }*/

    if (proc == 8) {
        int x = 0;
        for (auto i : processVector) {
            pushQueue.push([&]() {
                    return std::make_pair(value, x);
                }());
            x++;
        }
    }
    else {
        cout << "Error: Number of processVector is not 8" << endl;
        return 0;
    }

    cout << "\nSJF\n" << endl;
    sjf(processVector, time, CPUtime, x, bursts, pushQueue);
    printResult(waitHolder, turnAroundHolder, responseHolder, timeHolder);
    return 0;
}