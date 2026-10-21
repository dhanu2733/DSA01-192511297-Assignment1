# DSA01-192511297-Assignment1

## Namma Metro Smart-Card Fare System

| Field              | Detail                                      |
|--------------------|---------------------------------------------|
| **Name**           | Dhanush S                                   |
| **Register Number**| 192511297                                   |
| **Email**          | 192511297.simats@saveetha.com               |
| **Course**         | DSA0104 – Object Oriented Programming with C++ |
| **Faculty**        | Arunkumar Palthasar                         |
| **Institution**    | SIMATS Engineering                          |
| **Assignment**     | Assignment 1 – CO1                          |
| **Compiler**       | g++ (GCC) 13 (MinGW-w64 / MSYS2)           |
| **Compile flags**  | `g++ -std=c++17 -Wall -Wextra`              |

---

## Repository Structure

```
DSA01-192511297-Assignment1/
├── Task01/   task01_oop_analysis.cpp          (OOP Analysis)
├── Task02/   task02_tokens_constants.cpp      (Tokens & Constants)
├── Task03/   task03_data_types.cpp            (Data Types)
├── Task04/   task04_implicit_conversions.cpp  (Implicit Conversions)
├── Task05/   task05_operators_precedence.cpp  (Operators & Precedence)
├── Task06/   task06_control_structures.cpp    (Control Structures – Complete the Code)
├── Task07/   task07_proc_vs_oop.cpp           (Procedural vs OOP)
├── Task08/   task08_io.cpp                    (Safe I/O & Formatting)
├── Task09/   task09_implementation_test.cpp   (Full Implementation & Testing)
├── Task10/   task10_expectation_vs_execution.cpp (Expectation vs Execution)
├── Task11/   task11_final_design.cpp          (Final Design Recommendation)
├── Task12/   task12_design_challenge.cpp      (Day-4 Design Challenge)
└── README.md
```

---

## How to Compile and Run

> **Prerequisite:** Install [MinGW-w64](https://www.mingw-w64.org/) or [MSYS2](https://www.msys2.org/) for g++ on Windows.

### Compile a single task
```bash
g++ -std=c++17 -Wall -Wextra -o taskXX taskXX_<name>.cpp
./taskXX
```

### Example – Task 6 (Complete the Code)
```bash
cd Task06
g++ -std=c++17 -Wall -Wextra -o task06 task06_control_structures.cpp
./task06
```

### Compile and run all tasks (PowerShell)
```powershell
1..12 | ForEach-Object {
    $n = "Task{0:D2}" -f $_
    $f = Get-ChildItem "$n\*.cpp" | Select-Object -First 1
    if ($f) {
        g++ -std=c++17 -Wall -Wextra -o "$n\task" $f.FullName
        & "$n\task"
    }
}
```

---

## Task Summary & Results

| Task | Title | Marks | Key Evidence |
|------|-------|-------|-------------|
| Task 01 | Object-Oriented Analysis | 4 | SmartCard, Passenger, Journey, Zone, FareTable classes with OOP principles demonstrated |
| Task 02 | Tokens & Constants | 3 | All 5 token types shown; constexpr preferred over #define; enum class vs plain enum |
| Task 03 | Data Types & Compatibility | 4 | long long paise chosen; double rounding error measured; sizeof table printed |
| Task 04 | Implicit Conversions | 4 | Predicted output: `3 3.5 / C 68 / 1 true / s >= u`; corrected with static_cast |
| Task 05 | Operators & Precedence | 4 | Predicted output: `11 16 / 9 6 4 / 6 4 / 3 1 -3 -1 / 18`; lookup table alternative |
| Task 06 | Control Structures | 4 | TODO 1 & TODO 2 completed; boundary: 10 AM = off-peak, 8 PM = off-peak |
| Task 07 | Procedural vs OOP | 4 | Two working versions; OOP prevents negative balance; procedural cannot |
| Task 08 | Safe I/O | 4 | cin.fail() recovery; setw/setfill/fixed/setprecision; neat journey table |
| Task 09 | Implementation & Testing | 4 | 14-case test table (all PASS); 5 hand-calculated journeys verified |
| Task 10 | Expectation vs Execution | 4 | double error at 3M journeys; int overflow at 3M×3600 paise; long long exact |
| Task 11 | Final Recommendation | 4 | long long + constexpr + enum class + SmartCard; correct for 3M journeys |
| Task 12 | Design Challenge (Day-4) | 8 | Airport zone (Rs 60); weekend discount (10%); MonthlyPass class; all tests pass |

**Total: 50 / 50**

---

## Predicted Output (Tasks 4 & 5)

### Task 4 – Implicit Conversions
```
3 3.5
C 68
1 true
s >= u
```
**Explanation:**
- Line 1: `a/b` is integer division (7/2=3), widened to double 3.0; `static_cast<double>(a)/b` = 3.5
- Line 2: `'A'+2` = char 67 ('C'); `ch+1` promotes char to int → 68
- Line 3: `bool flag = 5` → non-zero int becomes true (1); boolalpha → "true"
- Line 4: `-1` (signed) compared with `1` (unsigned) → `-1` converts to UINT_MAX → `s >= u`

### Task 5 – Operators and Precedence
```
11 16
9 6 4
6 4
3 1 -3 -1
18
```
**Explanation:**
- Line 1: `*` before `+`: `5 + 3*2 = 11`; parens override: `(5+3)*2 = 16`
- Line 2: `x++=5` (post), `++y=4` (pre); `z=9`, `x=6`, `y=4`
- Line 3: `6>4` → 6; `6%4=2`, `2*2=4` (left-to-right, equal precedence)
- Line 4: Integer division truncates toward zero; `-10%3 = -1` (sign = sign of dividend)
- Line 5: `i` = 0,3,6,9 → sum = 18

---

## Key Design Decisions

1. **Money in integer paise** – Avoids IEEE-754 rounding error. `long long` handles 3M journeys × 6000 paise = 18 billion paise (within LLONG_MAX).
2. **`constexpr` constants** – Compile-time, type-safe, scoped, inlined; no runtime overhead.
3. **`enum class Zone`** – Strongly typed; prevents implicit comparison with `int`.
4. **`SmartCard::balance_` private** – The Rs-50 floor invariant is guaranteed by the class; no external code can violate it.
5. **`FareTable` separate from `SmartCard`** – Fare rules can change independently of card logic.
6. **Peak boundary rule** – `hour >= 8 && hour < 10`: 10 AM is **off-peak**; 8 PM is **off-peak** (strict less-than).

---

## Complexity Analysis Summary

| Operation | Time | Space | Notes |
|-----------|------|-------|-------|
| `computeFare()` | O(1) | O(1) | Switch on 3–4 zones |
| `canTravel()` | O(1) | O(1) | Single comparison |
| `deductFare()` | O(1) | O(1) | Subtraction + check |
| `isPeakHour()` | O(1) | O(1) | Two boolean expressions |
| Test loop (n cases) | O(n) | O(n) | Table-driven testing |
| Network total (n journeys) | O(n) | O(1) | Single accumulator |

---

*GitHub Repository: [DSA01-192511297-Assignment1](https://github.com/dhanu2733/DSA01-192511297-Assignment1)*
