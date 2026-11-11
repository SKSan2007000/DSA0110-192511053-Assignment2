// Task02 - compiler-warning evidence: returning a reference to a LOCAL variable.
// Compile:  g++ -std=c++17 -Wall -Wextra dangling_demo.cpp -o dangling_demo
// The program is UNDEFINED BEHAVIOUR when it runs - it exists only to capture the warning.
#include <iostream>

int& badHighest(const int marks[], int n)
{
    int best = marks[0];                 // local variable, destroyed when the function returns
    for (int i = 1; i < n; ++i)
        if (marks[i] > best) best = marks[i];
    return best;                         // WARNING: reference to local variable returned
}

int main()
{
    int marks[4] = {56, 78, 91, 64};
    int& r = badHighest(marks, 4);       // r is a dangling reference
    std::cout << "value through dangling reference: " << r << '\n';   // undefined behaviour
    return 0;
}
