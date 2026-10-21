/*
 * TASK 10 - Compare Expectations with Execution
 * DSA0104 - Object Oriented Programming with C++
 * Student: Dhanush S | Register No: 192511297
 * Faculty: Arunkumar Palthasar | SIMATS Engineering
 *
 * Measures cumulative totals for 20,000 / 500,000 / 3,000,000
 * journeys using double (rupees) vs long long (paise), shows
 * the rounding error, and checks int overflow.
 *
 * Compile: g++ -std=c++17 -Wall -Wextra -o task10 task10_expectation_vs_execution.cpp
 * Run    : ./task10
 */

#include <iostream>
#include <iomanip>
#include <climits>
#include <cmath>      // std::fabs

// ---------------------------------------------------------------
// Simulate n journeys; each journey is Rs 12 (Inner zone peak)
// = 1200 paise = Rs 12.00
// Expected exact total: n * 1200 paise = n * 12 rupees
// ---------------------------------------------------------------

constexpr long long FARE_PAISE   = 1200LL; // Rs 12.00 (Inner + 20% surcharge)
constexpr double    FARE_RUPEES  = 12.00;

void simulateJourneys(long long n) {
    // Using double (rupees) - floating-point accumulation
    double totalDouble = 0.0;
    for (long long i = 0; i < n; ++i) {
        totalDouble += FARE_RUPEES;
    }

    // Using long long (paise) - integer accumulation, exact
    long long totalPaise = n * FARE_PAISE;

    double expectedRupees = static_cast<double>(n) * FARE_RUPEES;
    double actualPaiseAsRupees = static_cast<double>(totalPaise) / 100.0;
    double error = std::fabs(totalDouble - expectedRupees);

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "  n = " << std::setw(12) << n << " journeys\n";
    std::cout << "    double total  : Rs " << totalDouble
              << " | expected Rs " << expectedRupees
              << " | error: " << error << " Rs\n";
    std::cout << "    paise total   : " << totalPaise
              << " paise = Rs " << actualPaiseAsRupees
              << " | error: 0 (EXACT)\n";

    // Check if int would overflow
    bool intOverflows = (n * FARE_PAISE) > static_cast<long long>(INT_MAX);
    std::cout << "    int overflow? : " << (intOverflows ? "YES (use long long!)" : "No") << '\n';
    std::cout << '\n';
}

// ---------------------------------------------------------------
// Show rounding error growth with n additions
// ---------------------------------------------------------------
void showRoundingGrowth() {
    std::cout << "=== Rounding Error Growth (double) ===\n";
    std::cout << "  Each addition of 12.00 may introduce a tiny IEEE-754 error.\n";
    std::cout << "  Error after n journeys (measured):\n\n";

    long long counts[] = {20000LL, 500000LL, 3000000LL};
    for (long long n : counts) {
        double sum = 0.0;
        for (long long i = 0; i < n; ++i) sum += 12.00;
        double expected = static_cast<double>(n) * 12.00;
        std::cout << std::fixed << std::setprecision(8);
        std::cout << "  n=" << std::setw(9) << n
                  << " | double error: " << (sum - expected) << " Rs\n";
    }
    std::cout << '\n';
}

// ---------------------------------------------------------------
// Demonstrate int overflow for network total
// ---------------------------------------------------------------
void intOverflowDemo() {
    std::cout << "=== int vs long long vs double at 3,000,000 journeys ===\n";
    const long long N     = 3000000LL;
    const long long FPAISE = 3600LL; // max fare: Outer peak

    std::cout << "  Each journey fare: " << FPAISE << " paise (Rs 36, Outer peak)\n";
    std::cout << "  3,000,000 x 3600 paise = " << (N * FPAISE) << " paise\n\n";

    // int overflow
    // int total = (int)(3000000 * 3600) = 10,800,000,000 > INT_MAX (2,147,483,647)
    long long exact = N * FPAISE;
    std::cout << "  INT_MAX              = " << INT_MAX << '\n';
    std::cout << "  Exact total (paise)  = " << exact << '\n';
    std::cout << "  Fits in int?         : " << (exact <= INT_MAX ? "YES" : "NO - OVERFLOW!") << '\n';
    std::cout << "  Fits in long long?   : " << (exact <= LLONG_MAX ? "YES" : "NO") << '\n';

    // Simulating int overflow (using modular arithmetic to show wrapped value)
    // We do NOT actually overflow int to avoid UB; just compute modulo
    long long wrappedInt = exact % (static_cast<long long>(INT_MAX) + 1LL);
    if (exact > INT_MAX) {
        std::cout << "  int wrapped value    ~ " << static_cast<int>(exact) // UB in C++, shown for illustration
                  << " (incorrect - data loss)\n";
    }

    // double loss of precision
    double dTotal = static_cast<double>(N) * static_cast<double>(FPAISE);
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  double total (paise) = " << dTotal
              << " (may lose ~1-2 digits of precision for large values)\n";
    std::cout << "  Exact long long      = " << exact
              << " (always correct)\n\n";

    (void)wrappedInt; // suppress unused warning
}

// ---------------------------------------------------------------
// Compiler warnings summary
// ---------------------------------------------------------------
void compilerWarningsSummary() {
    std::cout << "=== Compiler Warnings with -Wall -Wextra ===\n";
    std::cout << "  Warning 1: signed/unsigned comparison\n";
    std::cout << "    unsigned int u = 1; int s = -1; if (s < u) ...\n";
    std::cout << "    Fix: cast both to long long before comparing.\n\n";
    std::cout << "  Warning 2: implicit int-to-bool conversion\n";
    std::cout << "    bool flag = 5; -> use bool flag = (5 != 0);\n\n";
    std::cout << "  Warning 3: unused variable\n";
    std::cout << "    int x; (never read) -> remove or use (void)x;\n\n";
    std::cout << "  Warning 4: narrowing in brace init\n";
    std::cout << "    int x{3.7}; -> compiler rejects with error.\n\n";
    std::cout << "  Warning 5: integer division assigned to double\n";
    std::cout << "    double d = 7/2; -> use static_cast<double>(7)/2;\n";
}

int main() {
    std::cout << "=== Task 10: Expectation vs Execution ===\n\n";

    std::cout << "=== Journey Total Comparison (double vs long long) ===\n";
    simulateJourneys(20000LL);
    simulateJourneys(500000LL);
    simulateJourneys(3000000LL);

    showRoundingGrowth();
    intOverflowDemo();
    compilerWarningsSummary();

    return 0;
}
