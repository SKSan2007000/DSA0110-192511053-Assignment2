// Task05 - tickets created INSIDE a function that is called twice (fresh counter, capacity 3).
// Compile: g++ -std=c++17 -Wall -Wextra function_demo.cpp -o function_demo
#include <iostream>
#include <string>

class BusTicket {
    static int issued;
    static const int CAPACITY = 3;
    int seat;
    std::string name;
public:
    explicit BusTicket(const std::string& n) : seat(0), name(n)
    {
        if (issued < CAPACITY) { ++issued; seat = issued; }
    }
    ~BusTicket() { std::cout << "   (destroying ticket of " << name << ")\n"; }
    static int count() { return issued; }
    void print() const
    {
        std::cout << "   " << name << ": " << (seat ? "Seat " + std::to_string(seat) : std::string("Waiting")) << '\n';
    }
};
int BusTicket::issued = 0;

void bookTwo(const std::string& a, const std::string& b)
{
    BusTicket t1(a), t2(b);                  // local objects
    t1.print();
    t2.print();
}                                            // destroyed here

int main()
{
    std::cout << "Issued at start: " << BusTicket::count() << '\n';
    std::cout << "Call 1:\n";
    bookTwo("Asha", "Bala");
    std::cout << "Issued after call 1: " << BusTicket::count() << '\n';
    std::cout << "Call 2:\n";
    bookTwo("Chitra", "Dev");
    std::cout << "Issued after call 2: " << BusTicket::count()
              << "  -> the counter survives the objects; Dev got no seat because the one shared counter hit 3\n";
    // Inside a loop it is the same: each iteration's ticket is destroyed, the static keeps counting.
    return 0;
}
