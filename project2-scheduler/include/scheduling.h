#ifndef SCHEDULING_H
#define SCHEDULING_H

#include <queue>
#include <vector>
#include <stdexcept>
#include <ctime>

using namespace std;

/**
 * Set up our Transaction struct and Scheduling class (since we want to have methods).
 */

 // Components that will be ranked against one another by scheduler
struct Transaction {
    // Basic metadata
    int accountID;
    int targetID;
    double amount;
    int type;      // 0=deposit, 1=withdraw, 2=transfer
    
    // Priority ranking factors
    double fee;             // Higher fee = higher priority
    time_t arrivalTime;     // Older = higher priority (fairness)
    int complexity;         // Lower number = simpler transaction
    int accountTier;        // Higher tier = VIP treatment (0-5)
    
    // Calculate priority score based on weights
    double getPriority(const vector<double>& weights) const;
};

// Scheduler that prioritizes transactions using weighted factors
class Scheduler {
private:
    // Weight values for [fee, time, complexity, accountTier]
    vector<double> weights;
    
    // Custom comparison for priority queue - had to use a class here
    class Compare {
    private:
        vector<double> weights; // Store a copy of the weights
    public:
        Compare(const vector<double>& w) : weights(w) {}
        bool operator()(const Transaction& a, const Transaction& b) const;
    };
    
    // Priority queue using custom comparator
    priority_queue<Transaction, vector<Transaction>, Compare> queue;
    
    // Keep track of performance for evaluation
    int processed;
    double totalFees;
    
public:
    // Initialize with weights, default gives uniform weights between all factors
    Scheduler(vector<double> w = {0.25, 0.25, 0.25, 0.25});
    
    // Priroity Queue
    void add(const Transaction& t);
    Transaction next();
    bool empty() const;
    
    // Change priority strategy during runtime (optional for testing)
    void setWeights(const vector<double>& newWeights);
    
    // Get stats for evaluation
    int getProcessedCount() const;
    double getTotalFees() const;
};

#endif // SCHEDULING_H