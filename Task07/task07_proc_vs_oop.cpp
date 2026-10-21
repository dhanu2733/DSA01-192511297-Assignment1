/*
 * TASK 7 - From Procedural to Object-Oriented
 * DSA0104 - Object Oriented Programming with C++
 * Student: Dhanush S | Register No: 192511297
 * Faculty: Arunkumar Palthasar | SIMATS Engineering
 *
 * Presents TWO working implementations:
 *   Version A - Procedural style (global vars + free functions)
 *   Version B - Object-Oriented style (SmartCard class)
 * Both produce identical results; the comparison shows which is
 * easier to maintain when the minimum balance rule changes.
 *
 * Compile: g++ -std=c++17 -Wall -Wextra -o task07 task07_proc_vs_oop.cpp
 * Run    : ./task07
 */

#include <iostream>
#include <iomanip>
#include <string>

// ===============================================================
// SHARED CONSTANTS (used by both versions)
// ===============================================================
constexpr long long FARE_INNER_PAISE   = 1000LL; // Rs 10
constexpr long long FARE_MIDDLE_PAISE  = 2000LL; // Rs 20
constexpr long long FARE_OUTER_PAISE   = 3000LL; // Rs 30
constexpr int       PEAK_SURCHARGE_PC  = 20;
constexpr long long MIN_BALANCE_PAISE  = 5000LL; // Rs 50

enum class Zone { Inner = 0, Middle = 1, Outer = 2 };

bool isPeakHour(int hour) {
    return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
}

// ===============================================================
// VERSION A - PROCEDURAL STYLE
// ---------------------------------------------------------------
// Variables are plain values passed as arguments; the caller is
// responsible for enforcing all rules. Nothing stops external
// code from setting balance to a negative number.
// ===============================================================
namespace Procedural {

    long long computeFare(Zone z, bool peak) {
        long long base = 0;
        switch (z) {
            case Zone::Inner:  base = FARE_INNER_PAISE;  break;
            case Zone::Middle: base = FARE_MIDDLE_PAISE; break;
            case Zone::Outer:  base = FARE_OUTER_PAISE;  break;
        }
        if (peak) base += (base * PEAK_SURCHARGE_PC) / 100;
        return base;
    }

    bool canTravel(long long balance, long long fare) {
        return (balance - fare) >= MIN_BALANCE_PAISE;
    }

    // External code can directly modify balance - NO protection!
    // If we change MIN_BALANCE_PAISE, we must find and update
    // every canTravel() call site in the entire codebase.
    bool deductFare(long long& balance, long long fare) {
        if (!canTravel(balance, fare)) return false;
        balance -= fare;
        return true;
    }

    void printBalance(const std::string& cardId, long long balance) {
        std::cout << "  [Proc] Card " << cardId
                  << " Balance: Rs " << balance / 100 << '\n';
    }

    void runDemo() {
        std::cout << "=== VERSION A: Procedural Style ===\n";

        // All data is just plain variables - no encapsulation
        std::string cardId = "PROC-001";
        long long   balance = 15000LL; // Rs 150

        struct Trip { Zone zone; int hour; };
        Trip trips[] = {
            {Zone::Inner,  9},   // peak
            {Zone::Outer,  14},  // off-peak
            {Zone::Middle, 18},  // peak
        };

        for (const Trip& t : trips) {
            bool peak   = isPeakHour(t.hour);
            long long f = computeFare(t.zone, peak);
            if (deductFare(balance, f)) {
                std::cout << "  ALLOWED  | Fare: Rs " << f / 100
                          << " | Hour: " << t.hour << " | Peak: " << (peak ? "Y" : "N") << '\n';
            } else {
                std::cout << "  DENIED   | Fare: Rs " << f / 100 << " (low balance)\n";
            }
            printBalance(cardId, balance);
        }

        // DANGER: external code can corrupt balance with no check
        balance = -9999; // Nothing stops this in procedural style!
        std::cout << "  [WARNING] balance set to " << balance
                  << " - no protection in procedural style!\n\n";
    }
}

// ===============================================================
// VERSION B - OBJECT-ORIENTED STYLE
// ---------------------------------------------------------------
// SmartCard encapsulates balance_ as a private member.
// Only deductFare() and recharge() can change it.
// The Rs-50 rule is enforced in ONE place (deductFare).
// If the rule changes (e.g., Rs 75), only ONE line changes.
// ===============================================================
namespace OOP {

    class SmartCard {
    private:
        std::string cardId_;
        long long   balance_;   // PAISE, private - cannot be set externally

    public:
        SmartCard(const std::string& id, long long initialPaise)
            : cardId_(id), balance_(initialPaise) {}

        // Rule enforced in ONE place - change here = change everywhere
        bool canTravel(long long farePaise) const {
            return (balance_ - farePaise) >= MIN_BALANCE_PAISE;
        }

        // Only way to reduce balance; enforces the Rs-50 floor
        bool deductFare(long long farePaise) {
            if (!canTravel(farePaise)) return false;
            balance_ -= farePaise;
            return true;
            // balance_ can NEVER go below Rs 50 through this method
        }

        // Only way to increase balance; rejects invalid amounts
        bool recharge(long long amountPaise) {
            if (amountPaise <= 0) return false;
            balance_ += amountPaise;
            return true;
        }

        // Read-only access - no mutation
        long long   getBalance() const { return balance_; }
        std::string getId()      const { return cardId_;  }

        void printBalance() const {
            std::cout << "  [OOP]  Card " << cardId_
                      << " Balance: Rs " << balance_ / 100 << '\n';
        }
    };

    long long computeFare(Zone z, bool peak) {
        long long base = 0;
        switch (z) {
            case Zone::Inner:  base = FARE_INNER_PAISE;  break;
            case Zone::Middle: base = FARE_MIDDLE_PAISE; break;
            case Zone::Outer:  base = FARE_OUTER_PAISE;  break;
        }
        if (peak) base += (base * PEAK_SURCHARGE_PC) / 100;
        return base;
    }

    void runDemo() {
        std::cout << "=== VERSION B: Object-Oriented Style ===\n";

        SmartCard card("OOP-001", 15000LL); // Rs 150

        struct Trip { Zone zone; int hour; };
        Trip trips[] = {
            {Zone::Inner,  9},
            {Zone::Outer,  14},
            {Zone::Middle, 18},
        };

        for (const Trip& t : trips) {
            bool peak   = isPeakHour(t.hour);
            long long f = computeFare(t.zone, peak);
            if (card.deductFare(f)) {
                std::cout << "  ALLOWED  | Fare: Rs " << f / 100
                          << " | Hour: " << t.hour << " | Peak: " << (peak ? "Y" : "N") << '\n';
            } else {
                std::cout << "  DENIED   | Fare: Rs " << f / 100 << " (low balance)\n";
            }
            card.printBalance();
        }

        // This line will NOT compile if balance_ is private:
        // card.balance_ = -9999;  // ERROR: member is private
        std::cout << "  [SAFE] Cannot set balance_ directly (private). Rule always enforced.\n\n";
    }
}

// ===============================================================
// COMPARISON SUMMARY
// ===============================================================
void printComparison() {
    std::cout << "=== Design Comparison: Procedural vs OOP ===\n";
    std::cout << std::left;
    std::cout << std::setw(40) << "Aspect"
              << std::setw(20) << "Procedural"
              << "OOP (SmartCard)\n";
    std::cout << std::string(75, '-') << '\n';
    std::cout << std::setw(40) << "Balance protection"
              << std::setw(20) << "None (any code can set)"
              << "Private member, enforced by deductFare()\n";
    std::cout << std::setw(40) << "Rs-50 rule location"
              << std::setw(20) << "Every canTravel() call"
              << "One place: SmartCard::canTravel()\n";
    std::cout << std::setw(40) << "Cost to change minimum balance"
              << std::setw(20) << "Find all call sites"
              << "Change one constant + recompile\n";
    std::cout << std::setw(40) << "Can negative balance appear?"
              << std::setw(20) << "YES (no guard)"
              << "NO (deductFare enforces floor)\n";
    std::cout << std::setw(40) << "Time complexity per journey"
              << std::setw(20) << "O(1)"
              << "O(1)\n";
    std::cout << std::setw(40) << "Space per card"
              << std::setw(20) << "sizeof(long long)"
              << "sizeof(SmartCard) ~ same\n";
    std::cout << '\n';
    std::cout << "VERDICT: OOP is strongly preferred because the invariant\n";
    std::cout << "(balance never drops below Rs 50) is guaranteed by the\n";
    std::cout << "class itself, not by discipline of every caller.\n";
}

int main() {
    std::cout << "=== Task 7: Procedural vs Object-Oriented Design ===\n\n";
    Procedural::runDemo();
    OOP::runDemo();
    printComparison();
    return 0;
}
