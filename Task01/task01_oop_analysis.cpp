/*
 * TASK 1 - Understand the Problem and Identify the Objects
 * DSA0104 - Object Oriented Programming with C++
 * Student: Dhanush S | Register No: 192511297
 * Faculty: Arunkumar Palthasar | SIMATS Engineering
 *
 * This program demonstrates the object-oriented analysis of the
 * Namma Metro Smart-Card Fare System by modelling the objects
 * identified during analysis: SmartCard, Passenger, Journey,
 * Zone, and FareTable.
 *
 * Compile: g++ -std=c++17 -Wall -Wextra -o task01 task01_oop_analysis.cpp
 * Run    : ./task01
 */

#include <iostream>
#include <string>

// ---------------------------------------------------------------
// Zone enum class - represents the three fare zones
// enum class is chosen over plain enum for type-safety and scoping
// ---------------------------------------------------------------
enum class Zone { Inner = 0, Middle = 1, Outer = 2 };

// ---------------------------------------------------------------
// FareTable - encapsulates the static fare rules.
//   Data    : base fares per zone, surcharge percentage, minimum
//             balance required after a journey
//   Behaviour: computeFare(), isPeakHour()
//
// OOP principle applied: Encapsulation (fare rules are private
// constants; only the public interface is exposed).
// Abstraction     : callers ask for a fare without knowing how it
//                   is computed internally.
// ---------------------------------------------------------------
class FareTable {
public:
    // Base fares stored in integer PAISE (100 paise = Rs 1)
    // to avoid floating-point rounding errors.
    static constexpr long long FARE_INNER_PAISE  = 1000LL; // Rs 10
    static constexpr long long FARE_MIDDLE_PAISE = 2000LL; // Rs 20
    static constexpr long long FARE_OUTER_PAISE  = 3000LL; // Rs 30
    static constexpr int       PEAK_SURCHARGE_PC = 20;     // 20 %
    static constexpr long long MIN_BALANCE_PAISE = 5000LL; // Rs 50

    // Returns true when the hour falls in a peak window
    // Peak windows: 08:00 <= hour < 10:00  OR  17:00 <= hour < 20:00
    // (exactly 10 AM and exactly 8 PM are treated as off-peak)
    static bool isPeakHour(int hour) {
        return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
    }

    // Compute fare in paise for a given zone and peak flag
    static long long computeFare(Zone z, bool peak) {
        long long base = 0;
        switch (z) {
            case Zone::Inner:  base = FARE_INNER_PAISE;  break;
            case Zone::Middle: base = FARE_MIDDLE_PAISE; break;
            case Zone::Outer:  base = FARE_OUTER_PAISE;  break;
        }
        if (peak) {
            base += (base * PEAK_SURCHARGE_PC) / 100;
        }
        return base;
    }
};

// ---------------------------------------------------------------
// SmartCard - represents the stored-value card.
//   Data    : card ID, balance (in paise)
//   Behaviour: canTravel(), deductFare(), recharge(), printBalance()
//
// OOP principle applied:
//   Encapsulation - balance_ is private; only controlled methods
//                   can change it, preventing a negative balance.
// ---------------------------------------------------------------
class SmartCard {
private:
    std::string cardId_;
    long long   balance_;   // in paise

public:
    SmartCard(const std::string& id, long long initialPaise)
        : cardId_(id), balance_(initialPaise) {}

    // Rule: A journey is allowed only if (balance - fare) >= Rs 50
    bool canTravel(long long farePaise) const {
        return (balance_ - farePaise) >= FareTable::MIN_BALANCE_PAISE;
    }

    // Deduct fare; returns false if balance rule would be violated
    bool deductFare(long long farePaise) {
        if (!canTravel(farePaise)) return false;
        balance_ -= farePaise;
        return true;
    }

    // Recharge - negative amounts are rejected (rule: no negative recharge)
    bool recharge(long long amountPaise) {
        if (amountPaise <= 0) {
            std::cout << "[ERROR] Recharge amount must be positive.\n";
            return false;
        }
        balance_ += amountPaise;
        return true;
    }

    void printBalance() const {
        std::cout << "Card [" << cardId_ << "] Balance: Rs "
                  << balance_ / 100 << '.' << (balance_ % 100 < 10 ? "0" : "")
                  << balance_ % 100 << '\n';
    }

    long long getBalance() const { return balance_; }
    std::string getId()    const { return cardId_;  }
};

// ---------------------------------------------------------------
// Passenger - represents the card holder.
//   Data    : name, type (Regular / Student / SeniorCitizen), card
//   Behaviour: getDiscount()
//
// OOP principle applied:
//   Inheritance potential - Student and SeniorCitizen could be
//   derived classes of Passenger with overridden getDiscount().
//   Polymorphism - a virtual getDiscount() would let the fare
//   logic work uniformly for every passenger type.
// ---------------------------------------------------------------
enum class PassengerType { Regular, Student, SeniorCitizen };

class Passenger {
private:
    std::string   name_;
    PassengerType type_;
    SmartCard*    card_;   // association (not ownership)

public:
    Passenger(const std::string& name, PassengerType type, SmartCard* card)
        : name_(name), type_(type), card_(card) {}

    // Concession percentages
    int getConcessionPercent() const {
        switch (type_) {
            case PassengerType::Student:       return 25;
            case PassengerType::SeniorCitizen: return 30;
            default:                           return 0;
        }
    }

    SmartCard* getCard() const { return card_; }
    std::string getName() const { return name_; }
    PassengerType getType() const { return type_; }
};

// ---------------------------------------------------------------
// Journey - represents a single trip event.
//   Data    : zone, hour of day, passenger reference
//   Behaviour: process() - computes fare and triggers deduction
// ---------------------------------------------------------------
class Journey {
private:
    Zone      zone_;
    int       hour_;     // 0-23
    Passenger& pax_;

public:
    Journey(Zone zone, int hour, Passenger& pax)
        : zone_(zone), hour_(hour), pax_(pax) {}

    void process() {
        bool      peak      = FareTable::isPeakHour(hour_);
        long long fare      = FareTable::computeFare(zone_, peak);
        int       concPc    = pax_.getConcessionPercent();
        long long discount  = (fare * concPc) / 100;
        long long finalFare = fare - discount;

        SmartCard* card = pax_.getCard();
        std::cout << "Passenger: " << pax_.getName()
                  << " | Zone: " << static_cast<int>(zone_)
                  << " | Hour: " << hour_ << ":00"
                  << " | Peak: " << (peak ? "Yes" : "No")
                  << " | Fare: Rs " << fare / 100
                  << " | Concession: " << concPc << "%"
                  << " | Final: Rs " << finalFare / 100 << '\n';

        if (card->deductFare(finalFare)) {
            std::cout << "  Journey ALLOWED. ";
        } else {
            std::cout << "  Journey DENIED (insufficient balance). ";
        }
        card->printBalance();
    }
};

// ---------------------------------------------------------------
// main - demonstrates all objects and rules
// ---------------------------------------------------------------
int main() {
    std::cout << "=== Namma Metro Smart-Card Fare System - OOP Analysis ===\n\n";

    // Create smart cards (balance in paise)
    SmartCard card1("CARD-001", 20000LL); // Rs 200
    SmartCard card2("CARD-002", 8000LL);  // Rs 80

    // Create passengers
    Passenger regular ("Dhanush",   PassengerType::Regular,       &card1);
    Passenger student ("Priya",     PassengerType::Student,       &card2);

    // Process journeys
    std::cout << "--- Journey 1: Regular passenger, Inner zone, 9 AM (peak) ---\n";
    Journey j1(Zone::Inner, 9, regular);
    j1.process();

    std::cout << "\n--- Journey 2: Regular passenger, Outer zone, 6 PM (peak) ---\n";
    Journey j2(Zone::Outer, 18, regular);
    j2.process();

    std::cout << "\n--- Journey 3: Student passenger, Middle zone, 2 PM (off-peak) ---\n";
    Journey j3(Zone::Middle, 14, student);
    j3.process();

    std::cout << "\n--- Journey 4: Student card balance check ---\n";
    Journey j4(Zone::Outer, 19, student);
    j4.process();

    std::cout << "\n--- Recharge demonstration ---\n";
    card2.recharge(-500);       // invalid - negative
    card2.recharge(10000LL);    // Rs 100 valid recharge
    card2.printBalance();

    std::cout << "\n=== OOP Principles Summary ===\n";
    std::cout << "Encapsulation : SmartCard.balance_ is private; deductFare() enforces the Rs-50 rule.\n";
    std::cout << "Abstraction   : Journey.process() hides all fare arithmetic behind a simple call.\n";
    std::cout << "Inheritance   : PassengerType can be extended to derived classes (Student, Senior).\n";
    std::cout << "Polymorphism  : virtual getConcessionPercent() would allow runtime dispatch.\n";

    return 0;
}
