/*
 * TASK 2 - Use Tokens, Keywords, Identifiers and Constants Correctly
 * DSA0104 - Object Oriented Programming with C++
 * Student: Dhanush S | Register No: 192511297
 * Faculty: Arunkumar Palthasar | SIMATS Engineering
 *
 * Demonstrates the five kinds of tokens, the difference between
 * #define, const, and constexpr, and enum vs enum class for zones.
 *
 * Compile: g++ -std=c++17 -Wall -Wextra -o task02 task02_tokens_constants.cpp
 * Run    : ./task02
 */

#include <iostream>
#include <climits>   // INT_MAX, LLONG_MAX
#include <typeinfo>  // typeid (for demonstration)

// ---------------------------------------------------------------
// FIVE KINDS OF TOKENS (demonstrated throughout this file)
// 1. Keywords     : int, class, constexpr, enum, return, if, ...
// 2. Identifiers  : FARE_INNER_PAISE, Zone, SmartCard, balance_, ...
// 3. Constants    : 1000LL, 20, 5000LL, 'A', "hello"
// 4. Operators    : +, -, *, /, %, =, ==, <, >, &&, ||, ::, ...
// 5. Punctuators  : ; , { } ( ) [ ] :: ...
// ---------------------------------------------------------------

// ---------------------------------------------------------------
// #define macro - NO type checking, NO scope, text substitution only
// Avoided in modern C++ because:
//   - The preprocessor replaces it before compilation (no type safety)
//   - It leaks into every file that includes this header
//   - Debuggers cannot see the symbolic name
// ---------------------------------------------------------------
#define DEFINE_FARE_INNER  1000   // Rs 10 in paise (macro - no type)

// ---------------------------------------------------------------
// const - runtime constant, has type and scope, stored in memory
//   (The compiler may optimise it away, but it CAN occupy storage.)
// ---------------------------------------------------------------
const int CONST_FARE_MIDDLE = 2000;  // Rs 20 in paise

// ---------------------------------------------------------------
// constexpr - compile-time constant, guaranteed to be evaluated at
//   compile time, strongly typed, scoped, preferred for fare tables.
//   The compiler NEVER stores it in runtime memory unless its
//   address is taken; it is inlined as a literal wherever used.
// ---------------------------------------------------------------
constexpr long long FARE_INNER_PAISE   = 1000LL; // Rs 10
constexpr long long FARE_MIDDLE_PAISE  = 2000LL; // Rs 20
constexpr long long FARE_OUTER_PAISE   = 3000LL; // Rs 30
constexpr int       PEAK_SURCHARGE_PC  = 20;     // 20 % surcharge
constexpr long long MIN_BALANCE_PAISE  = 5000LL; // Rs 50 minimum

// ---------------------------------------------------------------
// Plain enum - values leak into the enclosing scope; risky
//   because Zone::Inner == 0 compares equal to any int 0.
// ---------------------------------------------------------------
enum ZonePlain { ZP_Inner = 0, ZP_Middle = 1, ZP_Outer = 2 };

// ---------------------------------------------------------------
// enum class - scoped, strongly typed; Zone::Inner cannot be
//   implicitly compared with an int; safer and more readable.
// PREFERRED for zones in this system.
// ---------------------------------------------------------------
enum class Zone { Inner = 0, Middle = 1, Outer = 2 };

// ---------------------------------------------------------------
// Identifier naming conventions used in this project
//   ALL_CAPS_WITH_UNDERSCORES  -> compile-time constants / macros
//   PascalCase                 -> class and enum class names
//   camelCase                  -> local variables, function params
//   trailingUnderscore_        -> private member variables (class)
//   verb_noun()                -> function names (computeFare)
// ---------------------------------------------------------------

// Helper: zone name for display
const char* zoneName(Zone z) {
    switch (z) {
        case Zone::Inner:  return "Inner";
        case Zone::Middle: return "Middle";
        case Zone::Outer:  return "Outer";
        default:           return "Unknown";
    }
}

// Compute fare using constexpr constants (preferred approach)
long long computeFare(Zone z, bool peak) {
    long long base = 0;
    switch (z) {
        case Zone::Inner:  base = FARE_INNER_PAISE;  break;
        case Zone::Middle: base = FARE_MIDDLE_PAISE; break;
        case Zone::Outer:  base = FARE_OUTER_PAISE;  break;
    }
    if (peak) {
        base += (base * PEAK_SURCHARGE_PC) / 100; // integer arithmetic, exact
    }
    return base;
}

int main() {
    std::cout << "=== Task 2: Tokens, Keywords, Identifiers and Constants ===\n\n";

    // ---- Token examples ----
    std::cout << "-- Five Token Kinds --\n";
    std::cout << "  Keywords    : int, constexpr, enum, class, return, if\n";
    std::cout << "  Identifiers : FARE_INNER_PAISE, Zone, computeFare, base\n";
    std::cout << "  Constants   : 1000LL, 20, 5000LL (integer literals)\n";
    std::cout << "  Operators   : +, -, *, /, %, =, ==, &&, ||\n";
    std::cout << "  Punctuators : ; { } ( ) [ ] :: ,\n\n";

    // ---- #define vs const vs constexpr ----
    std::cout << "-- Constant Comparison --\n";
    std::cout << "  #define DEFINE_FARE_INNER  = " << DEFINE_FARE_INNER
              << "  (no type, no scope, text-substitution)\n";
    std::cout << "  const   CONST_FARE_MIDDLE  = " << CONST_FARE_MIDDLE
              << "  (typed, scoped, may occupy memory)\n";
    std::cout << "  constexpr FARE_OUTER_PAISE = " << FARE_OUTER_PAISE
              << "  (compile-time, typed, scoped, inline)\n\n";

    // ---- enum vs enum class ----
    std::cout << "-- enum vs enum class --\n";
    int plainVal = ZP_Inner; // plain enum leaks: ZP_Inner converts to int
    std::cout << "  Plain enum ZP_Inner as int : " << plainVal << " (implicit conversion - risky)\n";

    // enum class Zone::Inner cannot be implicitly converted to int
    Zone z = Zone::Outer;
    std::cout << "  enum class Zone::Outer name: " << zoneName(z) << " (type-safe)\n\n";

    // ---- Fare computation using constexpr constants ----
    std::cout << "-- Fare Table (constexpr constants) --\n";
    for (int zoneIdx = 0; zoneIdx <= 2; ++zoneIdx) {
        Zone zone = static_cast<Zone>(zoneIdx);
        long long offPeak = computeFare(zone, false);
        long long onPeak  = computeFare(zone, true);
        std::cout << "  Zone " << zoneName(zone)
                  << " | Off-peak: Rs " << offPeak / 100
                  << " | Peak (+20%): Rs " << onPeak / 100 << '\n';
    }

    std::cout << "\n-- Why constexpr is preferred for fare table --\n";
    std::cout << "  1. Evaluated at compile time -> zero runtime overhead\n";
    std::cout << "  2. Strongly typed -> compiler catches type mismatches\n";
    std::cout << "  3. Scoped -> no accidental name collisions\n";
    std::cout << "  4. Debugger-visible -> unlike #define macros\n";
    std::cout << "  5. Inline at use site -> no memory allocation needed\n";

    std::cout << "\n-- Memory sizes on this system --\n";
    std::cout << "  sizeof(int)       = " << sizeof(int)       << " bytes\n";
    std::cout << "  sizeof(long long) = " << sizeof(long long) << " bytes\n";
    std::cout << "  sizeof(bool)      = " << sizeof(bool)      << " bytes\n";
    std::cout << "  sizeof(double)    = " << sizeof(double)    << " bytes\n";
    std::cout << "  INT_MAX           = " << INT_MAX           << '\n';
    std::cout << "  LLONG_MAX         = " << LLONG_MAX         << '\n';

    return 0;
}
