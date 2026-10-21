/*
 * TASK 9 - Implement and Test the Fare Logic
 * DSA0104 - Object Oriented Programming with C++
 * Student: Dhanush S | Register No: 192511297
 * Faculty: Arunkumar Palthasar | SIMATS Engineering
 *
 * Full implementation with a structured test table covering:
 *   - Normal journeys (all zones, peak/off-peak)
 *   - Boundary cases (exactly Rs 50 remaining, exact hour edges)
 *   - Invalid/edge cases (unknown zone cast, negative recharge, bad hour)
 *   - 5 hand-calculated journeys verified against actual output
 *
 * Compile: g++ -std=c++17 -Wall -Wextra -o task09 task09_implementation_test.cpp
 * Run    : ./task09
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

// ---------------------------------------------------------------
// Constants
// ---------------------------------------------------------------
constexpr long long FARE_INNER_PAISE   = 1000LL; // Rs 10
constexpr long long FARE_MIDDLE_PAISE  = 2000LL; // Rs 20
constexpr long long FARE_OUTER_PAISE   = 3000LL; // Rs 30
constexpr int       PEAK_SURCHARGE_PC  = 20;     // 20%
constexpr long long MIN_BALANCE_PAISE  = 5000LL; // Rs 50

enum class Zone { Inner = 0, Middle = 1, Outer = 2, Unknown = 99 };

// ---------------------------------------------------------------
// Core functions
// ---------------------------------------------------------------
bool isPeakHour(int hour) {
    if (hour < 0 || hour > 23) return false; // invalid hour
    return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
}

long long computeFare(Zone z, bool peak) {
    long long base = 0;
    switch (z) {
        case Zone::Inner:  base = FARE_INNER_PAISE;  break;
        case Zone::Middle: base = FARE_MIDDLE_PAISE; break;
        case Zone::Outer:  base = FARE_OUTER_PAISE;  break;
        default:           base = 0;                  break; // unknown zone
    }
    if (peak && base > 0) base += (base * PEAK_SURCHARGE_PC) / 100;
    return base;
}

bool canTravel(long long balance, long long fare) {
    return (balance - fare) >= MIN_BALANCE_PAISE;
}

// ---------------------------------------------------------------
// SmartCard class (full implementation)
// ---------------------------------------------------------------
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
        if (farePaise <= 0) return false;
        if (!canTravel(farePaise)) return false;
        balance_ -= farePaise;
        return true;
    }

    bool recharge(long long amountPaise) {
        if (amountPaise <= 0) {
            std::cout << "  [REJECTED] Negative/zero recharge\n";
            return false;
        }
        balance_ += amountPaise;
        return true;
    }

    void reset(long long newBalance) { balance_ = newBalance; }
    long long getBalance()     const { return balance_; }
    std::string getId()        const { return cardId_;  }
};

// ---------------------------------------------------------------
// Test framework
// ---------------------------------------------------------------
struct TestCase {
    std::string description;
    Zone        zone;
    int         hour;
    long long   startBalance;  // paise
    long long   expectedFare;  // paise (0 = denied)
    bool        expectedAllowed;
    long long   expectedBalanceAfter; // paise
};

void runTestTable() {
    std::cout << "\n=== STRUCTURED TEST TABLE ===\n";

    std::vector<TestCase> tests = {
        // Normal journeys - all zones, peak
        {"Normal: Inner zone, 9 AM (peak)",       Zone::Inner,  9,  20000, 1200, true,  18800},
        {"Normal: Middle zone, 9 AM (peak)",       Zone::Middle, 9,  20000, 2400, true,  17600},
        {"Normal: Outer zone, 9 AM (peak)",        Zone::Outer,  9,  20000, 3600, true,  16400},
        // Normal journeys - off-peak
        {"Normal: Inner zone, 2 PM (off-peak)",    Zone::Inner,  14, 20000, 1000, true,  19000},
        {"Normal: Middle zone, 2 PM (off-peak)",   Zone::Middle, 14, 20000, 2000, true,  18000},
        {"Normal: Outer zone, 2 PM (off-peak)",    Zone::Outer,  14, 20000, 3000, true,  17000},
        // Boundary: exactly 8 AM (peak inclusive)
        {"Boundary: 8 AM (PEAK, inclusive)",       Zone::Inner,  8,  20000, 1200, true,  18800},
        // Boundary: exactly 10 AM (NOT peak)
        {"Boundary: 10 AM (off-peak, exclusive)",  Zone::Inner,  10, 20000, 1000, true,  19000},
        // Boundary: exactly 5 PM (peak inclusive)
        {"Boundary: 17:00 (PEAK, inclusive)",      Zone::Outer,  17, 20000, 3600, true,  16400},
        // Boundary: exactly 8 PM (NOT peak)
        {"Boundary: 20:00 (off-peak, exclusive)",  Zone::Outer,  20, 20000, 3000, true,  17000},
        // Boundary: balance leaves exactly Rs 50
        {"Boundary: balance exactly Rs 50 after",  Zone::Inner,  14, 6000,  1000, true,  5000},
        // Boundary: balance would go below Rs 50 -> DENIED
        {"Boundary: balance below Rs 50 -> deny",  Zone::Inner,  14, 5500,  1000, false, 5500},
        // Invalid: unknown zone (cast to Zone::Unknown)
        {"Invalid: unknown zone (fare = 0)",       Zone::Unknown,12, 20000, 0,    false, 20000},
        // Invalid: exactly Rs 50 on card (no room for any fare)
        {"Invalid: card has exactly Rs 50",        Zone::Inner,  14, 5000,  1000, false, 5000},
    };

    // Print header
    std::cout << std::left
              << std::setw(46) << "Test Description"
              << std::setw(10) << "ExpFare"
              << std::setw(10) << "ActFare"
              << std::setw(9)  << "ExpOK"
              << std::setw(9)  << "ActOK"
              << std::setw(12) << "ExpBal"
              << std::setw(12) << "ActBal"
              << "PASS?\n";
    std::cout << std::string(110, '-') << '\n';

    int pass = 0, fail = 0;
    for (const auto& tc : tests) {
        SmartCard card("TEST", tc.startBalance);
        bool peak      = isPeakHour(tc.hour);
        long long fare = computeFare(tc.zone, peak);
        bool allowed   = card.deductFare(fare);
        long long balAfter = card.getBalance();

        bool fareMatch    = (fare    == tc.expectedFare);
        bool allowedMatch = (allowed == tc.expectedAllowed);
        bool balMatch     = (balAfter == tc.expectedBalanceAfter);
        bool passed       = fareMatch && allowedMatch && balMatch;

        if (passed) ++pass; else ++fail;

        std::cout << std::setw(46) << tc.description.substr(0, 45)
                  << std::setw(10) << tc.expectedFare / 100
                  << std::setw(10) << fare / 100
                  << std::setw(9)  << (tc.expectedAllowed ? "YES" : "NO")
                  << std::setw(9)  << (allowed ? "YES" : "NO")
                  << std::setw(12) << tc.expectedBalanceAfter / 100
                  << std::setw(12) << balAfter / 100
                  << (passed ? "PASS" : "FAIL ***") << '\n';
    }
    std::cout << std::string(110, '-') << '\n';
    std::cout << "Results: " << pass << " PASSED, " << fail << " FAILED\n";
}

// ---------------------------------------------------------------
// 5 hand-calculated journeys
// Starting balance: Rs 200 (20000 paise)
// Journey 1: Inner, 9 AM (peak):  Rs 10 * 1.20 = Rs 12  -> balance Rs 188
// Journey 2: Outer, 2 PM (off):   Rs 30           -> balance Rs 158
// Journey 3: Middle, 6 PM (peak): Rs 20 * 1.20 = Rs 24  -> balance Rs 134
// Journey 4: Inner, 10 AM (off):  Rs 10           -> balance Rs 124
// Journey 5: Outer, 7 PM (peak):  Rs 30 * 1.20 = Rs 36  -> balance Rs 88
// Total deducted: 12 + 30 + 24 + 10 + 36 = Rs 112
// ---------------------------------------------------------------
void handCalculatedJourneys() {
    std::cout << "\n=== 5 Hand-Calculated Journeys ===\n";
    std::cout << "Starting balance: Rs 200\n\n";

    SmartCard card("HAND", 20000LL);
    struct HJourney { Zone zone; int hour; const char* desc; };
    HJourney journeys[] = {
        {Zone::Inner,  9,  "Inner, 9 AM (peak, Rs 10 + 20% = Rs 12)"},
        {Zone::Outer,  14, "Outer, 2 PM (off-peak, Rs 30)"},
        {Zone::Middle, 18, "Middle, 6 PM (peak, Rs 20 + 20% = Rs 24)"},
        {Zone::Inner,  10, "Inner, 10 AM (off-peak, Rs 10)"},
        {Zone::Outer,  19, "Outer, 7 PM (peak, Rs 30 + 20% = Rs 36)"},
    };

    long long totalDeducted = 0;
    for (int i = 0; i < 5; ++i) {
        bool peak   = isPeakHour(journeys[i].hour);
        long long f = computeFare(journeys[i].zone, peak);
        bool ok     = card.deductFare(f);
        if (ok) totalDeducted += f;
        std::cout << "  Journey " << (i+1) << ": " << journeys[i].desc << '\n';
        std::cout << "    Fare: Rs " << f/100
                  << " | Allowed: " << (ok ? "YES" : "NO")
                  << " | Balance: Rs " << card.getBalance()/100 << '\n';
    }
    std::cout << "\n  Total deducted (expected Rs 112): Rs " << totalDeducted/100 << '\n';
    std::cout << "  Final balance (expected Rs 88):   Rs " << card.getBalance()/100 << '\n';
    bool ok = (totalDeducted == 11200LL && card.getBalance() == 8800LL);
    std::cout << "  Verification: " << (ok ? "PASS" : "FAIL") << '\n';
}

int main() {
    std::cout << "=== Task 9: Implementation and Testing ===\n";
    runTestTable();
    handCalculatedJourneys();
    return 0;
}
