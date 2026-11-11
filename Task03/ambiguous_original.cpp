// Task03 - the developer's ORIGINAL overload set. It does NOT compile (ambiguous call).
// Compile:  g++ -std=c++17 -Wall -Wextra ambiguous_original.cpp -o amb     -> error evidence
#include <iostream>

double charge(double weight) { return weight * 50; }
double charge(double weight, bool express) { return weight * 50 + (express ? 25 : 0); }
double charge(double weight, bool express = false, bool insured = false)
{
    return weight * 50 + (express ? 25 : 0) + (insured ? 20 : 0);
}

int main()
{
    std::cout << charge(2.5) << '\n';        // ERROR: matches (double) AND (double, bool=false, bool=false)
    std::cout << charge(2.5, true) << '\n';  // ERROR: matches (double,bool) AND (double,bool,bool=false)
    return 0;
}
