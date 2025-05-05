#include "../include/scheduling.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>

// Load transactions from one of workload files
std::vector<Transaction> loadWorkload(const std::string& filename) {
    std::vector<Transaction> transactions;
    std::ifstream file(filename);
    
    if (!file) {
        std::cerr << "Couldn't open " << filename << "... check the path?" << std::endl;
        return transactions;
    }
    
    Transaction t;
    // Parse each line in the file - skip lines starting with #
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;  // Skip comments and empty lines
        }
        
        std::istringstream iss(line);
        if (iss >> t.accountID >> t.targetID >> t.amount >> t.type 
               >> t.fee >> t.arrivalTime >> t.complexity >> t.accountTier) {
            transactions.push_back(t);
        }
    }
    
    return transactions;
}

// Run a workload with specific priority weights
void processWorkload(const std::string& filename, 
                    const std::vector<double>& weights, 
                    const std::string& strategyName) {
    auto transactions = loadWorkload(filename);
    if (transactions.empty()) {
        std::cout << "No transactions found in " << filename << std::endl;
        return;
    }
    
    std::cout << "Processing " << transactions.size() << " transactions with " 
              << strategyName << " strategy..." << std::endl;
    
    // Create scheduler with the chosen weights
    Scheduler scheduler(weights);
    
    // Add all transactions to the queue
    for (const auto& t : transactions) {
        scheduler.add(t);
    }
    
    // Process everything and time it
    auto startTime = std::chrono::steady_clock::now();
    
    std::cout << "  Processing transactions in priority order:" << std::endl;
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
        std::cout << "  " << count << ". Transaction ID: " << t.accountID 
                  << ", Priority: " << t.getPriority(weights)
                  << " [Fee: $" << t.fee
                  << ", Arrival: " << timeBuffer
                  << ", Wait: " << waitTime << " hrs"
                  << ", Complexity: " << t.complexity
                  << ", Account Tier: " << t.accountTier << "]" << std::endl;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(10 * t.complexity));
    }
    
    auto endTime = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    
    // Show the results
    std::cout << "  Processed: " << scheduler.getProcessedCount() << " transactions" << std::endl;
    std::cout << "  Total fees: $" << scheduler.getTotalFees() << std::endl;
    std::cout << "  Processing time: " << duration.count() / 1000.0 << " seconds" << std::endl;
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    // Default to first workload if none specified
    std::string workload = "workloads/workload_01.txt";
    if (argc > 1) {
        workload = argv[1];
    }
    
    // Only process the Balanced Priority strategy
    processWorkload(workload, {0.4, 0.3, 0.2, 0.1}, "Balanced Priority");
    
    return 0;
}