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

## Sample Work