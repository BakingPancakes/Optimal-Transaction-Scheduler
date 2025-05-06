# Optimal Transaction Scheduler

My implementation of a bank transaction scheduler that prioritizes transactions based on multiple factors instead of just using FIFO.

## Key Features

- **Multi-factor prioritization**
  - Fee revenue optimization (make more money!)
  - Arrival time fairness (older transactions shouldn't wait forever)
  - Processing complexity awareness (simpler jobs first)
  - Account tier support (VIP customers get faster service)
- **Configurable scheduling strategies**
  - Change weights to prioritize different factors
  - All weights are normalized automatically
- **Fairness guarantees**
  - No transaction starvation 
  - Balance between making money and being fair
- **Performance tracking**
  - Measures total fees collected
  - Tracks processing time

## Project Structure

```
project2-scheduler/
├── include/
│   └── scheduling.h       # Transaction and Scheduler classes
├── src/
│   ├── scheduling.cpp     # Core scheduler implementation
│   ├── main_scheduling.cpp # Main program
│   ├── accounts.cpp       # Account management 
│   └── test.cpp           # Unit tests
├── obj/
│   └── (build files go here)
├── bin/
│   └── (executables go here)
├── workloads/
│   ├── workload_01.txt    # Simple workload
│   ├── workload_02.txt    # Fee priority workload
│   ├── workload_03.txt    # Time priority workload
│   ├── workload_04.txt    # Account tier workload
│   └── workload_05.txt    # Complex mixed workload
├── Makefile
└── README.md
```

## Building and Running

```bash
# Build everything
make

# Run the test suite
make test

# Run with all workloads
make run

# Run with specific workload
make run1  # Simple workload
make run2  # Fee priority workload 
make run3  # Time priority workload
make run4  # Account tier workload
make run5  # Complex mixed workload

# Clean up build files
make clean
```

## Scheduling Strategies

I've implemented several strategies to test different priority approaches:

1. **Fee Priority** `[1,0,0,0]`
   - Maximize bank revenue
   - Process high-fee transactions first

2. **Time Priority (FIFO)** `[0,1,0,0]`
   - First-come, first-served
   - Traditional queue behavior

3. **Complexity Priority** `[0,0,1,0]`
   - Simple transactions first
   - Better throughput

4. **Account Tier Priority** `[0,0,0,1]`
   - VIPs first
   - Keep premium customers happy

5. **Balanced Priority** `[0.4,0.3,0.2,0.1]`
   - Weighted combination
   - My recommended approach

## Test Cases

For testing our transaction scheduler, I created five different workload files to evaluate how our priority algorithms perform in various scenarios:

1. **Simple Workload (workload_01.txt)**
   This basic workload contains just 5 transactions with similar characteristics (standard accounts, similar complexity, moderate fees). It serves as our baseline for initial testing and verification that the scheduler works correctly. This workload shouldn't show dramatic differences between strategies since the transactions don't vary much in their properties.

2. **Fee Priority Workload (workload_02.txt)**
   Contains 10 transactions with widely varying fees (from $1.25 to $100.00) but similar arrival times and complexity levels. This tests whether our fee prioritization actually processes high-fee transactions first, maximizing revenue. When running with fee priority weights `[1,0,0,0]`, we expect to see transactions processed in descending fee order (492, 831, 305, 939, etc.).

3. **Time Priority Workload (workload_03.txt)**
   Contains 10 transactions with arrival times spanning from 24+ hours ago to just now. This tests whether our system respects FIFO principles when using time priority weights `[0,1,0,0]` and whether our anti-starvation mechanisms work. We expect older transactions (101, 305, 764) to be processed before newer ones (614, 939).

4. **Account Tier Workload (workload_04.txt)**
   Contains 10 transactions with account tiers ranging from 0 (regular) to 5 (highest VIP). All transactions have identical arrival times to isolate the tier effect. When using account tier priority weights `[0,0,0,1]`, we expect to see VIP accounts (101, 305, 764) processed first, followed by standard accounts (831, 403, etc.).

5. **Complex Mixed Workload (workload_05.txt)**
   Our most sophisticated test case with 10 transactions having competing priorities: some are high-fee but complex, others are simple but low-fee, some are old VIP transactions vs. new high-fee ones, etc. This tests how our balanced strategy `[0.4,0.3,0.2,0.1]` handles real-world scenarios with trade-offs between different priority factors. We expect a processing order that's different from any single-factor strategy.

The unit tests in `test.cpp` additionally verify two critical aspects of our scheduler:
1. **Priority Ordering**: Tests that each individual priority factor (fee, time, complexity, account tier) correctly influences transaction ordering when that factor has 100% weight.
2. **Fairness/Starvation Prevention**: Tests that very old transactions eventually get processed even when competing against many newer high-fee transactions, ensuring no transaction waits indefinitely.

## Workload Format

Each line in the workload files represents one transaction:
```
accountID targetID amount type fee arrivalTime complexity accountTier
```

Where:
- `accountID`: Source account
- `targetID`: Destination account for transfers
- `amount`: Transaction amount
- `type`: 0=deposit, 1=withdraw, 2=transfer
- `fee`: Fee amount (higher = higher priority)
- `arrivalTime`: When the transaction arrived (Unix timestamp)
- `complexity`: How complex/slow to process (1-10)
- `accountTier`: Premium level (0=regular, 1-5=VIP)