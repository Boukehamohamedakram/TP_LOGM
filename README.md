# Logic Solver

<div align="center">
  <img src="Pictures/ESI_Logo.png" alt="Logic Solver Logo" width="200">
  <h3>Advanced Propositional Logic Solver with Resolution Refutation</h3>
  <p>Developed at École nationale Supérieure d'Informatique (ESI)</p>
</div>

---

## 📖 Overview

Logic Solver is an advanced educational tool for analyzing and verifying propositional logic formulas expressed in **Conjunctive Normal Form (CNF)**. Using the **resolution by refutation** method introduced by **J.A. Robinson in 1965**, this tool allows users to create, manipulate, and test logic formulas through a simple graphical interface.

<div align="center">
  <img src="Pictures/capture.png" alt="Logic Solver Screenshot" width="600">
</div>

---

## ✨ Features

- 📝 **Create and manage logic formulas** in CNF format
- 🔍 **Test satisfiability** using resolution by refutation
- 👁️ **Preview formulas** in clean logical notation (∧, ∨, ¬)
- 📁 **Organize formulas** in a local collection
- 🔄 **Automatic CNF conversion** from general logic expressions
- 🧠 **Optimized resolution engine** with:
  - Unit clause propagation
  - Tautology detection
  - Pure literal elimination
  - Subsumption simplification

---

## 🚀 Getting Started

### Prerequisites

- 🖥️ Windows operating system (64-bit recommended)
- 📦 No additional software or dependencies needed

### Installation

1. Download the latest version from the [Releases](https://github.com/yourusername/logic-solver/releases) page
2. Extract the ZIP archive
3. Make sure these files are in the same directory:
   - `gui_solver.exe`
   - `logic_solver.exe`
   - (Optional) `input_generator.exe` for terminal-only usage
4. Run `gui_solver.exe` to launch the interface

---

## 📚 How to Use

### ➕ Creating a Formula

1. Click “Create New Formula” in the main menu
2. Enter a name for the file (no `.cnf` needed)
3. Specify the number of clauses
4. Enter literals using:
   - Space-separated format: `P !Q R`
   - `!` for NOT (¬)
   - Example: `!A B` → (¬A ∨ B)

### 🧪 Testing a Formula

1. Click “Test Existing Formula”
2. Select a `.cnf` file
3. A popup will show the result:
   - `SATISFIABLE`: At least one interpretation satisfies the formula
   - `UNSATISFIABLE`: No interpretation satisfies the formula

### 📂 Viewing and Editing Formulas

- Use “Show Available Formulas” to list and read existing `.cnf` files
- Click on a file to view and test its content

---

## 🧠 How It Works

### Resolution by Refutation (Robinson, 1965)

Logic Solver works by attempting to derive an **empty clause** through resolution:

1. Read CNF clauses from file
2. Identify pairs of complementary literals (e.g., `P` and `!P`)
3. Combine clauses into **resolvents**
4. Repeat until:
   - An empty clause is derived → `UNSATISFIABLE`
   - No further resolutions are possible → `SATISFIABLE`

### Optimization Techniques

- **Unit Propagation**: Apply early resolution to unit clauses (1-literal clauses)
- **Tautology Elimination**: Skip clauses with both `X` and `!X`
- **Subsumption**: Remove redundant supersets of existing clauses
- **Pure Literal Elimination**: Eliminate literals appearing with one polarity

---

## 🌟 Examples

### 🧨 Example 1: Contradiction

File: `contradiction.cnf`
