/*
 * TASK 5 - Write Correct Expressions with the Right Precedence
 * DSA0104 - Object Oriented Programming with C++
 * Student: Dhanush S | Register No: 192511297
 * Faculty: Arunkumar Palthasar | SIMATS Engineering
 *
 * Part A: "Predict the Output" program from assignment (exact copy).
 *         Prediction written in comments BEFORE running.
 * Part B: Peak-hour lookup table alternative.
 *
 * Compile: g++ -std=c++17 -Wall -Wextra -o task05 task05_operators_precedence.cpp
 * Run    : ./task05
 */

#include <iostream>

// ---------------------------------------------------------------
// PART A - Exact program from assignment
// ---------------------------------------------------------------
// MY PREDICTED OUTPUT (written before running):
//
// Line 1: x + y * 2   -> * has higher precedence than +
//           = 5 + (3*2) = 5 + 6 = 11
//         (x + y) * 2 -> parentheses override precedence
//           = (5+3)*2 = 8*2 = 16
//         Expected: "11 16"
//
// Line 2: z = x++ + ++y
//           x++ : post-increment, uses x=5 THEN increments x to 6
//           ++y : pre-increment,  increments y to 4 THEN uses 4
//           z = 5 + 4 = 9
//           After: x=6, y=4
//         Expected: "9 6 4"
//
// Line 3: (x > y ? x : y)  -> x=6, y=4, 6>4 true -> 6
//         x % 4 * 2         -> % and * have equal precedence, left-to-right
//           x=6: 6 % 4 = 2, then 2 * 2 = 4
//         Expected: "6 4"
//
// Line 4: 10 / 3   = 3 (integer division, truncates toward zero)
//         10 % 3   = 1
//        -10 / 3   = -3 (truncates toward zero in C++11+)
//        -10 % 3   = -1 (sign of result = sign of dividend)
//         Expected: "3 1 -3 -1"
//
// Line 5: k starts at 0, i goes 0,3,6,9 (i += 3, stops when i=12>=10)
//         k = 0+3+6+9 = 18
//         Expected: "18"
//
// FULL PREDICTED OUTPUT:
//   11 16
//   9 6 4
//   6 4
//   3 1 -3 -1
//   18
// ---------------------------------------------------------------

void partA_assignmentProgram() {
    std::cout << "=== PART A: Assignment Program (exact) ===\n";

    int x = 5, y = 3;
    std::cout << x + y * 2 << ' ' << (x + y) * 2 << '\n'; // line 1

    int z = x++ + ++y;
    std::cout << z << ' ' << x << ' ' << y << '\n';        // line 2

    std::cout << (x > y ? x : y) << ' ' << x % 4 * 2 << '\n'; // line 3
    std::cout << 10 / 3 << ' ' << 10 % 3 << ' '
              << -10 / 3 << ' ' << -10 % 3 << '\n';        // line 4

    int k = 0;
    for (int i = 0; i < 10; i += 3) k += i;
    std::cout << k << '\n';                                 // line 5
}

// ---------------------------------------------------------------
// PART B - Operator explanation and peak-hour expressions
// ---------------------------------------------------------------
void partB_peakHourExpressions() {
    std::cout << "\n=== PART B: Peak-Hour Expressions ===\n";

    // Correct peak-hour check - with explicit parentheses (best readability)
    // Peak: 08:00 <= hour < 10:00  OR  17:00 <= hour < 20:00
    auto isPeakParen = [](int hour) -> bool {
        return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
    };

    // Equivalent without outer parentheses (harder to read, same semantics)
    // && has higher precedence than ||, so groups correctly already.
    // But adding parentheses is best practice.
    auto isPeakNoParen = [](int hour) -> bool {
        return hour >= 8 && hour < 10 || hour >= 17 && hour < 20;
    };

    // Test boundary hours
    int testHours[] = {7, 8, 9, 10, 16, 17, 19, 20, 21};
    std::cout << "  Hour | With parens | Without parens | Notes\n";
    std::cout << "  " << std::string(55, '-') << '\n';
    for (int h : testHours) {
        std::cout << "   " << h << "   |    " << (isPeakParen(h) ? "PEAK   " : "off-peak")
                  << "  |    " << (isPeakNoParen(h) ? "PEAK   " : "off-peak")
                  << "  | ";
        if (h == 10) std::cout << "10 AM -> OFF-PEAK (< 10, exclusive)";
        if (h == 20) std::cout << "8 PM  -> OFF-PEAK (< 20, exclusive)";
        if (h == 8)  std::cout << "8 AM  -> PEAK (>= 8, inclusive)";
        if (h == 17) std::cout << "5 PM  -> PEAK (>= 17, inclusive)";
        std::cout << '\n';
    }
}

// ---------------------------------------------------------------
// PART C - Lookup table alternative for peak check
// ---------------------------------------------------------------
void partC_lookupTablePeak() {
    std::cout << "\n=== PART C: Lookup Table Alternative ===\n";

    // 24-element bool array: true for peak hours, false otherwise
    // Space: 24 bytes (24 x sizeof(bool))
    // Time:  O(1) - single array index
    bool peakTable[24] = {};
    for (int h = 8;  h < 10; ++h) peakTable[h] = true;  // 8, 9
    for (int h = 17; h < 20; ++h) peakTable[h] = true;  // 17, 18, 19

    std::cout << "  Peak hours (lookup table):\n  ";
    for (int h = 0; h < 24; ++h) {
        if (peakTable[h]) std::cout << h << " ";
    }
    std::cout << '\n';
    std::cout << "  Space used by table: " << sizeof(peakTable) << " bytes\n";

    // Test 10 AM and 8 PM specifically
    std::cout << "  10 AM is peak? " << (peakTable[10] ? "Yes" : "No")
              << " (correct: No)\n";
    std::cout << "  20:00 is peak? " << (peakTable[20] ? "Yes" : "No")
              << " (correct: No)\n";
    std::cout << "   8 AM is peak? " << (peakTable[8]  ? "Yes" : "No")
              << " (correct: Yes)\n";
    std::cout << "  17:00 is peak? " << (peakTable[17] ? "Yes" : "No")
              << " (correct: Yes)\n";
}

// ---------------------------------------------------------------
// PART D - Demonstrate pre vs post increment pitfall
// ---------------------------------------------------------------
void partD_incrementPitfall() {
    std::cout << "\n=== PART D: Pre vs Post Increment ===\n";
    int x{5};
    std::cout << "  x = 5\n";
    std::cout << "  x++ uses then increments: " << x++ << " (x now " << x << ")\n";
    x = 5;
    std::cout << "  ++x increments then uses: " << ++x << " (x now " << x << ")\n";

    std::cout << "\n  WHY i = i++ IS UNDEFINED BEHAVIOUR:\n";
    std::cout << "  The side effect of i++ and the assignment to i both\n";
    std::cout << "  modify i without a sequence point between them.\n";
    std::cout << "  C++ standard: behaviour is undefined (compiler may do anything).\n";
    std::cout << "  Use i++ or ++i standalone; never combine with assignment to itself.\n";
}

int main() {
    std::cout << "=== Task 5: Operators and Precedence ===\n\n";
    partA_assignmentProgram();
    partB_peakHourExpressions();
    partC_lookupTablePeak();
    partD_incrementPitfall();
    return 0;
}
