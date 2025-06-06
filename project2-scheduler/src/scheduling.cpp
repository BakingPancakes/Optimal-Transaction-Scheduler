#include "../include/scheduling.h"
#include <ctime>
#include <algorithm>
#include <numeric>
#include <iostream>

/**
 * This file creates our class methods for our scheduling functionality.
 * 
 * === Overview: ===
 *  Priority queue:
 * - Transaction::getPriority(vector<weights>) -> calculate priority for given transaction (added here for codebase coherence)
 * - Scheduler::add(Transaction)    -> add a transaction to the scheduling queue
 * - Scheduler::next()              -> grab transaction with next highest priority
 * - Scheduler::empty()             -> check if queue is empty
 * - Scheduler::compare::operator() -> compare two transactions directly
 * - Scheduler::setWeights(vector<newWeights>)        -> change weights (initialized in constructor) between tests
 * 
 *  For evaluation:
 * - Scheduler::getProcessedCount() -> retrieve total processed transactions
 * - Scheduler::getTotalFees()      -> retrieve total fees collected (revenue)
 */

// Priority score calculation algorithm
double Transaction::getPriority(const std::vector<double>& weights) const {
    // Get current time to figure out how long this has been waiting
    time_t now = time(nullptr);
    double waitTime = difftime(now, arrivalTime) / 3600.0;  // Convert to hours
    
    // Calculate scores for each factor (higher is better)
    double feeScore = fee;  // More money = higher priority
    double timeScore = waitTime;  // Waited longer = higher priority
    double complexityScore = 1.0 / (complexity + 1.0);  // Invert so lower complexity = higher score
    double tierScore = accountTier;  // Higher tier = higher priority
    
    // Calculate priority according to input weights
    double priority = weights[0] * feeScore + 
                      weights[1] * timeScore + 
                      weights[2] * complexityScore + 
                      weights[3] * tierScore;

    return priority;
}

// Construct scheduler with initial weights
Scheduler::Scheduler(std::vector<double> w) : 
    weights(w),
    queue(Compare(weights)),
    processed(0),
    totalFees(0) {
    
    // Normalize weights so they add up to 1.0
    double sum = std::accumulate(weights.begin(), weights.end(), 0.0);
    if (sum > 0) {
        for (auto& weight : weights) {
            weight /= sum;
        }
    }
}

// Add a transaction to the queue
void Scheduler::add(const Transaction& t) {
    queue.push(t);
}

// Get the next highest priority transaction
Transaction Scheduler::next() {
    if (queue.empty()) {
        throw std::runtime_error("Queue is empty - nothing to process.");
    }
    
    Transaction t = queue.top();
    queue.pop();
    
    // Update stats
    processed++;
    totalFees += t.fee;
    
    return t;
}

// Check if we have more transactions to process
bool Scheduler::empty() const {
    return queue.empty();
}


// Custom comparator for the priority queue
bool Scheduler::Compare::operator()(const Transaction& lhs, const Transaction& rhs) const {
    // Lower scores come out last from priority queue, so flip the comparison
    // This was tricky to figure out :/
    return lhs.getPriority(weights) < rhs.getPriority(weights);
}

// Change how we prioritize transactions by updating weights
void Scheduler::setWeights(const std::vector<double>& newWeights) {
    // Make sure we have exactly 4 weights
    if (newWeights.size() != 4) {
        throw std::invalid_argument("Usage: please give exactly 4 weights {fee, time, complexity, tier}");
    }
    
    // Set and normalize weights
    weights = newWeights;
    double sum = std::accumulate(weights.begin(), weights.end(), 0.0);
    if (sum > 0) {
        for (auto& weight : weights) {
            weight /= sum;
        }
    }
    
    // Rebuild the queue with new weights
    std::vector<Transaction> transactions;
    while (!queue.empty()) {
        transactions.push_back(queue.top());
        queue.pop();
    }
    
    queue = std::priority_queue<Transaction, std::vector<Transaction>, Compare>(Compare(weights));
    for (const auto& t : transactions) {
        queue.push(t);
    }
}

// Evaluation metrics:

// Return how many transactions we've processed
int Scheduler::getProcessedCount() const {
    return processed;
}

// Return total fees collected (our revenue)
double Scheduler::getTotalFees() const {
    return totalFees;
}