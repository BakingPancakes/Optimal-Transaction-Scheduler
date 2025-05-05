#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <list>
#include <queue>
#include <string>
#include <ctime>
#include <iostream>
#include <vector>

using namespace std;


struct Transaction {
    int accountID;
    int otherID;  // For transfers
    double amount;
    int mode;     // 0=deposit, 1=withdraw, 2=transfer

    // Scheduler fields
    double fee;
    time_t arrivalTime;
    int complexityScore;

    // Method to calculate priority based on weights
    double calculatePriority(double w1, double w2, double w3) const {
        return w1 * fee - w2 * difftime(time(nullptr), arrivalTime) + w3 * complexityScore;
    }

    // Comparator for priority_queue
    bool operator<(const Transaction& other) const {
        // Default priority for queue (lowest at top); higher value = higher priority
        return calculatePriority(1.0, 1.0, 1.0) < other.calculatePriority(1.0, 1.0, 1.0);
    }
};

class PriorityScheduler {
private:
    priority_queue<Transaction> transactionQueue;
    double feeWeight;
    double timeWeight;
    double complexityWeight;

public:
    PriorityScheduler(double w1 = 1.0, double w2 = 1.0, double w3 = 1.0)
        : feeWeight(w1), timeWeight(w2), complexityWeight(w3) {}

    void addTransaction(Transaction t) {
        transactionQueue.push(t);
    }

    Transaction getNextTransaction() {
        if (transactionQueue.empty()) throw runtime_error("Queue is empty");
        Transaction t = transactionQueue.top();
        transactionQueue.pop();
        return t;
    }

    void adjustWeights(double w1, double w2, double w3) {
        feeWeight = w1;
        timeWeight = w2;
        complexityWeight = w3;
    }

};

// ========================= Process Scheduling Structures =========================

typedef struct Process Process;
struct Process {
    int arrival;
    int first_run;
    int duration;
    int completion;
};

class ArrivalComparator {
public:
    bool operator()(const Process lhs, const Process rhs) const {
        if (lhs.arrival != rhs.arrival)
            return lhs.arrival > rhs.arrival;
        else
            return lhs.duration > rhs.duration;
    }
};

class DurationComparator {
public:
    bool operator()(const Process lhs, const Process rhs) const {
        if (lhs.duration != rhs.duration)
            return lhs.duration > rhs.duration;
        else
            return lhs.arrival > rhs.arrival;
    }
};

typedef priority_queue<Process, vector<Process>, ArrivalComparator> pqueue_arrival;
typedef priority_queue<Process, vector<Process>, DurationComparator> pqueue_duration;

pqueue_arrival read_workload(string filename);
void show_workload(pqueue_arrival workload);
void show_processes(list<Process> processes);

list<Process> fifo(pqueue_arrival workload);
list<Process> sjf(pqueue_arrival workload);
list<Process> stcf(pqueue_arrival workload);
list<Process> rr(pqueue_arrival workload);

float avg_turnaround(list<Process> processes);
float avg_response(list<Process> processes);
void show_metrics(list<Process> processes);

// ========================= Debug Macro =========================

#ifdef DEBUGMODE
#define debug(msg) \
    std::cout << "[" << __FILE__ << ":" << __LINE__ << "] " << msg << std::endl;
#else
#define debug(msg)
#endif

#endif // SCHEDULER_H
