/*
 * TASK 4 - Control Implicit Conversions
 * DSA0104 - Object Oriented Programming with C++
 * Student: Dhanush S | Register No: 192511297
 * Faculty: Arunkumar Palthasar | SIMATS Engineering
 *
 * Part A: "Predict the Output" program from the assignment (exact copy).
 *         Prediction is written in comments BEFORE the code runs.
 * Part B: Corrected version using static_cast and brace-initialisation.
 *
 * Compile: g++ -std=c++17 -Wall -Wextra -o task04 task04_implicit_conversions.cpp
 * Run    : ./task04
 */

#include <iostream>

// ---------------------------------------------------------------
// PART A - Exact program from assignment (as-is)
// ---------------------------------------------------------------
// MY PREDICTED OUTPUT (written before running):
//
//   Line 1: c = a/b integer division -> 7/2 = 3, stored as double 3.0 -> prints 3
//            d = (double)7 / 2 = 3.5 -> prints 3.5
//            Expected: "3 3.5"
//
//   Line 2: ch = 'A' + 2 = 65 + 2 = 67 -> char 'C'
//            ch + 1 = 67 + 1 = 68 -> int (char promoted to int) -> prints 68
//            Expected: "C 68"
//
//   Line 3: bool flag = 5 -> any non-zero int becomes true(1)
//            Without boolalpha -> prints 1
//            With boolalpha    -> prints "true"
//            Expected: "1 true"
//
//   Line 4: unsigned int u = 1; int s = -1;
//            s < u: -1 compared with unsigned 1.
//            -1 converts to unsigned -> very large number (UINT_MAX)
//            UINT_MAX >= 1, so s >= u -> prints "s >= u"
//            Expected: "s >= u"
//
// FULL PREDICTED OUTPUT:
//   3 3.5
//   C 68
//   1 true
//   s >= u
// ---------------------------------------------------------------

void partA_assignmentProgram() {
    std::cout << "=== PART A: Assignment Program (exact) ===\n";
    int a = 7, b = 2;
    double c = a / b;                         // integer division: 3, widened to 3.0
    double d = static_cast<double>(a) / b;    // 7.0 / 2 = 3.5
    std::cout << c << ' ' << d << '\n';       // line 1

    char ch = 'A' + 2;                        // 'A'=65 + 2 = 67 = 'C'
    std::cout << ch << ' ' << ch + 1 << '\n'; // line 2: 'C' then 68 (int promotion)

    bool flag = 5;                            // int 5 -> bool true (non-zero)
    std::cout << flag << ' ' << std::boolalpha << flag << '\n'; // line 3

    unsigned int u = 1;
    int s = -1;
    std::cout << (s < u ? "s < u" : "s >= u") << '\n'; // line 4: signed-to-unsigned surprise
}

// ---------------------------------------------------------------
// PART B - Corrected version with explicit casts and brace-init
// ---------------------------------------------------------------
// Explanation of each conversion in PART A:
//
// Line 1 - INTEGER DIVISION: a/b performs int division (result=3),
//   then the int 3 is widened to double 3.0 (implicit widening).
//   d uses static_cast<double> before division -> true real division.
//
// Line 2 - CHAR-TO-INT PROMOTION: 'A' is char (ASCII 65). 'A'+2
//   promotes 'A' to int before addition -> 67 -> assigned back to
//   char ch. ch+1 promotes ch to int again -> 68 is printed as int.
//
// Line 3 - INT-TO-BOOL: bool flag = 5 -> any non-zero int becomes
//   true (1). Without boolalpha prints the int 1; with boolalpha
//   prints the word "true".
//
// Line 4 - SIGNED-TO-UNSIGNED: When signed and unsigned values are
//   compared, the signed value is implicitly converted to unsigned.
//   -1 as unsigned int = 4294967295 (all bits set = UINT_MAX).
//   UINT_MAX >= 1 is true, so the condition s < u is FALSE.
//   This is a classic C++ gotcha flagged by -Wall.

void partB_correctedVersion() {
    std::cout << "\n=== PART B: Corrected Version (explicit casts) ===\n";

    int a{7}, b{2};                           // brace-init: narrows forbidden
    // Explicit cast avoids integer-division surprise
    double c{ static_cast<double>(a) / b };
    double d{ static_cast<double>(a) / b };
    std::cout << c << ' ' << d << '\n';       // both now 3.5

    char ch{ static_cast<char>('A' + 2) };   // explicit: int -> char
    int  chPlus1{ static_cast<int>(ch) + 1 };
    std::cout << ch << ' ' << chPlus1 << '\n';

    bool flag{ a != 0 };                      // explicit boolean expression
    std::cout << flag << ' ' << std::boolalpha << flag << '\n';

    // Correct signed/unsigned comparison: cast to common signed type
    unsigned int u{1};
    int s{-1};
    if (static_cast<long long>(s) < static_cast<long long>(u)) {
        std::cout << "s < u (correct with explicit cast)\n";
    } else {
        std::cout << "s >= u\n";
    }
}

// ---------------------------------------------------------------
// PART C - Demonstrate narrowing conversions and brace-init safety
// ---------------------------------------------------------------
void partC_narrowingDemo() {
    std::cout << "\n=== PART C: Narrowing Conversions ===\n";

    // int x{3.7};  // ERROR at compile time: narrowing conversion disallowed
    // This line would NOT compile with brace initialisation - that's the POINT.
    // Old-style: int x = 3.7; -> silently truncates to 3 (dangerous).

    int x = static_cast<int>(3.7); // explicit truncation - deliberate
    std::cout << "  3.7 -> int (static_cast): " << x << "  (truncated, explicit)\n";

    // Brace-init with compatible types (no narrowing -> fine)
    int   balancePaise{10000};
    long long bigBalance{static_cast<long long>(balancePaise)};
    std::cout << "  int " << balancePaise
              << " -> long long " << bigBalance << " (widening, safe)\n";

    std::cout << "\n  Summary of conversion rules:\n";
    std::cout << "  - Integer division truncates toward zero (7/2 = 3)\n";
    std::cout << "  - int -> double: widening, no data loss\n";
    std::cout << "  - double -> int: narrowing, truncation (brace-init REJECTS this)\n";
    std::cout << "  - any non-zero int -> bool: becomes true\n";
    std::cout << "  - signed int -> unsigned: reinterprets bit pattern (surprise!)\n";
    std::cout << "  - char -> int: char is promoted; ASCII value used\n";
}

int main() {
    std::cout << "=== Task 4: Control Implicit Conversions ===\n\n";
    partA_assignmentProgram();
    partB_correctedVersion();
    partC_narrowingDemo();
    return 0;
}
