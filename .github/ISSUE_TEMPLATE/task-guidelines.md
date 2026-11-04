## 📋 Project Management & Workflow Guidelines

This repository follows a strict agile workflow optimized for weekly development cycles. All tasks and contributions must align with these estimation and tracking rules.

---

### 🔢 Estimation Guidelines (Time-Based)

The **Estimate** field on the GitHub project board tracks the total effort required to complete a task, measured explicitly in **Ideal Developer Days**.

*   **0.5 Days:** A minor fix, simple code refactor, or documentation update (takes half a day or less).
*   **1.0 Day:** A standard core task, such as implementing a single structural method or basic iterator tests.
*   **2.0 Days:** A complex algorithmic implementation, deep optimization pass, or debugging multifaceted issues.
*   **3.0+ Days:** Too large. Any task estimated at 3 days or more **must be broken down** into smaller, independent sub-issues before being moved to the `Ready` column.

---

### 🔄 Weekly Cycle & Velocity Benchmarking

Our team operates on a **7-day weekly iteration window** running from Monday to Sunday. 

1. **Cycle Planning (Mondays):** Review the `Backlog`, pull high-priority tasks into `Ready`, and ensure every item has a realistic day-based estimate.
2. **In-Flight Limits:** To prevent multitasking overhead, a maximum of **3 days' worth of work** per developer should reside in the `In progress` column at any single time.
3. **Velocity Tracking (Sundays):** At the end of each iteration, calculate the week's **Velocity Score**:
   $$\text{Weekly Velocity} = \sum (\text{Estimate values of items successfully moved to 'Done'})$$
   * Use this benchmark score to accurately gauge capacity and protect planning accuracy for the following week.

---

### 🛠️ Definition of Done (DoD)

An issue cannot be transitioned to the **Done** column unless it meets the following criteria:
* [ ] The code compiles cleanly under target standards with zero warnings.
* [ ] Public container methods are fully guarded with appropriate template concepts.
* [ ] Unit tests pass, covering edge boundaries (e.g., `npos`, empty, and maximum capacities).
* [ ] The change has been pushed, verified via workflow checks, and reviewed under `In review`.
