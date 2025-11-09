# Stock Price Analysis System (DSA Project)

A console-based Stock Price Analysis Application built in **C++**, demonstrating the use of core **Data Structures and Algorithms** such as **Arrays, Vectors, Stacks, Queues, Sorting, Searching, and File Handling**.  
This project allows users to store, update, analyze, and visualize historical stock price data interactively.

---

## Features

- **Add New Stock Data** (company name + daily price records)
- **Display All Stored Stocks**
- **Append New Daily Prices** using Queue
- **View Prices (Oldest → Newest & Newest → Oldest)** using **Queue & Stack**
- **Maximum and Minimum Price Analysis**
- **7-Day Moving Average Calculation**
- **Sort Prices** (Ascending / Descending)
- **Price Trend Analysis** (Increasing / Decreasing / Stable)
- **Search Price by Day**
- **Delete Stock**
- **Auto Save & Load using File (`stock.txt`)**

---

## Data Structures Used

| Data Structure | Usage |
|---------------|-------|
| `vector`      | Storing daily stock prices |
| `queue`       | Adding new prices & sequential display |
| `stack`       | Reverse display (latest to oldest) & last accessed stocks |
| `file handling` | Storing and loading stock records persistently |
| `sort()` | Sorting prices for analysis |

---

## How to Run

1. Ensure you have a C++ compiler installed (e.g., GCC).
2. Compile the program:
   ```bash
   g++ project.cpp -o stock_app
