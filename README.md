# Pathfinding Algorithms Research

This repository contains C++ implementations and a test generator for a research paper on shortest path algorithms. The paper reviews various categories of graph algorithms — single source shortest path, all-pairs shortest path, and heuristic algorithms, analyzing their computational complexity, memory usage, and scalability. The code here supports the empirical evaluation of these algorithms by generating test graphs with specified sizes and densities. Implementations of the algorithms outlined in the paper are also provided.

## Research Context

This paper is written by Hristo Hristov, a bachelor student at the University of Ruse "A. Kanchev", Bulgaria, under the supervision of Assoc. Prof. Galina Atanasova. The work is prepared for a student scientific session at the university. The paper, titled *"Graph Shortest Path Algorithms and Their Applications"*, provides a comprehensive analysis of classical and modern graph algorithms and approaches. It explores their practical applications and trade-offs, drawing insights from recent developments in graph theory. Each algorithm's performance is evaluated based on the following two metrics:

- **Size of Graphs:** Evaluated using graphs of varying sizes (N = 100, 1,000, 10,000) with a fixed density (D = 0.1).
- **Density of Graphs:** Tested at (N = 1,000) with densities (D = 0.1, 0.5, 0.9).

Specific results regarding the performance and memory usage will be available in the paper itself.
This repository provides the tools to generate test graphs and will eventually include implementations of the algorithms discussed in the paper.

## Repository Contents

- **`testGenerator.cpp`**: A C++ program to generate random, connected graphs using the Erdős–Rényi model. It creates graphs with specified (N) (number of nodes) and (D) (density), with an option to allow negative edge weights.
- **Generated Graphs**: Test graph files (e.g., `graph_N100_D0.1_negfalse_1.txt`) stored in the repository root, containing edge lists for use in algorithm testing.
  - Format: First line is (N), followed by lines of `source target weight`.

## Usage

### Prerequisites

- A C++17-compatible compiler (e.g., `g++` version 13, which is used in this project).
- Git installed to clone and manage the repository.

### Setup

1. **Clone the Repository:**

   ```bash
   git clone https://github.com/Milkeles/GraphAlgorithms.git
   cd GraphAlgorithms
   ```

2. **Compile the Generator:**

   ```bash
   g++ -std=c++17 testGenerator.cpp -o testGenerator
   ```

   Note: This project was developed using `g++` 13, which supports C++17 and later standards.

3. **Run the Generator:**

   ```bash
   ./testGenerator
   ```

   This generates test graphs based on the parameters defined in `size_tests` and `density_tests` within `testGenerator.cpp`.

### Customizing Test Graphs

- **Modify Parameters:** Edit the `size_tests` and `density_tests` arrays in `testGenerator.cpp` to change (N) and (D). For example:

  ```cpp
  std::vector<std::pair<int, double>> size_tests = {
      {100, 0.1},    // Small
      {1000, 0.1},   // Medium
      {10000, 0.1}   // Large
  };
  ```
- **Negative Weights:** Toggle the `allow_negative_weights` boolean to `true` to include negative edge weights (range: -10 to 10, excluding 0).

### Future Additions

- C++ implementations of shortest path algorithms (e.g., Dijkstra’s, Bellman-Ford, A*).
- Test scripts to run and compare algorithm performance using the generated graphs.

## File Structure
- Algorithm implementations.
- `testGenerator.cpp`: Source code for the test graph generator.
- `graph_N*_D*_neg*_*.in`: Generated graph files (e.g., `graph_N100_D0.100000_negfalse_1.txt`).
- `README.md`: This file.
**Note:** *Due to the complexity of the algorithm outlined in the paper "Negative-weight single-source shortest paths in near-linear time" by Aaron Bernstein, Danupon Nanongkai and Christian Wulff-Nilsen, a java implementation provided by Nevin George was used. The said implementation can be found [here](https://github.com/nevingeorge/Negative-Weight-SSSP).*
