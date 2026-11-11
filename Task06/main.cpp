// Task06 - Q6 : Friend Functions and Inline Functions (Train Journey Planner)
// Name: B. Santhosh Kumar   Register No: 192511053
// Build twice:  g++ -std=c++17 -Wall -Wextra -O0 main.cpp -o task06_O0
//               g++ -std=c++17 -Wall -Wextra -O2 main.cpp -o task06_O2
#include <chrono>
#include <cstdlib>
#include <iostream>

volatile long long g_sink = 0;                  // stops the optimiser deleting the loops
#define NOINLINE __attribute__((noinline))   // GCC/Clang: forbid inlining (simulates a call across files)

// ===========================================================================
// DESIGN A : friend functions (the assignment's design)
// ===========================================================================
class Time {
    int h, m;                                // private: only friends and members can see these
public:
    Time(int hh, int mm) : h(hh), m(mm) {}
    // ---- friend functions (not members: called as f(a, b), no 'this') ----
    friend int minutesBetween(const Time& a, const Time& b);
    friend bool earlier(const Time& a, const Time& b);
    friend int minutesBetweenNI(const Time& a, const Time& b);   // same body, never inlined
    friend bool earlierNI(const Time& a, const Time& b);
    // ---- DESIGN B : member functions (called as a.minutesTo(b)) ----
    inline int minutesTo(const Time& b) const
    {
        int d = (b.h * 60 + b.m) - (h * 60 + m);
        return d < 0 ? -d : d;
    }
    inline bool isEarlier(const Time& b) const { return h * 60 + m < b.h * 60 + b.m; }
    // ---- DESIGN C : public getters (used by free functions that are NOT friends) ----
    int hours() const { return h; }
    int minutes() const { return m; }
};

inline int minutesBetween(const Time& a, const Time& b)
{
    // TODO 1 (done): absolute difference in minutes, using the private members
    int d = (b.h * 60 + b.m) - (a.h * 60 + a.m);
    return d < 0 ? -d : d;
}
inline bool earlier(const Time& a, const Time& b)
{
    // TODO 2 (done): true if a is earlier in the day than b
    return a.h * 60 + a.m < b.h * 60 + b.m;
}
NOINLINE int minutesBetweenNI(const Time& a, const Time& b)
{
    int d = (b.h * 60 + b.m) - (a.h * 60 + a.m);
    return d < 0 ? -d : d;
}
NOINLINE bool earlierNI(const Time& a, const Time& b) { return a.h * 60 + a.m < b.h * 60 + b.m; }

// Design C free functions using the getters
inline int minutesBetweenGet(const Time& a, const Time& b)
{
    int d = (b.hours() * 60 + b.minutes()) - (a.hours() * 60 + a.minutes());
    return d < 0 ? -d : d;
}
inline bool earlierGet(const Time& a, const Time& b)
{
    return a.hours() * 60 + a.minutes() < b.hours() * 60 + b.minutes();
}

// ---------------------------------------------------------------------------
void correctness()
{
    std::cout << "=== Complete the Code program ===\n";
    Time departure(9, 45), arrival(13, 10);
    std::cout << minutesBetween(departure, arrival) << ' ' << std::boolalpha << earlier(arrival, departure) << '\n';

    std::cout << "\n=== Correctness checks (hand-verified) ===\n";
    Time t1(9, 45), t2(9, 45), before(11, 50), after(12, 10), noon(12, 0);
    std::cout << "same time twice  : minutesBetween=" << minutesBetween(t1, t2)
              << " (expect 0), earlier(t1,t2)=" << earlier(t1, t2) << " (expect false)\n";
    std::cout << "11:50 and 12:10  : minutesBetween=" << minutesBetween(before, after)
              << " (expect 20), earlier(11:50,12:10)=" << earlier(before, after) << " (expect true)\n";
    std::cout << "12:10 and 11:50  : minutesBetween=" << minutesBetween(after, before)
              << " (expect 20, order does not matter), earlier=" << earlier(after, before) << " (expect false)\n";
    std::cout << "11:50 and 12:00  : minutesBetween=" << minutesBetween(before, noon) << " (expect 10)\n";
    std::cout << "member versions : " << departure.minutesTo(arrival) << ' ' << arrival.isEarlier(departure)
              << " | getter versions : " << minutesBetweenGet(departure, arrival) << ' ' << earlierGet(arrival, departure) << '\n';
}

template <class F>
double timeIt(F f)
{
    auto t0 = std::chrono::steady_clock::now();
    long long r = f();
    auto t1 = std::chrono::steady_clock::now();
    g_sink = r;
    return std::chrono::duration<double, std::milli>(t1 - t0).count();
}

int main()
{
    correctness();

    const long N = 10000000;
    const Time fixed(13, 10);
    std::cout << "\n=== Growth Investigation: " << N << " calls each ===\n";
#ifdef __OPTIMIZE__
    std::cout << "(compiled with optimisation: -O2)\n";
#else
    std::cout << "(compiled WITHOUT optimisation: -O0)\n";
#endif
    auto make = [](long i) { return Time(static_cast<int>((i / 60) % 24), static_cast<int>(i % 60)); };
    double a1 = timeIt([&] { long long s = 0; for (long i = 0; i < N; ++i) s += minutesBetween(make(i), fixed) + earlier(make(i), fixed); return s; });
    double a2 = timeIt([&] { long long s = 0; for (long i = 0; i < N; ++i) s += minutesBetweenNI(make(i), fixed) + earlierNI(make(i), fixed); return s; });
    double b1 = timeIt([&] { long long s = 0; for (long i = 0; i < N; ++i) s += make(i).minutesTo(fixed) + make(i).isEarlier(fixed); return s; });
    double c1 = timeIt([&] { long long s = 0; for (long i = 0; i < N; ++i) s += minutesBetweenGet(make(i), fixed) + earlierGet(make(i), fixed); return s; });
    std::cout << "friend, inline            : " << a1 << " ms\n";
    std::cout << "friend, NOT inline        : " << a2 << " ms\n";
    std::cout << "member functions (inline) : " << b1 << " ms\n";
    std::cout << "public getters (inline)   : " << c1 << " ms\n";
    return 0;
}
