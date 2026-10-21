/*
 * TASK 8 - Read and Print Data Safely
 * DSA0104 - Object Oriented Programming with C++
 * Student: Dhanush S | Register No: 192511297
 * Faculty: Arunkumar Palthasar | SIMATS Engineering
 *
 * Demonstrates safe input with cin + cin.fail() recovery,
 * and formatted output with <iomanip>.
 *
 * Compile: g++ -std=c++17 -Wall -Wextra -o task08 task08_io.cpp
 * Run    : ./task08
 */

#include <iostream>
#include <iomanip>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

// ---------------------------------------------------------------
// Constants
// ---------------------------------------------------------------
constexpr long long FARE_INNER_PAISE  = 1000LL;
constexpr long long FARE_MIDDLE_PAISE = 2000LL;
constexpr long long FARE_OUTER_PAISE  = 3000LL;
constexpr int       PEAK_SURCHARGE_PC = 20;
constexpr long long MIN_BALANCE_PAISE = 5000LL;

enum class Zone { Inner = 0, Middle = 1, Outer = 2 };

// ---------------------------------------------------------------
// Safe integer input with cin.fail() recovery
// Keeps asking until the user enters a valid integer in [lo, hi].
// ---------------------------------------------------------------
int safeReadInt(const std::string& prompt, int lo, int hi) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (!std::cin.fail() && value >= lo && value <= hi) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        // Recovery sequence:
        std::cin.clear();  // reset failbit / badbit
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard bad chars
        std::cout << "  [ERROR] Invalid input. Please enter a number between "
                  << lo << " and " << hi << ".\n";
    }
}

// ---------------------------------------------------------------
// Helper functions
// ---------------------------------------------------------------
bool isPeakHour(int hour) {
    return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
}

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

const char* zoneName(Zone z) {
    switch (z) {
        case Zone::Inner:  return "Inner";
        case Zone::Middle: return "Middle";
        case Zone::Outer:  return "Outer";
        default:           return "?";
    }
}

// ---------------------------------------------------------------
// Print formatted money in Rs X.XX format
// ---------------------------------------------------------------
void printRupees(long long paise) {
    std::cout << "Rs " << paise / 100 << '.'
              << std::setw(2) << std::setfill('0') << paise % 100
              << std::setfill(' ');
}

// ---------------------------------------------------------------
// Print a neat journey report table
// ---------------------------------------------------------------
struct JourneyRecord {
    int       tripNo;
    Zone      zone;
    int       hour;
    bool      peak;
    long long farePaise;
    bool      allowed;
    long long balanceAfterPaise;
};

void printJourneyReport(const std::vector<JourneyRecord>& records) {
    std::cout << "\n+-------+--------+------+------+----------+---------+-----------+\n";
    std::cout << "| Trip  | Zone   | Hour | Peak | Fare     | Status  | Balance   |\n";
    std::cout << "+-------+--------+------+------+----------+---------+-----------+\n";
    for (const auto& r : records) {
        std::cout << "| " << std::setw(5) << r.tripNo << " "
                  << "| " << std::setw(6) << zoneName(r.zone) << " "
                  << "| " << std::setw(4) << r.hour << " "
                  << "| " << std::setw(4) << (r.peak ? "Yes" : "No") << " "
                  << "| ";
        // Fare column - formatted Rs X.XX
        std::ostringstream fareStr;
        fareStr << "Rs " << r.farePaise / 100 << '.'
                << std::setw(2) << std::setfill('0') << r.farePaise % 100;
        std::cout << std::setfill(' ') << std::setw(8) << fareStr.str() << " "
                  << "| " << std::setw(7) << (r.allowed ? "OK" : "DENIED") << " "
                  << "| ";
        std::ostringstream balStr;
        balStr << "Rs " << r.balanceAfterPaise / 100;
        std::cout << std::setw(9) << balStr.str() << " |\n";
    }
    std::cout << "+-------+--------+------+------+----------+---------+-----------+\n";
}

// ---------------------------------------------------------------
// Interactive gate simulation (demonstrates cin safety)
// ---------------------------------------------------------------
void interactiveGate() {
    std::cout << "\n=== Interactive Gate Simulation ===\n";
    std::cout << "Starting balance: Rs 200\n\n";

    long long balance = 20000LL; // Rs 200
    std::vector<JourneyRecord> records;
    int tripNo = 0;

    char another = 'y';
    while (another == 'y' || another == 'Y') {
        ++tripNo;
        std::cout << "\n-- Trip " << tripNo << " --\n";

        // Safe zone input
        int zoneInput = safeReadInt("  Enter zone (0=Inner, 1=Middle, 2=Outer): ", 0, 2);
        // Safe hour input
        int hour      = safeReadInt("  Enter hour (0-23): ", 0, 23);

        Zone z    = static_cast<Zone>(zoneInput);
        bool peak = isPeakHour(hour);
        long long f = computeFare(z, peak);

        JourneyRecord rec;
        rec.tripNo  = tripNo;
        rec.zone    = z;
        rec.hour    = hour;
        rec.peak    = peak;
        rec.farePaise = f;

        if ((balance - f) >= MIN_BALANCE_PAISE) {
            balance -= f;
            rec.allowed = true;
            std::cout << "  Access GRANTED. Fare: ";
            printRupees(f);
            std::cout << " | Balance: ";
            printRupees(balance);
            std::cout << '\n';
        } else {
            rec.allowed = false;
            std::cout << "  Access DENIED. Insufficient balance. Balance: ";
            printRupees(balance);
            std::cout << '\n';
        }
        rec.balanceAfterPaise = balance;
        records.push_back(rec);

        std::cout << "\n  Process another journey? (y/n): ";
        std::cin >> another;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    if (!records.empty()) {
        std::cout << "\n=== Journey Report ===";
        printJourneyReport(records);
        std::cout << "Final balance: ";
        printRupees(balance);
        std::cout << '\n';
    }
}

// ---------------------------------------------------------------
// Demo run with scripted inputs (for automated testing)
// ---------------------------------------------------------------
void automatedDemo() {
    std::cout << "=== Automated Formatting Demo ===\n";

    // Demonstrate setw, setfill, fixed, setprecision
    std::cout << "\n-- iomanip formatting --\n";
    std::cout << "  setw(10) right-aligned: " << std::setw(10) << 42 << '\n';
    std::cout << "  setfill('0') + setw(5): "
              << std::setw(5) << std::setfill('0') << 42
              << std::setfill(' ') << '\n';

    long long paise = 1200;
    std::cout << "  Rs " << paise / 100 << '.'
              << std::setw(2) << std::setfill('0') << paise % 100
              << std::setfill(' ') << " (formatted fare)\n";

    // Using fixed + setprecision for double (comparison only)
    double fareDouble = 12.00;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  double Rs " << fareDouble << " (fixed, 2dp)\n";
    std::cout << std::defaultfloat;

    // Demonstrate error recovery from string input
    std::cout << "\n-- getline + stoi alternative (Task 8 extra) --\n";
    std::vector<std::string> inputs = {"9", "abc", "-5", "23"};
    for (const std::string& s : inputs) {
        try {
            int val = std::stoi(s);
            if (val < 0 || val > 23) throw std::out_of_range("Hour out of range");
            std::cout << "  Input \"" << s << "\" -> valid hour " << val << '\n';
        } catch (const std::invalid_argument&) {
            std::cout << "  Input \"" << s << "\" -> INVALID (not a number)\n";
        } catch (const std::out_of_range&) {
            std::cout << "  Input \"" << s << "\" -> OUT OF RANGE (0-23)\n";
        }
    }
}

int main() {
    std::cout << "=== Task 8: Read and Print Data Safely ===\n\n";
    automatedDemo();
    interactiveGate();
    return 0;
}
