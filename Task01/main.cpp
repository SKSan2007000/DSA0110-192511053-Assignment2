// Task01 - Q1 : Call by Value, by Reference and by Pointer (Hostel Fee Concession Updater)
// Name: B. Santhosh Kumar   Register No: 192511053
#include <chrono>
#include <iostream>
#include <string>

// ---------------------------------------------------------------------------
// PART A : the concession function written three ways
// ---------------------------------------------------------------------------
// 1) By value  : 'fee' is a COPY living in the callee's stack frame.
void applyByValue(long long fee, int percent)
{
    fee -= fee * percent / 100;                 // only the copy changes
    std::cout << "   inside byValue   : fee = " << fee << '\n';
}
// 2) By reference : 'fee' is an alias (no new storage) of the caller's variable.
void applyByReference(long long& fee, int percent)
{
    fee -= fee * percent / 100;
    std::cout << "   inside byReference: fee = " << fee << '\n';
}
// 3) By pointer : 'fee' is a new local variable (8 bytes) holding the caller's address.
void applyByPointer(long long* fee, int percent)
{
    if (fee == nullptr) return;                 // a pointer can be null, a reference cannot
    *fee -= *fee * percent / 100;
    std::cout << "   inside byPointer : fee = " << *fee << '\n';
}

void partA()
{
    std::cout << "=== PART A : three versions (45000, 10% merit concession) ===\n";
    // PREDICTION (written before running):
    //   byValue -> stored fee stays 45000 ; byReference -> 40500 ; byPointer -> 36450 (10% again)
    long long fee = 45000;
    applyByValue(fee, 10);
    std::cout << "   stored fee after byValue    : " << fee << '\n';
    applyByReference(fee, 10);
    std::cout << "   stored fee after byReference: " << fee << '\n';
    applyByPointer(&fee, 10);
    std::cout << "   stored fee after byPointer  : " << fee << '\n';
}

// ---------------------------------------------------------------------------
// PART B : Predict the Output program (given in the assignment)
// ---------------------------------------------------------------------------
namespace predict {
void change(int a, int& b, int* c)
{
    a += 10;
    b += 10;
    *c += 10;
}
int& bigger(int& p, int& q) { return p > q ? p : q; }
void show(int x, int y = 5, int z = 10) { std::cout << x + y + z << ' '; }
void run()
{
    int a = 1, b = 2, c = 3;
    change(a, b, &c);
    std::cout << a << ' ' << b << ' ' << c << '\n';
    bigger(a, c) = 100;
    std::cout << a << ' ' << c << '\n';
    show(1);
    show(1, 2);
    show(1, 2, 3);
    std::cout << '\n';
}
} // namespace predict

// ---------------------------------------------------------------------------
// PART C : Complete the Code (TODO 1 completed)
// ---------------------------------------------------------------------------
namespace complete {
void applyByValue(long long fee, int percent)
{
    fee -= fee * percent / 100;                 // changes only the local copy
}
void applyConcession(long long& fee, int percent)
{
    // TODO 1 (done): reduce the caller's fee by percent using integer arithmetic
    fee -= fee * percent / 100;
}
void run()
{
    long long hostelFee = 45000;
    applyByValue(hostelFee, 10);
    std::cout << "after call by value : " << hostelFee << '\n';
    applyConcession(hostelFee, 10);
    std::cout << "after call by reference: " << hostelFee << '\n';
}
} // namespace complete

// ---------------------------------------------------------------------------
// PART D : Design decision - several concessions, return the new fee
// ---------------------------------------------------------------------------
long long applyConcession(long long fee, int percent) { return fee - fee * percent / 100; }

void partD()
{
    std::cout << "=== PART D : several concessions by returning the new fee ===\n";
    long long fee = 45000;
    long long step = applyConcession(applyConcession(applyConcession(fee, 10), 15), 25);
    std::cout << "   45000 -> merit 10% -> sports 15% -> single parent 25% = " << step
              << "  (original untouched: " << fee << ")\n";
}

// ---------------------------------------------------------------------------
// PART E : Alternative concept - cost of passing a large Student object
// ---------------------------------------------------------------------------
struct Student {                                 // deliberately large object
    char name[64];
    char hostelBlock[64];
    long long history[100];                      // 800 bytes of fee history
    long long fee;
};
long long feeByValue(Student s) { return s.fee; }
long long feeByRef(Student& s) { return s.fee; }
long long feeByConstRef(const Student& s) { return s.fee; }
long long feeByLongLong(long long f) { return f; }

template <class F>
double timeMs(F f)
{
    auto t0 = std::chrono::steady_clock::now();
    f();
    auto t1 = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::milli>(t1 - t0).count();
}

void partE()
{
    std::cout << "=== PART E : bytes copied per call ===\n";
    std::cout << "   sizeof(long long) = " << sizeof(long long)
              << " bytes (by value copies 8, by reference/pointer passes an address of "
              << sizeof(long long*) << ")\n";
    std::cout << "   sizeof(Student)   = " << sizeof(Student)
              << " bytes (by value copies all of it; by reference / const reference "
              << sizeof(Student*) << ")\n";
    static Student s{};
    s.fee = 45000;
    const long N = 5000000;
    volatile long long sink = 0;
    double tv = timeMs([&] { for (long i = 0; i < N; ++i) sink = sink + feeByValue(s); });
    double tr = timeMs([&] { for (long i = 0; i < N; ++i) sink = sink + feeByRef(s); });
    double tc = timeMs([&] { for (long i = 0; i < N; ++i) sink = sink + feeByConstRef(s); });
    double tl = timeMs([&] { for (long i = 0; i < N; ++i) sink = sink + feeByLongLong(i); });
    std::cout << "   " << N << " calls: Student by value " << tv << " ms, by reference " << tr
              << " ms, by const reference " << tc << " ms, long long by value " << tl << " ms\n";
    std::cout << "   (all are O(1) per call; only the copied bytes differ)\n";
}

int main()
{
    partA();
    std::cout << "\n=== PART B : Predict the Output program ===\n";
    predict::run();
    std::cout << "\n=== PART C : Complete the Code program ===\n";
    complete::run();
    std::cout << '\n';
    partD();
    std::cout << '\n';
    partE();
    return 0;
}
