// Task04 - Q4 : Specifying a Class with an Array Inside It (Savings Account Mini-Statement)
// Name: B. Santhosh Kumar   Register No: 192511053
// Build for 10 transactions : g++ -std=c++17 -Wall -Wextra main.cpp -o task04
// Build for 50 transactions : g++ -std=c++17 -Wall -Wextra -DHISTORY=50 main.cpp -o task04_50
#include <chrono>
#include <deque>
#include <iostream>

#ifndef HISTORY
#define HISTORY 10                                 // the ONLY line that changes for "last 50"
#endif

struct Transaction {
    char type;                                     // 'D' deposit, 'W' withdrawal
    long long amount;
};

// ===========================================================================
// DESIGN 1 : fixed array, SHIFT when full  (the class the question asks for)
// ===========================================================================
class SavingsAccount {
    long long accountNo;                           // private data: only deposit()/withdraw()
    long long balance;                             // can touch the history, so it can never
    Transaction history[HISTORY];                  // be edited or reordered from outside
    int count;                                     // number of valid entries (<= HISTORY)
    void record(char type, long long amt);         // helper, defined OUTSIDE the class
public:
    explicit SavingsAccount(long long no, long long opening = 0)   // defined INSIDE the class
        : accountNo(no), balance(opening), count(0) {}
    long long getBalance() const { return balance; }               // INSIDE (implicitly inline)
    void deposit(long long amt);                   // OUTSIDE
    bool withdraw(long long amt);                  // OUTSIDE
    void miniStatement() const;                    // OUTSIDE
};

void SavingsAccount::record(char type, long long amt)
{
    if (count == HISTORY) {                        // 11th transaction: discard the oldest
        for (int i = 1; i < HISTORY; ++i)          // shift k-1 elements left  -> O(k)
            history[i - 1] = history[i];
        --count;
    }
    history[count].type = type;
    history[count].amount = amt;
    ++count;
}
void SavingsAccount::deposit(long long amt)
{
    if (amt <= 0) return;
    balance += amt;
    record('D', amt);
}
bool SavingsAccount::withdraw(long long amt)
{
    if (amt <= 0 || amt > balance) return false;   // rejected withdrawals are not recorded
    balance -= amt;
    record('W', amt);
    return true;
}
void SavingsAccount::miniStatement() const
{
    std::cout << "Mini-statement for A/C " << accountNo << " (last " << count << " transactions)\n";
    for (int i = 0; i < count; ++i)
        std::cout << "  " << (history[i].type == 'D' ? "Deposit   " : "Withdrawal") << ' '
                  << history[i].amount << '\n';
    std::cout << "  Balance: " << balance << '\n';
}

// ===========================================================================
// DESIGN 2 : circular buffer with a start index  (O(1) insert)
// ===========================================================================
class SavingsAccountCircular {
    long long accountNo, balance;
    Transaction history[HISTORY];
    int start, count;                              // oldest entry index, number stored
    void record(char type, long long amt)
    {
        int pos = (start + count) % HISTORY;
        if (count == HISTORY) { start = (start + 1) % HISTORY; pos = (start + HISTORY - 1) % HISTORY; }
        else ++count;
        history[pos].type = type;
        history[pos].amount = amt;
    }
public:
    explicit SavingsAccountCircular(long long no, long long opening = 0)
        : accountNo(no), balance(opening), start(0), count(0) {}
    void deposit(long long amt) { if (amt > 0) { balance += amt; record('D', amt); } }
    bool withdraw(long long amt)
    {
        if (amt <= 0 || amt > balance) return false;
        balance -= amt; record('W', amt); return true;
    }
    void miniStatement() const
    {
        std::cout << "[circular] Mini-statement for A/C " << accountNo << " (last " << count << ")\n";
        for (int i = 0; i < count; ++i) {
            const Transaction& t = history[(start + i) % HISTORY];
            std::cout << "  " << (t.type == 'D' ? "Deposit   " : "Withdrawal") << ' ' << t.amount << '\n';
        }
        std::cout << "  Balance: " << balance << '\n';
    }
    long long checksum() const
    {
        long long s = 0;
        for (int i = 0; i < count; ++i) { const Transaction& t = history[(start + i) % HISTORY]; s = s * 31 + t.amount + t.type; }
        return s;
    }
};

// ===========================================================================
// DESIGN 3 : std::deque (push_back / pop_front)
// ===========================================================================
class SavingsAccountDeque {
    long long balance = 0;
    std::deque<Transaction> history;
public:
    void deposit(long long amt)
    {
        balance += amt;
        history.push_back({'D', amt});
        if (history.size() > HISTORY) history.pop_front();
    }
    long long checksum() const
    {
        long long s = 0;
        for (const Transaction& t : history) s = s * 31 + t.amount + t.type;
        return s;
    }
};

// helper copy of the shift account that exposes a checksum for equality testing
class ShiftForBench {
    Transaction h[HISTORY];
    int count = 0;
public:
    void add(long long amt)
    {
        if (count == HISTORY) { for (int i = 1; i < HISTORY; ++i) h[i - 1] = h[i]; --count; }
        h[count++] = {'D', amt};
    }
    long long checksum() const
    {
        long long s = 0;
        for (int i = 0; i < count; ++i) s = s * 31 + h[i].amount + h[i].type;
        return s;
    }
};

int main()
{
    std::cout << "HISTORY (capacity) = " << HISTORY << "\n\n";
    std::cout << "--- Design 1 : shifting array ---\n";
    SavingsAccount a(1001, 5000);
    for (int i = 1; i <= 12; ++i) {                // 12 transactions: the 11th and 12th force a discard
        if (i % 4 == 0) a.withdraw(100 * i);
        else a.deposit(100 * i);
    }
    a.miniStatement();

    std::cout << "\n--- Design 2 : circular buffer (same 12 transactions) ---\n";
    SavingsAccountCircular c(1001, 5000);
    for (int i = 1; i <= 12; ++i) {
        if (i % 4 == 0) c.withdraw(100 * i);
        else c.deposit(100 * i);
    }
    c.miniStatement();

    // ---- timing: 2,000,000 transactions into each design ----
    const long N = 2000000;
    auto t0 = std::chrono::steady_clock::now();
    ShiftForBench s; for (long i = 0; i < N; ++i) s.add(i + 1);
    auto t1 = std::chrono::steady_clock::now();
    SavingsAccountCircular cc(1, 0); for (long i = 0; i < N; ++i) cc.deposit(i + 1);
    auto t2 = std::chrono::steady_clock::now();
    SavingsAccountDeque d; for (long i = 0; i < N; ++i) d.deposit(i + 1);
    auto t3 = std::chrono::steady_clock::now();
    bool same = s.checksum() == cc.checksum() && cc.checksum() == d.checksum();
    auto ms = [](auto x, auto y) { return std::chrono::duration<double, std::milli>(y - x).count(); };
    std::cout << "\nTime to add " << N << " transactions (capacity " << HISTORY << "):\n";
    std::cout << "  shifting array : " << ms(t0, t1) << " ms   O(k) per add\n";
    std::cout << "  circular buffer: " << ms(t1, t2) << " ms   O(1) per add\n";
    std::cout << "  std::deque     : " << ms(t2, t3) << " ms   O(1) per add\n";
    std::cout << "All three designs hold identical last-" << HISTORY << " history: " << std::boolalpha << same << '\n';
    std::cout << "Space: array = " << sizeof(Transaction) * HISTORY << " bytes of history (fixed); circular = same + 1 start index;"
                 " deque = heap blocks + bookkeeping\n";
    return 0;
}
