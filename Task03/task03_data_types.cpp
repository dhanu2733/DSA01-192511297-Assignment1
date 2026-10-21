/*
 * TASK 3 - Choose Data Types and Check Type Compatibility
 * DSA0104 - Object Oriented Programming with C++
 * Student: Dhanush S | Register No: 192511297
 * Faculty: Arunkumar Palthasar | SIMATS Engineering
 *
 * Selects C++ data types for every value in the metro system and
 * demonstrates why money must be stored as integer paise, not
 * floating-point rupees.
 *
 * Compile: g++ -std=c++17 -Wall -Wextra -o task03 task03_data_types.cpp
 * Run    : ./task03
 */

#include <iostream>
#include <climits>    // INT_MAX, LLONG_MAX
#include <cfloat>     // DBL_MAX
#include <iomanip>    // setprecision, fixed

// ---------------------------------------------------------------
// DATA TYPE TABLE FOR THE METRO FARE SYSTEM
// ---------------------------------------------------------------
// Value              Type          Reason
// -------------------------------------------------------
// fare (paise)       long long     Avoids float rounding;
//                                  supports network totals
//                                  (3M journeys x 3600 paise
//                                   = 10,800,000,000 > INT_MAX)
// balance (paise)    long long     Same reasoning as fare
// zone identifier    enum class    Type-safe, named constants
// peak flag          bool          Binary yes/no value
// hour of day        int           Range 0-23, fits in int
// journey count      long long     Up to 3,000,000/day > INT_MAX
// concession %       int           Small positive integer (0-100)
// surcharge %        int           Small positive integer (20)
// ---------------------------------------------------------------

constexpr long long FARE_INNER_PAISE  = 1000LL;
constexpr long long FARE_MIDDLE_PAISE = 2000LL;
constexpr long long FARE_OUTER_PAISE  = 3000LL;
constexpr int       PEAK_SURCHARGE_PC = 20;
constexpr long long MIN_BALANCE_PAISE = 5000LL;

// Demonstrate the rounding error when using double for money
void demonstrateDoublePrecisionError() {
    std::cout << "=== Double Precision Problem ===\n";

    // Simulate 20,000 journeys at Rs 12 (Rs 10 + 20% peak) = Rs 12.00
    // Using double (rupees):
    double totalDouble = 0.0;
    for (int i = 0; i < 20000; ++i) {
        totalDouble += 12.00;
    }

    // Using long long (paise):
    long long totalPaise = 0LL;
    for (int i = 0; i < 20000; ++i) {
        totalPaise += 1200LL; // Rs 12.00 = 1200 paise
    }

    double expectedDouble = 20000.0 * 12.00;
    long long expectedPaise = 20000LL * 1200LL;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "  After 20,000 journeys at Rs 12.00:\n";
    std::cout << "  double total    : Rs " << totalDouble
              << "  (expected: " << expectedDouble << ")\n";
    std::cout << "  Error in double : " << (totalDouble - expectedDouble) << " Rs\n";
    std::cout << "  long long total : " << totalPaise << " paise = Rs "
              << totalPaise / 100 << '\n';
    std::cout << "  Error in paise  : " << (totalPaise - expectedPaise) << " paise (EXACT)\n\n";
}

// Demonstrate overflow risk with int for network totals
void demonstrateIntOverflow() {
    std::cout << "=== int Overflow for Network Total ===\n";
    std::cout << "  INT_MAX          = " << INT_MAX << '\n';
    std::cout << "  3,000,000 journeys x 3000 paise (max fare) = "
              << 3000000LL * 3000LL << " paise\n";
    std::cout << "  " << 3000000LL * 3000LL << " > INT_MAX (" << INT_MAX << ")? "
              << (3000000LL * 3000LL > INT_MAX ? "YES - int OVERFLOWS!" : "No") << '\n';
    std::cout << "  LLONG_MAX        = " << LLONG_MAX << '\n';
    std::cout << "  long long safely holds the network total.\n\n";
}

// Show sizeof and ranges for all types used
void showTypeSizes() {
    std::cout << "=== Data Type Sizes on This System ===\n";
    std::cout << std::left << std::setw(20) << "Type"
              << std::setw(12) << "sizeof"
              << "Range / Notes\n";
    std::cout << std::string(60, '-') << '\n';
    std::cout << std::setw(20) << "bool"
              << std::setw(12) << sizeof(bool)
              << "0 (false) or 1 (true)\n";
    std::cout << std::setw(20) << "char"
              << std::setw(12) << sizeof(char)
              << "-128 to 127 (or 0 to 255 unsigned)\n";
    std::cout << std::setw(20) << "int"
              << std::setw(12) << sizeof(int)
              << INT_MIN << " to " << INT_MAX << '\n';
    std::cout << std::setw(20) << "long long"
              << std::setw(12) << sizeof(long long)
              << LLONG_MIN << " to " << LLONG_MAX << '\n';
    std::cout << std::setw(20) << "float"
              << std::setw(12) << sizeof(float)
              << "~7 significant decimal digits (NOT for money)\n";
    std::cout << std::setw(20) << "double"
              << std::setw(12) << sizeof(double)
              << "~15 significant decimal digits (NOT for money)\n";
    std::cout << std::setw(20) << "unsigned int"
              << std::setw(12) << sizeof(unsigned int)
              << "0 to " << UINT_MAX << '\n';
    std::cout << '\n';
}

// Demonstrate type compatibility in mixed expressions
void demonstrateTypeCompatibility() {
    std::cout << "=== Type Compatibility in Mixed Expressions ===\n";

    int hour  = 9;       // int: hours 0-23
    bool peak = (hour >= 8 && hour < 10); // bool: result of comparison

    // Mixing int and bool: bool promotes to int (0 or 1)
    int val = peak + 5;  // peak(true=1) + 5 = 6
    std::cout << "  peak(bool=true) + 5(int) = " << val
              << "  [bool promoted to int 1]\n";

    // Mixing int and double: int widened to double
    long long fareP = FARE_OUTER_PAISE;
    double fareD    = static_cast<double>(fareP) / 100.0; // explicit, safe
    std::cout << "  long long " << fareP << " / 100.0 = Rs " << fareD
              << "  [explicit cast, no information loss]\n";

    // Integer division (common mistake)
    int a = 7, b = 2;
    std::cout << "  7 / 2 (int/int)    = " << a / b
              << "  [truncated, NOT 3.5!]\n";
    std::cout << "  7.0 / 2 (dbl/int)  = " << 7.0 / b
              << "  [int widened to double]\n\n";
}

int main() {
    std::cout << "=== Task 3: Data Types and Type Compatibility ===\n\n";

    showTypeSizes();
    demonstrateDoublePrecisionError();
    demonstrateIntOverflow();
    demonstrateTypeCompatibility();

    // ---- Justify each type choice ----
    std::cout << "=== Justified Type Choices for Metro System ===\n";
    std::cout << "  Fare / balance   -> long long paise : avoids float error, handles network total\n";
    std::cout << "  Zone             -> enum class Zone  : type-safe named constants\n";
    std::cout << "  Peak flag        -> bool             : binary condition\n";
    std::cout << "  Hour of day      -> int              : range 0-23, fits comfortably\n";
    std::cout << "  Journey count    -> long long        : 3,000,000 > INT_MAX\n";
    std::cout << "  Concession pct   -> int              : small value 0-100\n";
    std::cout << "  Card ID          -> std::string      : alphanumeric identifier\n";

    return 0;
}
