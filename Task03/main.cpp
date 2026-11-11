// Task03 - Q3 : Default Arguments and Function Overloading (Courier Charge Calculator)
// Name: B. Santhosh Kumar   Register No: 192511053
#include <iomanip>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

// Pricing: Rs 50 per kg, express = +50% of base, insurance = flat Rs 20.
struct Parcel {
    double kg;
    bool express;
    bool insured;
    std::string city;
    Parcel(double w, bool e = false, bool i = false, std::string c = "Chennai")
        : kg(w), express(e), insured(i), city(std::move(c)) {}
};

// ---------------------------------------------------------------------------
// REDESIGN : exactly one function per KIND of input, defaults only in ONE of them
// ---------------------------------------------------------------------------
// (1) weight in grams (int) - no default arguments, so it can never compete with (2)
double charge(int grams)
{
    std::cout << "  [charge(int grams)] ";
    return grams / 1000.0 * 50;
}
// (2) weight in kg (double) with defaults given from the RIGHT, in one declaration only
double charge(double kg, bool express = false, bool insured = false)
{
    std::cout << "  [charge(double kg, express, insured)] ";
    double base = kg * 50;
    if (express) base += base * 0.5;
    if (insured) base += 20;
    return base;
}
// (3) a whole Parcel object
double charge(const Parcel& p)
{
    std::cout << "  [charge(const Parcel&)] ";
    double base = p.kg * 50;
    if (p.express) base += base * 0.5;
    if (p.insured) base += 20;
    return base;
}

// ---------------------------------------------------------------------------
// ALTERNATIVE CONCEPT : one options struct (a new option = one new field, no new overloads)
// ---------------------------------------------------------------------------
struct Options {
    bool express = false;
    bool insured = false;
    bool fragile = false;                    // a NEW option added later: nothing else changes
};
double chargeWith(double kg, const Options& o = {})
{
    double base = kg * 50;
    if (o.express) base += base * 0.5;
    if (o.insured) base += 20;
    if (o.fragile) base += 15;
    return base;
}
// ... or a function template (one body for int grams, double kg, float kg ...)
template <typename W>
double chargeT(W weight, bool express = false)
{
    double kg = std::is_integral<W>::value ? static_cast<double>(weight) / 1000.0
                                           : static_cast<double>(weight);
    return kg * 50 * (express ? 1.5 : 1.0);
}

int main()
{
    std::cout << std::fixed << std::setprecision(2);
    Parcel parcel(3.0, true, true);

    // PREDICTION (written before running) - which function each call selects:
    //  1. charge(2)            -> int exact match            -> charge(int grams)       = 0.10
    //  2. charge(2.0f)         -> float->double PROMOTION    -> charge(double kg,...)   = 100.00
    //  3. charge(2.5)          -> double exact match         -> charge(double kg,...)   = 125.00
    //  4. charge(1.2, true)    -> (double,bool)              -> charge(double kg,...)   = 90.00
    //  5. charge(1.2, true, true)                            -> charge(double kg,...)   = 110.00
    //  6. charge(parcel)       -> Parcel exact match         -> charge(const Parcel&)   = 245.00
    //  7. charge(1500)         -> int exact match            -> charge(int grams)       = 75.00
    std::cout << "1. charge(2)               =" << charge(2) << '\n';
    std::cout << "2. charge(2.0f)            =" << charge(2.0f) << '\n';
    std::cout << "3. charge(2.5)             =" << charge(2.5) << '\n';
    std::cout << "4. charge(1.2, true)       =" << charge(1.2, true) << '\n';
    std::cout << "5. charge(1.2, true, true) =" << charge(1.2, true, true) << '\n';
    std::cout << "6. charge(parcel)          =" << charge(parcel) << '\n';
    std::cout << "7. charge(1500)            =" << charge(1500) << '\n';
    // charge(2L) would be AMBIGUOUS: long->int and long->double are both conversions of equal rank.
    // charge(2L);   // error: call of overloaded 'charge(long int)' is ambiguous

    std::cout << "\n--- Alternative: options struct and function template ---\n";
    Options o; o.express = true; o.insured = true; o.fragile = true;
    std::cout << "chargeWith(3.0, {express, insured, fragile}) = " << chargeWith(3.0, o) << '\n';
    std::cout << "chargeWith(2.5)                              = " << chargeWith(2.5) << '\n';
    std::cout << "chargeT(1500)   (int grams)   = " << chargeT(1500) << '\n';
    std::cout << "chargeT(2.5)    (double kg)   = " << chargeT(2.5) << '\n';
    std::cout << "chargeT(2.0f,true) (float kg) = " << chargeT(2.0f, true) << '\n';

    std::cout << "\nBytes copied per call: double=" << sizeof(double) << ", int=" << sizeof(int)
              << ", Parcel by value=" << sizeof(Parcel) << ", Parcel by const& = " << sizeof(void*)
              << ", Options by const& = " << sizeof(void*) << " (Options itself is " << sizeof(Options) << ")\n";
    return 0;
}
