// Task07 - Q7 : Diagnose, Redesign and Validate
// Name: B. Santhosh Kumar   Register No: 192511053
//
// PROGRAM CHOSEN : Q4 SavingsAccount mini-statement (array of last-k transactions, SHIFT when full).
//
// A. DIAGNOSIS (from my own Task04 evidence): when the history is full, every new transaction shifts
//    k-1 elements left. Task04 measured 2,000,000 adds at ~41 ms for k=10 and ~112 ms for k=50
//    (about 2.7x slower for 5x the capacity): cost per add grows with k because the design MOVES DATA
//    to keep the oldest entry at index 0.
// B. OBJECTIVE   : At capacity k = 5000 the redesign must add a transaction at least 100x faster
//    than the shifting design, its time per add must stay within 2x between k = 10 and k = 5000
//    (i.e. independent of k), and it must print exactly the same mini-statement.
// D. PREDICTION (written BEFORE running this program):
//    shifting design : ns per add grows roughly linearly with k (k=500 about 50x the k=10 cost)
//    circular design : ns per add roughly constant (about 1-3 ns) for every k
//    speed-up        : >= 10x at k=500 and >= 100x at k=5000; statements identical.
//
// Build: g++ -std=c++17 -Wall -Wextra -O2 main.cpp -o task07
#include <chrono>
#include <iostream>
#include <sstream>
#include <string>

struct Transaction {
    char type;
    long long amount;
};

// ------------------------- ORIGINAL : shift when full -------------------------
class AccountShift {
    Transaction* h;
    int cap, count;
public:
    explicit AccountShift(int k) : h(new Transaction[k]), cap(k), count(0) {}
    ~AccountShift() { delete[] h; }
    AccountShift(const AccountShift&) = delete;
    AccountShift& operator=(const AccountShift&) = delete;
    void add(char type, long long amt)
    {
        if (count == cap) {
            for (int i = 1; i < cap; ++i) h[i - 1] = h[i];     // O(k) data movement
            --count;
        }
        h[count].type = type;
        h[count].amount = amt;
        ++count;
    }
    std::string statement() const
    {
        std::ostringstream os;
        for (int i = 0; i < count; ++i) os << h[i].type << h[i].amount << ';';
        return os.str();
    }
    long long checksum() const
    {
        long long s = 0;
        for (int i = 0; i < count; ++i) s = s * 31 + h[i].amount + h[i].type;
        return s;
    }
};

// ------------------------- REDESIGN : circular buffer -------------------------
class AccountCircular {
    Transaction* h;
    int cap, start, count;                    // one extra int (start) is the only new memory
public:
    explicit AccountCircular(int k) : h(new Transaction[k]), cap(k), start(0), count(0) {}
    ~AccountCircular() { delete[] h; }
    AccountCircular(const AccountCircular&) = delete;
    AccountCircular& operator=(const AccountCircular&) = delete;
    void add(char type, long long amt)
    {
        int pos;
        if (count == cap) {                   // overwrite the oldest entry, move 'start' forward
            pos = start;
            start = (start + 1 == cap) ? 0 : start + 1;
        } else {
            pos = (start + count >= cap) ? start + count - cap : start + count;
            ++count;
        }
        h[pos].type = type;                   // O(1): no element is moved
        h[pos].amount = amt;
    }
    std::string statement() const
    {
        std::ostringstream os;
        for (int i = 0; i < count; ++i) {
            const Transaction& t = h[(start + i) % cap];
            os << t.type << t.amount << ';';
        }
        return os.str();
    }
    long long checksum() const
    {
        long long s = 0;
        for (int i = 0; i < count; ++i) { const Transaction& t = h[(start + i) % cap]; s = s * 31 + t.amount + t.type; }
        return s;
    }
};

template <class A>
double run(A& acc, long n)
{
    auto t0 = std::chrono::steady_clock::now();
    for (long i = 0; i < n; ++i) acc.add(i % 3 == 0 ? 'W' : 'D', i + 1);
    auto t1 = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::nano>(t1 - t0).count() / static_cast<double>(n);   // ns per add
}

int main()
{
    // ---- E1: identical-behaviour test on a small case (k = 10, 25 transactions) ----
    {
        AccountShift a(10);
        AccountCircular c(10);
        for (int i = 1; i <= 25; ++i) { a.add(i % 2 ? 'D' : 'W', i * 100); c.add(i % 2 ? 'D' : 'W', i * 100); }
        std::cout << "Original statement  : " << a.statement() << '\n';
        std::cout << "Redesigned statement: " << c.statement() << '\n';
        std::cout << "Identical mini-statement: " << std::boolalpha << (a.statement() == c.statement()) << "\n\n";
    }

    // ---- E2: same measurements for both designs ----
    const long N = 200000;
    const int ks[] = {10, 50, 500, 5000};
    double shiftNs[4], circNs[4];
    std::cout << "ns per add transaction (" << N << " adds into a full history)\n";
    std::cout << "     k |  original(shift) | redesign(circular) | speed-up | same history\n";
    for (int j = 0; j < 4; ++j) {
        AccountShift a(ks[j]);
        AccountCircular c(ks[j]);
        shiftNs[j] = run(a, N);
        circNs[j] = run(c, N);
        bool same = a.checksum() == c.checksum();
        std::cout.width(6);  std::cout << ks[j] << " | ";
        std::cout.width(16); std::cout << shiftNs[j] << " | ";
        std::cout.width(18); std::cout << circNs[j] << " | ";
        std::cout.width(8);  std::cout << shiftNs[j] / circNs[j] << " | " << same << '\n';
    }

    // ---- objective check ----
    bool o1 = shiftNs[3] / circNs[3] >= 100.0;
    bool o2 = circNs[3] / circNs[0] <= 2.0;
    std::cout << "\nObjective 1 (>=100x faster at k=5000)           : " << (o1 ? "ACHIEVED" : "NOT achieved")
              << "  (" << shiftNs[3] / circNs[3] << "x)\n";
    std::cout << "Objective 2 (time at k=5000 within 2x of k=10)   : " << (o2 ? "ACHIEVED" : "NOT achieved")
              << "  (ratio " << circNs[3] / circNs[0] << ")\n";
    std::cout << "Space: original " << sizeof(Transaction) << "*k bytes; redesign the same + 4 bytes (start index)\n";
    return 0;
}
