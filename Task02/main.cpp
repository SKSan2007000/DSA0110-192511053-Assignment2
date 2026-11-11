// Task02 - Q2 : Return by Reference and Its Dangers (Examination Marks Register)
// Name: B. Santhosh Kumar   Register No: 192511053
#include <chrono>
#include <iostream>
#include <vector>

// Returns a REFERENCE to the highest mark: the caller can read it, modify it, or assign to the call.
int& highest(int marks[], int n)
{
    int best = 0;
    for (int i = 1; i < n; ++i)
        if (marks[i] > marks[best]) best = i;
    return marks[best];                      // element lives in the caller's array -> safe
}
// Returns a CONST reference: read-only view, no copy, cannot be modified through it.
const int& highestConst(const int marks[], int n)
{
    int best = 0;
    for (int i = 1; i < n; ++i)
        if (marks[i] > marks[best]) best = i;
    return marks[best];
}
// Returns a COPY: an independent value; changing it never touches the array.
int highestCopy(const int marks[], int n) { return highestConst(marks, n); }

// Alternatives for the "Alternative Concept" part: index and pointer
int highestIndex(const int marks[], int n)
{
    int best = 0;
    for (int i = 1; i < n; ++i)
        if (marks[i] > marks[best]) best = i;
    return best;
}
int* highestPtr(int marks[], int n) { return &marks[highestIndex(marks, n)]; }

void print(const char* label, const int marks[], int n)
{
    std::cout << label;
    for (int i = 0; i < n; ++i) std::cout << marks[i] << ' ';
    std::cout << '\n';
}

int main()
{
    int marks[6] = {56, 78, 91, 64, 85, 70};
    print("original marks            : ", marks, 6);

    // 1. Moderation applied directly to the highest mark (+2)
    highest(marks, 6) += 2;
    print("after highest() += 2      : ", marks, 6);

    // 2. Call on the LEFT of '='
    highest(marks, 6) = 95;
    print("after highest() = 95      : ", marks, 6);

    // 3. Reference vs const reference vs copy
    int copy = highestCopy(marks, 6);
    copy += 5;                               // copy only
    const int& view = highestConst(marks, 6);
    // view += 1;                            // ERROR: assignment of read-only reference
    int& ref = highest(marks, 6);
    ref -= 1;                                // changes marks[2]
    std::cout << "copy (+5, array unchanged): " << copy << '\n';
    std::cout << "const ref sees the array  : " << view << " (follows the element: now 94)\n";
    print("after ref -= 1            : ", marks, 6);

    // 4. Changed condition : std::vector that grows
    std::cout << "\n--- std::vector that grows ---\n";
    std::vector<int> v = {56, 78, 91};
    v.shrink_to_fit();
    const int* addressBefore = v.data();     // where the element lives now
    int& r = v[2];                           // reference obtained before growth
    (void)r;
    std::cout << "capacity before growth : " << v.capacity() << '\n';
    v.push_back(60);                         // exceeds capacity -> reallocation
    const int* addressAfter = v.data();
    std::cout << "capacity after growth  : " << v.capacity() << '\n';
    std::cout << "storage moved          : " << std::boolalpha << (addressBefore != addressAfter) << '\n';
    std::cout << "=> 'r' still refers to the OLD freed block: using it now is undefined behaviour,\n"
                 "   so it is NOT used here. Re-obtain the reference after the push_back instead.\n";
    int& fresh = v[2];
    fresh += 2;
    std::cout << "fresh reference works  : v[2] = " << v[2] << '\n';

    // 5. Index or pointer alternatives survive differently
    int idx = highestIndex(marks, 6);
    int* p = highestPtr(marks, 6);
    marks[idx] += 1;
    *p += 1;
    print("index/pointer versions +2 : ", marks, 6);
    std::vector<int> w = {56, 78, 91};
    int widx = highestIndex(w.data(), 3);    // an INDEX stays valid after growth
    w.push_back(60);
    w[widx] += 2;
    std::cout << "index after vector growth : w[" << widx << "] = " << w[widx] << " (still valid)\n";

    // 6. Timing: finding the highest, O(n) in all three
    const int N = 1000000;
    std::vector<int> big(N);
    for (int i = 0; i < N; ++i) big[i] = (i * 7919) % 100003;
    volatile int sink = 0;
    auto time = [&](auto f) {
        auto t0 = std::chrono::steady_clock::now();
        for (int k = 0; k < 20; ++k) sink = sink + f();
        return std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t0).count();
    };
    double t1 = time([&] { return highest(big.data(), N); });
    double t2 = time([&] { return highestIndex(big.data(), N); });
    double t3 = time([&] { return *highestPtr(big.data(), N); });
    std::cout << "\n20 scans of 1,000,000 marks: reference " << t1 << " ms, index " << t2
              << " ms, pointer " << t3 << " ms  (all O(n) time, O(1) extra space)\n";
    return 0;
}
