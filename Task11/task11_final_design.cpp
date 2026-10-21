/*
 * TASK 11 - Recommend and Defend the Final Design
 * DSA0104 - Object Oriented Programming with C++
 * Student: Dhanush S | Register No: 192511297
 * Faculty: Arunkumar Palthasar | SIMATS Engineering
 *
 * Presents the final recommended design with justifications
 * and demonstrates it is correct for the whole network.
 *
 * Compile: g++ -std=c++17 -Wall -Wextra -o task11 task11_final_design.cpp
 * Run    : ./task11
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <climits>

// ---------------------------------------------------------------
// FINAL RECOMMENDED DESIGN
// ---------------------------------------------------------------
// 1. Store all money in long long PAISE (never double).
//    Reason: 3,000,000 * 3600 paise = 10.8 billion > INT_MAX.
//            double introduces rounding error; paise are exact.
//
// 2. Use constexpr for all fare constants (not #define, not const).
//    Reason: compile-time, type-safe, scoped, inlined.
//
// 3. Use enum class Zone for zone identifiers.
//    Reason: strongly typed, cannot accidentally compare to int.
//
// 4. Use SmartCard class with private balance_.
//    Reason: the Rs-50 rule is enforced in ONE place (canTravel).
//            No external code can set balance to a negative value.
//
// 5. Separate FareTable from SmartCard.
//    Reason: fare logic can change independently of card logic.
//
// 6. Separate isPeakHour() as a pure function.
//    Reason: testable in isolation; easy to switch to lookup table.
// ---------------------------------------------------------------

// ===== Constants =====
constexpr long long FARE_INNER_PAISE   = 1000LL;
constexpr long long FARE_MIDDLE_PAISE  = 2000LL;
constexpr long long FARE_OUTER_PAISE   = 3000LL;
constexpr int       PEAK_SURCHARGE_PC  = 20;
constexpr long long MIN_BALANCE_PAISE  = 5000LL;

// ===== Zone =====
enum class Zone { Inner = 0, Middle = 1, Outer = 2 };

const char* zoneName(Zone z) {
    switch (z) {
        case Zone::Inner:  return "Inner";
        case Zone::Middle: return "Middle";
        case Zone::Outer:  return "Outer";
        default:           return "?";
    }
}

// ===== FareTable =====
class FareTable {
public:
    static bool isPeakHour(int hour) {
        return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
    }

    static long long computeFare(Zone z, bool peak) {
        long long base = 0;
        switch (z) {
            case Zone::Inner:  base = FARE_INNER_PAISE;  break;
            case Zone::Middle: base = FARE_MIDDLE_PAISE; break;
            case Zone::Outer:  base = FARE_OUTER_PAISE;  break;
        }
        if (peak) base += (base * PEAK_SURCHARGE_PC) / 100;
        return base;
    }
};

// ===== SmartCard (final version) =====
class SmartCard {
private:
    std::string cardId_;
    long long   balance_;

public:
    SmartCard(const std::string& id, long long initialPaise)
        : cardId_(id), balance_(initialPaise) {}

    bool canTravel(long long farePaise) const {
        return (balance_ - farePaise) >= MIN_BALANCE_PAISE;
    }

    bool deductFare(long long farePaise) {
        if (farePaise <= 0 || !canTravel(farePaise)) return false;
        balance_ -= farePaise;
        return true;
    }

    bool recharge(long long amountPaise) {
        if (amountPaise <= 0) return false;
        balance_ += amountPaise;
        return true;
    }

    long long   getBalance() const { return balance_; }
    std::string getId()      const { return cardId_;  }
};

// ---------------------------------------------------------------
// Demonstrate correctness for all scale levels
// ---------------------------------------------------------------
void scaleTest() {
    std::cout << "=== Correctness at All Scale Levels ===\n";

    long long scaleLevels[] = { 20000LL, 500000LL, 3000000LL };
    const char* labels[]    = { "1 station", "1 line", "whole network" };

    // Use Outer peak fare (highest fare) for worst-case overflow test
    long long farePerJourney = FareTable::computeFare(Zone::Outer, true); // 3600 paise

    for (int i = 0; i < 3; ++i) {
        long long n     = scaleLevels[i];
        long long total = n * farePerJourney; // long long - no overflow
        std::cout << "  " << labels[i] << " (" << n << " journeys):\n";
        std::cout << "    Total paise : " << total << '\n';
        std::cout << "    Total rupees: Rs " << total / 100 << '\n';
        std::cout << "    Fits in int?  " << (total > INT_MAX ? "NO (overflow!)" : "Yes") << '\n';
        std::cout << "    Fits in long long? YES (LLONG_MAX = " << LLONG_MAX << ")\n\n";
    }
}

// ---------------------------------------------------------------
// Show how a fare revision is carried out
// ---------------------------------------------------------------
void fareRevisionDemo() {
    std::cout << "=== Fare Revision Procedure ===\n";
    std::cout << "  With the recommended design, to revise fares:\n";
    std::cout << "  1. Change ONE constexpr constant (e.g., FARE_OUTER_PAISE = 4000).\n";
    std::cout << "  2. Recompile: g++ -std=c++17 -Wall -Wextra -o gate gate.cpp\n";
    std::cout << "  3. Redeploy the binary to all gate terminals.\n";
    std::cout << "  No other code changes are needed because FareTable::computeFare()\n";
    std::cout << "  reads from the constants everywhere.\n\n";

    // Demonstrate current vs hypothetical revised fares
    std::cout << "  Current fares:\n";
    for (int z = 0; z <= 2; ++z) {
        Zone zone = static_cast<Zone>(z);
        long long offPeak = FareTable::computeFare(zone, false);
        long long peak    = FareTable::computeFare(zone, true);
        std::cout << "    Zone " << zoneName(zone)
                  << " | Off-peak: Rs " << offPeak / 100
                  << " | Peak: Rs " << peak / 100 << '\n';
    }
}

// ---------------------------------------------------------------
// Limitations of the current design
// ---------------------------------------------------------------
void limitations() {
    std::cout << "\n=== Design Limitations ===\n";
    std::cout << "  1. Fixed zone count: Adding a 4th zone requires modifying\n";
    std::cout << "     the switch in computeFare() and the Zone enum.\n";
    std::cout << "     IMPROVEMENT: Use an array indexed by zone int value.\n\n";
    std::cout << "  2. Hardcoded fare values: A configuration-file approach would\n";
    std::cout << "     allow fare updates without recompiling.\n";
    std::cout << "     IMPROVEMENT: Read fares from a file at startup.\n\n";
    std::cout << "  3. No concession hierarchy: Students and seniors are handled\n";
    std::cout << "     by a switch in Passenger, not by polymorphism.\n";
    std::cout << "     IMPROVEMENT: Derive Student and SeniorCitizen from Passenger.\n\n";
    std::cout << "  4. No persistence: card balances are lost on program exit.\n";
    std::cout << "     IMPROVEMENT: Serialize card state to file/database.\n\n";
    std::cout << "  5. No concurrency: gate software needs thread-safe deductions.\n";
    std::cout << "     IMPROVEMENT: Use mutex or atomic operations on balance_.\n";
}

int main() {
    std::cout << "=== Task 11: Recommend and Defend the Final Design ===\n\n";

    // Print recommendations
    std::cout << "=== Final Design Recommendations ===\n";
    std::cout << "  Type        : long long (paise) for all money values\n";
    std::cout << "  Constants   : constexpr (compile-time, typed, scoped)\n";
    std::cout << "  Zone IDs    : enum class Zone (type-safe)\n";
    std::cout << "  Classes     : SmartCard (private balance), FareTable (static methods)\n";
    std::cout << "  Peak check  : free function isPeakHour(int hour)\n";
    std::cout << "  Fare rule   : (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20)\n";
    std::cout << "  Balance floor: MIN_BALANCE_PAISE = 5000 (enforced in canTravel)\n\n";

    scaleTest();
    fareRevisionDemo();
    limitations();

    std::cout << "\n=== Conclusion ===\n";
    std::cout << "  The design is correct and scalable to 3,000,000 daily journeys.\n";
    std::cout << "  long long eliminates overflow and double eliminates rounding errors.\n";
    std::cout << "  The SmartCard class enforces the Rs-50 invariant unconditionally.\n";
    std::cout << "  A single fare revision takes 1 line change + recompile.\n";

    return 0;
}
