#include "../include/scheduling.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>

using namespace std;

// Load transactions from one of workload files
vector<Transaction> loadWorkload(const string& filename) {
    vector<Transaction> transactions;
    ifstream file(filename);
    
    if (!file) {
        cerr << "Couldn't open " << filename << "... check the path?" << endl;
        return transactions;
    }
    
    Transaction t;
    // Parse each line in the file - skip lines starting with #
    string line;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;  // Skip comments and empty lines
        }
        
        istringstream iss(line);
        if (iss >> t.accountID >> t.targetID >> t.amount >> t.type 
               >> t.fee >> t.arrivalTime >> t.complexity >> t.accountTier) {
            transactions.push_back(t);
        }
    }
    
    return transactions;
}

// Run a workload with specific priority weights
void processWorkload(const string& filename, 
                    const vector<double>& weights, 
                    const string& strategyName) {
    auto transactions = loadWorkload(filename);
    if (transactions.empty()) {
        cout << "No transactions found in " << filename << endl;
        return;
    }
    
    cout << "Processing " << transactions.size() << " transactions with " 
              << strategyName << " strategy..." << endl;
    
    // Create scheduler with the chosen weights
    Scheduler scheduler(weights);
    
    // Add all transactions to the queue
    for (const auto& t : transactions) {
        scheduler.add(t);
    }
    
    // Process everything and time it
    auto startTime = chrono::steady_clock::now();
    
    cout << "  Processing transactions in priority order:" << endl;
    int count = 0;
    while (!scheduler.empty()) {
        Transaction t = scheduler.next();
        
        // Convert arrival timestamp to human-readable format
        char timeBuffer[30];
        struct tm *timeInfo = localtime(&t.arrivalTime);
        strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", timeInfo);
        
        // Current time for wait calculation
        time_t now = time(nullptr);
        double waitTime = difftime(now, t.arrivalTime) / 3600.0;  // Hours
        
        // Print transaction information with count for ordering
        count++;
        cout << "  " << count << ". Transaction ID: " << t.accountID 
                  << ", Priority: " << t.getPriority(weights)
                  << " [Fee: $" << t.fee
                  << ", Arrival: " << timeBuffer
                  << ", Wait: " << waitTime << " hrs"
                  << ", Complexity: " << t.complexity
                  << ", Account Tier: " << t.accountTier << "]" << endl;
        
        this_thread::sleep_for(chrono::milliseconds(10 * t.complexity));
    }
    
    auto endTime = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);
    
    // Show the results
    cout << "  Processed: " << scheduler.getProcessedCount() << " transactions" << endl;
    cout << "  Total fees: $" << scheduler.getTotalFees() << endl;
    cout << "  Processing time: " << duration.count() / 1000.0 << " seconds" << endl;
    cout << endl;
}

int main(int argc, char* argv[]) {
    // Default to first workload if none specified
    string workload = "workloads/workload_01.txt";
    if (argc > 1) {
        workload = argv[1];
    }
    
    // Only process the Balanced Priority strategy
    processWorkload(workload, {0.4, 0.3, 0.2, 0.1}, "Balanced Priority");
    
    return 0;
}