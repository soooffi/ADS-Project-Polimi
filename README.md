# MOVHEX- ADS Project @Polimi 
Algorithms and Data Structures Project at Politecnico di Milano, A.Y. 2024/2025  


---


## Objective

This is one of the three mandatory projects required to graduate with a **Bachelor's Degree in Computer Engineering at Politecnico di Milano**. The project changes every year with a new problem statement.

For the **A.Y. 2024/2025**, the full problem specification is available here: [Problem Statement](./problem_statement.pdf)


---


##  Performance
The table below shows the time and memory complexity requirements that must be met to achieve each grade.

| Grade | Time Limit [s] | Memory Limit [MiB] |
|-------|---------------|-------------------|
| 18 | 30.000 s | 194.0 MiB |
| 21 | 25.250 s | 152.0 MiB |
| 24 | 20.500 s | 110.0 MiB |
| 27 | 12.000 s | 68.0 MiB |
| 30 | 11.000 s | 26.0 MiB |
| **30L** | **10.000 s** | **26.0 MiB** |


---


## My Final Grade

| | |
|---|---|
| **Grade** | 30L |
| **Time Limit [s]** | 8.731s |
| **Memory Limit [MiB]** | 24.5s |



---


##  Solution

The project was implemented in **C**, using efficient data structures to manage both the hexagonal grid and the air routes.

The most challenging part at the beginning was handling the **neighbors** in a hexagonal grid: unlike a square grid, each cell (excluding border ones) has exactly 6 neighbors, and their indices differ depending on whether the row is even or odd. To solve this, I created a struct containing a fixed-size array of 6 neighbor indices, computed statically during initialization in the `init` function — avoiding any runtime overhead.

Initially the struct held more fields, but after a deeper analysis of the problem I identified recurring patterns (for example, the cost of an air route is always equal to the exit cost of the source hexagon) which allowed me to simplify the data structures significantly.

### Function Breakdown

**`init`** — Allocates the grid and precomputes the 6 neighbors for each hexagon based on its row parity. All cells are initialized with cost 1 and no air routes.

**`change_cost`** — Uses a **BFS** from the target hexagon to propagate cost changes within the given radius. The delta applied decreases linearly with distance from the center, and values are clamped to [0, 100].

**`toggle_air_route`** — Manages a **linked list** of air routes for each hexagon. If the route already exists it is removed, otherwise a new node is added. The cost is computed as the floor average of all existing outgoing air route costs and the hexagon exit cost.

**`travel_cost`** — Implements **Dijkstra's algorithm** using a manually built **binary min-heap**. Both physical neighbors and air routes are explored. Hexagons with cost 0 are impassable. Returns -1 if the destination is unreachable.

---

##  Project Structure

```
.
└── movhex4.c   # Single source file containing the entire implementation
