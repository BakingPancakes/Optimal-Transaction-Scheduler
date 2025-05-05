#ifndef SCHEDULING_H
#define SCHEDULING_H

#include <queue>
#include <vector>
#include <stdexcept>
#include <ctime>

// Improved transaction structure with priority attributes
struct Transaction {
    // Basic transaction data
    int accountID;
    int targetID;  // Where money goes for transfers
    double amount;
    int type;      // 0=deposit, 1=withdraw, 2=transfer
    
    // Priority ranking factors
    double fee;            // Higher fee = higher priority
    time_t arrivalTime;    // Older = higher priority (fairness)
    int complexity;        // Lower number = simpler transaction
    int accountTier;       // Higher tier = VIP treatment (0-5)
    
    // Calculate priority score based on weights
    double getPriority(const std::vector<double>& weights) const;
};

// Scheduler that prioritizes transactions using weighted factors
class Scheduler {
private:
    // Weight values for [fee, time, complexity, accountTier]
    std::vector<double> weights;
    
    // Custom comparison for priority queue - had to use a class here
    class Compare {
    private:
        std::vector<double> weights; // Store a copy of the weights
    public:
        Compare(const std::vector<double>& w) : weights(w) {}
        bool operator()(const Transaction& a, const Transaction& b) const;
    };
    
    // Priority queue using custom comparator
    std::priority_queue<Transaction, std::vector<Transaction>, Compare> queue;
    
    // Keeping track of performance
    int processed;
    double totalFees;
    
public:
    // Initialize with default weights or custom ones
    Scheduler(std::vector<double> w = {0.25, 0.25, 0.25, 0.25});
    
    // Core functions
    void add(const Transaction& t);
    Transaction next();
    bool empty() const;
    
    // Change priority strategy on the fly
    void setWeights(const std::vector<double>& newWeights);
    
    // Get stats for analysis
    int getProcessedCount() const;
    double getTotalFees() const;
};

#endif // SCHEDULING_H