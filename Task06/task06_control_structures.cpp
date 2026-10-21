/*
 * TASK 6 - Design the Control Structures
 * DSA0104 - Object Oriented Programming with C++
 * Student: Dhanush S | Register No: 192511297
 * Faculty: Arunkumar Palthasar | SIMATS Engineering
 *
 * Complete-the-Code: fills in TODO 1 and TODO 2 from the assignment
 * and adds the boundary journey tests at 10 AM and 8 PM.
 *
 * Pseudocode (SIMATS Engineering Standard):
 * ------------------------------------------
 * FUNCTION fare(zone z, bool peak):
 *   BEGIN
 *     SWITCH z
 *       CASE Inner  : base <- 1000
 *       CASE Middle : base <- 2000
 *       CASE Outer  : base <- 3000
 *     END SWITCH
 *     IF peak THEN
 *       surcharge <- (base * 20) / 100
 *       base <- base + surcharge
 *     END IF
 *     RETURN base
 *   END
 *
 * FUNCTION canTravel(balance, fare):
 *   BEGIN
 *     RETURN (balance - fare) >= MIN_BALANCE_PAISE
 *   END
 * ------------------------------------------
 *
 * Compile: g++ -std=c++17 -Wall -Wextra -o task06 task06_control_structures.cpp
 * Run    : ./task06
 */

#include <iomanip>
#include <iostream>

enum class Zone { Inner, Middle, Outer };

constexpr int      BASE_PAISE[]           = {1000, 2000, 3000}; // Rs 10, Rs 20, Rs 30
constexpr int      PEAK_SURCHARGE_PERCENT = 20;
constexpr long long MIN_BALANCE_PAISE     = 5000; // Rs 50 must remain on the card

// ---------------------------------------------------------------
// TODO 1 COMPLETED:
//   Switch on z to pick the base fare from BASE_PAISE, then add
//   PEAK_SURCHARGE_PERCENT in peak hours using integer arithmetic only.
// ---------------------------------------------------------------
long long fare(Zone z, bool peak)
{
    long long base = 0;
    switch (z) {
        case Zone::Inner:  base = BASE_PAISE[0]; break;
        case Zone::Middle: base = BASE_PAISE[1]; break;
        case Zone::Outer:  base = BASE_PAISE[2]; break;
        // default not needed: all enum values covered
        // If an unknown zone is passed (via cast), base stays 0
    }
    if (peak) {
        // Integer arithmetic only: (base * 20) / 100 - exact, no float
        base += (base * PEAK_SURCHARGE_PERCENT) / 100;
    }
    return base;
}

// ---------------------------------------------------------------
// TODO 2 COMPLETED:
//   Return true only if the balance left after paying the fare
//   is at least MIN_BALANCE_PAISE.
// ---------------------------------------------------------------
bool canTravel(long long balance, long long f)
{
    return (balance - f) >= MIN_BALANCE_PAISE;
}

// ---------------------------------------------------------------
// Helper: isPeakHour - correct boundary handling
//   Peak: 08:00 <= hour < 10  OR  17:00 <= hour < 20
//   10 AM is NOT peak; 8 PM is NOT peak.
// ---------------------------------------------------------------
bool isPeakHour(int hour)
{
    return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
}

// ---------------------------------------------------------------
// Process a batch of trips (original assignment main, preserved)
// ---------------------------------------------------------------
void processOriginalTrips()
{
    long long balance = 10000; // Rs 100.00 in paise
    struct Trip { Zone zone; bool peak; };
    Trip trips[] = {{Zone::Inner, false}, {Zone::Outer, true}, {Zone::Middle, false}};

    std::cout << "=== Original Assignment Trips ===\n";
    for (const Trip& t : trips) {
        long long f = fare(t.zone, t.peak);
        if (canTravel(balance, f)) {
            balance -= f;
            std::cout << "Fare Rs " << f / 100 << '.' << std::setw(2)
                      << std::setfill('0') << f % 100 << std::setfill(' ');
        } else {
            std::cout << "Denied (low balance)";
        }
        std::cout << " balance Rs " << balance / 100 << '\n';
    }
}

// ---------------------------------------------------------------
// Boundary test: exactly 10 AM and exactly 8 PM
// ---------------------------------------------------------------
void boundaryTest()
{
    std::cout << "\n=== Boundary Journey Tests ===\n";

    long long balance = 20000; // Rs 200 to allow multiple journeys
    struct BoundaryCase { int hour; const char* label; Zone zone; };
    BoundaryCase cases[] = {
        { 10, "exactly 10 AM (should be OFF-PEAK)", Zone::Inner },
        { 20, "exactly 8 PM  (should be OFF-PEAK)", Zone::Inner },
        {  9, "9 AM  (PEAK)",                       Zone::Inner },
        { 19, "7 PM  (PEAK)",                       Zone::Inner },
        {  8, "8 AM  (PEAK)",                       Zone::Inner },
        { 17, "5 PM  (PEAK)",                       Zone::Inner },
    };

    for (const auto& c : cases) {
        bool peak = isPeakHour(c.hour);
        long long f = fare(c.zone, peak);
        std::cout << "  Hour " << std::setw(2) << c.hour
                  << " | " << (peak ? "PEAK    " : "off-peak")
                  << " | Fare: Rs " << f / 100
                  << " | " << c.label << '\n';
        if (canTravel(balance, f)) balance -= f;
    }
}

// ---------------------------------------------------------------
// Demonstrate: for, range-for, while, break, continue
// ---------------------------------------------------------------
void controlStructureDemo()
{
    std::cout << "\n=== Control Structure Comparison ===\n";
    long long balance = 15000; // Rs 150
    Zone zones[] = { Zone::Inner, Zone::Middle, Zone::Outer, Zone::Inner };
    bool peaks[]  = { false, true, false, true };
    int  hours[]  = { 14, 9, 22, 8 };

    // -- Range-based for (best when iterating a fixed collection) --
    std::cout << "Range-based for loop:\n";
    int idx = 0;
    for (const Zone& z : zones) {
        bool peak = isPeakHour(hours[idx]);
        long long f = fare(z, peak);
        if (f == 0) { ++idx; continue; } // skip invalid fare (continue demo)
        if (!canTravel(balance, f)) {
            std::cout << "  Trip " << idx+1 << ": DENIED (low balance) - stopping\n";
            break;                         // break demo
        }
        balance -= f;
        std::cout << "  Trip " << idx+1 << ": Fare Rs " << f/100
                  << " | Balance Rs " << balance/100 << '\n';
        ++idx;
    }

    // -- Classic for loop (best when index needed) --
    std::cout << "\nClassic for loop (all zones, off-peak):\n";
    for (int i = 0; i <= 2; ++i) {
        Zone z    = static_cast<Zone>(i);
        long long f = fare(z, false);
        std::cout << "  Zone " << i << ": Rs " << f/100 << '\n';
    }

    // -- while loop (best when count unknown) --
    std::cout << "\nWhile loop until balance < Rs 50 buffer:\n";
    long long bal2 = 8000; // Rs 80
    int tripCount  = 0;
    while (canTravel(bal2, fare(Zone::Inner, false))) {
        bal2 -= fare(Zone::Inner, false);
        ++tripCount;
        std::cout << "  Trip " << tripCount << ": Balance now Rs " << bal2/100 << '\n';
    }
    std::cout << "  Cannot travel further - balance Rs " << bal2/100
              << " (minimum Rs 50 rule enforced)\n";
}

int main()
{
    std::cout << "=== Task 6: Control Structures ===\n\n";
    processOriginalTrips();
    boundaryTest();
    controlStructureDemo();
    return 0;
}
