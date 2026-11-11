# DSA01-192511053-Assignment2

**Name:** B. Santhosh Kumar  
**Register Number:** 192511053  
**Course:** DSA0110 - Object-Oriented Programming with C++  
**Assignment:** 2 (CO2 - functions and object-oriented concepts), Q1-Q7  
**GitHub Repository:** https://github.com/SKSan2007000/DSA01-192511053-Assignment2

## Compiler
`g++ (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0` (g++ 13, C++17)

Compile command used for every program:
```
g++ -std=c++17 -Wall -Wextra main.cpp -o taskNN
```
Variants: Task04 capacity 50 adds `-DHISTORY=50`; Task06 and Task07 are compiled with `-O0` and/or `-O2` (see the table).


## Result of each task
| Task | Question | Result |
|---|---|---|
| Task01 | Q1 call by value / reference / pointer | By value leaves 45000; reference gives 40500; pointer gives 36450. Predict-the-output matched (`1 12 13`, `1 100`, `16 13 6`). Chosen design: return the new fee. |
| Task02 | Q2 return by reference | `highest()+=2` and `highest()=95` work on the array; returning a local reference gives `-Wreturn-local-addr` warning and a crash; vector growth invalidates references. |
| Task03 | Q3 default arguments / overloading | Original set is ambiguous (compile error captured); redesign `charge(int)`, `charge(double,bool=false,bool=false)`, `charge(const Parcel&)`; all predictions matched. |
| Task04 | Q4 class with array | SavingsAccount with 10-entry history; shift vs circular vs deque; capacity 50 changes one constant. |
| Task05 | Q5 static members | Token prints `101 103 104 4`; BusTicket output matches the expected output; counter survives destroyed objects. |
| Task06 | Q6 friend and inline | `205 false`; tests pass; inline matters at -O2 (18.6 ms vs 55.7 ms) and not at -O0. |
| Task07 | Q7 diagnose, redesign, validate | Q4 shifting array redesigned as circular buffer: 1044x faster at k=5000, identical statements. |

`REPORT.md` contains the cover page and the written answers for Q1-Q7.


