import queue

# Function to calculate FCFS
def fcfs(processes, ready, bursts):
    arrivalTime = [0] * len(processes)
    waitingTime = [0] * len(processes)
    responseTime = [-1] * len(processes)
    input = [(float('inf'), x) for x in range(len(processes))]
    time = 0
    CPUtime = 0
    calculateBurst = time - CPUtime

    while not ready.empty():
        front = ready.get()
        update = processes[front][bursts[front] + 1]

        if responseTime[front] == -1:
            responseTime[front] = time - arrivalTime[front]

        waitingTime[front] += time - arrivalTime[front]
        time += processes[front][bursts[front]]

        if bursts[front] <= len(processes[front]) - 3:
            for n in range(len(processes)):
                if input[n][1] == front:
                    input[n] = (update + time, input[n][1])
                    if input[n][0] < time:
                        ready.get()

        y = 0
        for x in input:
            if input[y][0] <= time:
                ready.put(input[y][1])
                if input[y][0] > time:
                    ready.get()
                arrivalTime[input[y][1]] = input[y][0]
                input[y] = (float('inf'), input[y][1])
                y += 1

        if ready.empty():
            while input[0][0] != float('inf') and input[0][0] > time:
                for i in range(len(input)):
                    time += 1
                    CPUtime += 1

        bursts[front] += 2
        input.sort(key=lambda x: x[0])

    turnAroundtime = [0] * len(processes)
    for n in range(len(processes)):
        counter = 0
        for k in range(len(processes[n]):
            counter += processes[n][k]
            print("Current burst:", counter)
        turnAroundtime[n] += counter + waitingTime[n]

    global burstHolder
    global timeHolder
    burstHolder += calculateBurst
    timeHolder += time

    for i in range(len(processes)):
        print("Process:", i, "Waiting Time:", waitingTime[i])
        global waitHolder
        waitHolder += waitingTime[i]

    for i in range(len(processes)):
        print("Process:", i, "Turnaround Time:", turnAroundtime[i])
        global turnAroundHolder
        turnAroundHolder += turnAroundtime[i]

    for i in range(len(processes)):
        print("Process:", i, "Response Time:", responseTime[i])
        global responseHolder
        responseHolder += responseTime[i]


# Function to print results
def printResult(waittime, turnaround, responsetime):
    print("CPU Usage:", burstHolder / timeHolder * 100)
    print("\nWaiting time:", waittime / 8)
    print("\nTurnaround time:", turnaround / 8)
    print("\nResponse time:", responsetime / 8)


if __name__ == "__main__":
    proc = 8
    processes = [[] for _ in range(proc)]
    ready = queue.Queue()
    bursts = [0] * proc

    processes[0] = [5, 27, 3, 31, 5, 43, 4, 18, 6, 22, 4, 26, 3, 24, 4]
    processes[1] = [4, 48, 5, 44, 7, 42, 12, 37, 9, 76, 4, 41, 9, 31, 7, 43, 8]
    processes[2] = [8, 33, 12, 41, 18, 65, 14, 21, 4, 61, 15, 18, 14, 26, 5, 31, 6]
    processes[3] = [3, 35, 4, 41, 5, 45, 3, 51, 4, 61, 5, 54, 6, 82, 5, 77, 3]
    processes[4] = [16, 24, 17, 21, 5, 36, 16, 26, 7, 31, 13, 28, 11, 21, 6, 13, 3, 11, 4]
    processes[5] = [11, 22, 4, 8, 5, 10, 6, 12, 7, 14, 9, 18, 12, 24, 15, 30, 8]
    processes[6] = [14, 46, 17, 41, 11, 42, 15, 21, 4, 32, 7, 19, 16, 33, 10]
    processes[7] = [4, 14, 5, 33, 6, 51, 14, 73, 16, 87, 6]

    x = 0
    for i in processes:
        ready.put(x)
        x += 1

    burstHolder = 0
    timeHolder = 0
    waitHolder = 0
    turnAroundHolder = 0
    responseHolder = 0

    print("\nFCFS\n")
    fcfs(processes, ready, bursts)
    printResult(waitHolder, turnAroundHolder, responseHolder)
