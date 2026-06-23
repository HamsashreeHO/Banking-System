# Banking System in C++

A Banking Management System developed in C++ using Object-Oriented Programming concepts, demonstrating abstraction, inheritance, polymorphism, exception handling, and operator overloading.

## Features
- Create Savings Account or Current Account
- Deposit Money
- Withdraw Money (with account-specific rules — minimum balance for Savings, overdraft limit for Current)
- View Single Account (with transaction history)
- View All Accounts
- Apply Interest to All Eligible Accounts
- Delete Account
- Check Total Bank Balance
- Track Total Accounts Created
- Custom Exception Handling (Insufficient Funds, Account Not Found)

## Technologies Used
- C++
- OOP Concepts (Abstraction, Inheritance, Polymorphism, Encapsulation)
- STL (`vector`, `unique_ptr`)
- Exception Handling
- Operator Overloading
- VS Code
- GCC Compiler

## OOP Concepts Demonstrated
- **Abstraction** — `Account` is an abstract base class with pure virtual functions (`withdraw()`, `calculateInterest()`, `getAccountType()`)
- **Inheritance** — `SavingsAccount` and `CurrentAccount` inherit from `Account`
- **Polymorphism** — Virtual functions dispatch different behavior per account type (e.g. `calculateInterest()` returns 0 for Current accounts, a computed rate for Savings)
- **Encapsulation** — Private member variables with controlled access via getters and a protected setter
- **Operator Overloading** — `operator<<` for display, `operator==` for comparing accounts by account number
- **Static Members** — `nextAccNo` and `totalAccounts` track state across all accounts
- **Smart Pointers** — `vector<unique_ptr<Account>>` manages account memory automatically
- **Exception Handling** — Custom exceptions (`InsufficientFundsException`, `AccountNotFoundException`) for safe error handling
- **const Correctness** — Read-only methods like `getBalance()` and `totalBalance()` marked `const`

## How to Run

1. Compile the program:
```bash
g++ banking_system.cpp -o bank.exe
```

2. Run the program:
```bash
./bank.exe
```

> Note: On Windows PowerShell, run `.\bank.exe` instead.

## Project Structure
```text
Banking_System/
└── banking_system.cpp
```

## Learning Outcomes
- Abstract Classes and Pure Virtual Functions
- Inheritance and Polymorphism
- Constructors and Destructors
- Operator Overloading
- Custom Exception Classes
- Smart Pointers (`unique_ptr`)
- STL Containers (`vector`)
- const Correctness
