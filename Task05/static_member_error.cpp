// Evidence for "why a static member function cannot use this or a non-static member".
// Compile:  g++ -std=c++17 -Wall -Wextra static_member_error.cpp -o err    -> compile errors
#include <iostream>
class T {
    static int next;
    int id = 0;
public:
    static int bad() { return id; }          // ERROR: invalid use of member 'id' in static member function
    static int bad2() { return this->id; }   // ERROR: 'this' is unavailable for static member functions
};
int T::next = 0;
int main() { std::cout << T::bad() << '\n'; }
