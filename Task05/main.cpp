// Task05 - Q5 : Static Members and Arrays of Objects (College Bus Seat Tickets)
// Name: B. Santhosh Kumar   Register No: 192511053
#include <iostream>
#include <string>

// ---------------------------------------------------------------------------
// PART A : Predict the Output program
// PREDICTION (written before running):  101 103 104 4
//   counter[0..2] get 101,102,103 ; extra gets 104 ; issued() = 104 - 100 = 4
//   constructor ran 4 times (3 array elements + 1 object); 'next' starts at 100 because
//   the single shared copy is defined and initialised once, before main(), as 100.
// ---------------------------------------------------------------------------
namespace predict {
class Token {
    static int next;
    int id;
public:
    Token() { id = ++next; }
    int get() const { return id; }
    static int issued() { return next - 100; }
};
int Token::next = 100;
void run()
{
    Token counter[3];
    Token extra;
    std::cout << counter[0].get() << ' ' << counter[2].get() << ' ' << extra.get() << ' '
              << Token::issued() << '\n';
}
} // namespace predict

// ---------------------------------------------------------------------------
// PART B : Complete the Code (TODO 1 and TODO 2 completed)
// ---------------------------------------------------------------------------
class BusTicket {
    static int issued;                       // shared by all BusTicket objects (exists once)
    static const int CAPACITY = 3;
    int seat;
    std::string name;
public:
    explicit BusTicket(const std::string& n) : seat(0), name(n)
    {
        // TODO 1 (done): fewer than CAPACITY issued -> take the next seat, else waiting list
        if (issued < CAPACITY) {
            ++issued;
            seat = issued;
        }
    }
    static int count()
    {
        // TODO 2 (done): number of seats issued so far
        return issued;
    }
    void print() const
    {
        std::cout << name << ": "
                  << (seat ? "Seat " + std::to_string(seat) : std::string("Waiting")) << '\n';
    }
};
int BusTicket::issued = 0;                   // definition of the static data member (once, in ONE .cpp)

// Tickets created inside a function: the objects die, the static counter does NOT.
void bookTwo(const std::string& a, const std::string& b)
{
    BusTicket t1(a), t2(b);
    t1.print();
    t2.print();
}                                            // t1 and t2 destroyed here; 'issued' is unchanged

// ---------------------------------------------------------------------------
// PART C : Alternative concept - a TicketOffice object instead of a static (two buses at once)
// ---------------------------------------------------------------------------
class TicketOffice {                         // one counter PER bus
    int capacity, issued = 0;
public:
    explicit TicketOffice(int cap) : capacity(cap) {}
    int nextSeat() { return issued < capacity ? ++issued : 0; }
    int count() const { return issued; }
};
class OfficeTicket {
    int seat;
    std::string name;
public:
    OfficeTicket(const std::string& n, TicketOffice& office) : seat(office.nextSeat()), name(n) {}
    void print() const
    {
        std::cout << "  " << name << ": " << (seat ? "Seat " + std::to_string(seat) : std::string("Waiting")) << '\n';
    }
};

int main()
{
    std::cout << "=== PART A : Predict the Output program ===\n";
    predict::run();

    std::cout << "\n=== PART B : Complete the Code program ===\n";
    BusTicket trip[4] = {BusTicket("Asha"), BusTicket("Bala"), BusTicket("Chitra"), BusTicket("Dev")};
    for (const BusTicket& t : trip) t.print();
    std::cout << "Issued: " << BusTicket::count() << '\n';

    std::cout << "\n=== PART B2 : bigger array than capacity + tickets created in a function ===\n";
    std::cout << "Count before calling the function: " << BusTicket::count() << " (capacity already full)\n";
    bookTwo("Esha", "Farid");
    bookTwo("Gita", "Hari");
    std::cout << "Issued after two calls: " << BusTicket::count()
              << " (objects destroyed, static counter and seats are NOT given back)\n";

    std::cout << "\n=== PART C : TicketOffice per bus (two buses at once) ===\n";
    TicketOffice busA(2), busB(2);
    OfficeTicket a1("Ila", busA), a2("Jeev", busA), a3("Kavi", busA);
    OfficeTicket b1("Lata", busB), b2("Mani", busB);
    std::cout << "Bus A (capacity 2):\n"; a1.print(); a2.print(); a3.print();
    std::cout << "Bus B (capacity 2):\n"; b1.print(); b2.print();
    std::cout << "Issued A = " << busA.count() << ", Issued B = " << busB.count() << '\n';
    return 0;
}
