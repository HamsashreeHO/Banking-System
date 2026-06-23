// Banking System - OOP demonstration in C++
// Concepts: Encapsulation, Inheritance, Polymorphism, Abstraction,
//           Exception Handling, Operator Overloading, Static Members,
//           Smart Pointers, Composition, const correctness

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <iomanip>
#include <exception>

using namespace std;


// ---------- EXCEPTION CLASSES ----------

class InsufficientFundsException : public exception {
private:
    string message;
public:
    InsufficientFundsException(double requested, double available) {
        message = "Requested: Rs." + to_string(requested)
                + " | Available: Rs." + to_string(available);
    }
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class AccountNotFoundException : public exception {
public:
    const char* what() const noexcept override {
        return "Account not found.";
    }
};


// ---------- ABSTRACT BASE CLASS ----------
// Account is abstract — cannot be instantiated directly.
// Every subclass must implement withdraw(), calculateInterest(), getAccountType().

class Account {

private:
    int    accNo;
    string ownerName;
    double balance;
    vector<string> transactions;

    static int nextAccNo;
    static int totalAccounts;

protected:
    void setBalance(double amt) { balance = amt; }

public:
    Account(string name, double initialDeposit) {
        accNo    = nextAccNo++;
        ownerName = name;
        balance   = initialDeposit;
        totalAccounts++;
        addTransaction("Account opened with Rs." + to_string(initialDeposit));
    }

    static int getTotalAccounts() { return totalAccounts; }

    int    getAccNo()   const { return accNo; }
    string getName()    const { return ownerName; }
    double getBalance() const { return balance; }

    void deposit(double amt) {
        if (amt <= 0) { cout << "  Amount must be positive.\n"; return; }
        balance += amt;
        addTransaction("Deposited Rs." + to_string(amt));
    }

    void addTransaction(string record) {
        transactions.push_back(record);
    }

    void showTransactions() const {
        cout << "\n  --- Transaction History ---\n";
        if (transactions.empty()) { cout << "  No transactions yet.\n"; return; }
        for (const string& t : transactions)
            cout << "  > " << t << "\n";
    }

    // Pure virtual functions — make Account abstract
    virtual void   withdraw(double amt)      = 0;
    virtual double calculateInterest() const = 0;
    virtual string getAccountType()    const = 0;

    // Virtual — subclasses override this to display their extra fields
    virtual void display() const {
        cout << "\n  ==============================\n";
        cout << "  Type    : " << getAccountType() << "\n";
        cout << "  Acc No  : " << accNo << "\n";
        cout << "  Name    : " << ownerName << "\n";
        cout << "  Balance : Rs." << fixed << setprecision(2) << balance << "\n";
    }

    friend ostream& operator<<(ostream& os, const Account& a) {
        os << "  [" << a.getAccountType() << "]"
           << "  No: " << a.accNo
           << "  |  " << a.ownerName
           << "  |  Rs." << fixed << setprecision(2) << a.balance;
        return os;
    }

    bool operator==(const Account& other) const {
        return accNo == other.accNo;
    }

    // Deleted — copying would create duplicate account numbers
    Account(const Account&)            = delete;
    Account& operator=(const Account&) = delete;

    virtual ~Account() = default;
};

int Account::nextAccNo    = 1001;
int Account::totalAccounts = 0;


// ---------- SAVINGS ACCOUNT ----------

class SavingsAccount : public Account {

private:
    double interestRate;
    double minBalance;

public:
    SavingsAccount(string name, double bal, double rate = 4.0, double minBal = 500.0)
        : Account(name, bal), interestRate(rate), minBalance(minBal) {}

    void withdraw(double amt) override {
        if (amt <= 0) { cout << "  Amount must be positive.\n"; return; }
        if (getBalance() - amt < minBalance)
            throw InsufficientFundsException(amt, getBalance() - minBalance);
        setBalance(getBalance() - amt);
        addTransaction("Withdrew Rs." + to_string(amt));
        cout << "  Done. Balance: Rs." << fixed << setprecision(2) << getBalance() << "\n";
    }

    double calculateInterest() const override {
        return (getBalance() * interestRate) / 100.0;
    }

    string getAccountType() const override { return "Savings"; }

    void display() const override {
        Account::display();
        cout << "  Interest : " << interestRate << "%\n";
        cout << "  Min Bal  : Rs." << fixed << setprecision(2) << minBalance << "\n";
        cout << "  Int Due  : Rs." << fixed << setprecision(2) << calculateInterest() << "\n";
        cout << "  ==============================\n";
    }
};


// ---------- CURRENT ACCOUNT ----------

class CurrentAccount : public Account {

private:
    double overdraftLimit;
    string businessName;

public:
    CurrentAccount(string name, double bal, string biz = "", double overdraft = 10000.0)
        : Account(name, bal), overdraftLimit(overdraft), businessName(biz) {}

    // Different withdraw behavior vs SavingsAccount — this is polymorphism
    void withdraw(double amt) override {
        if (amt <= 0) { cout << "  Amount must be positive.\n"; return; }
        if (amt > getBalance() + overdraftLimit)
            throw InsufficientFundsException(amt, getBalance() + overdraftLimit);
        setBalance(getBalance() - amt);
        addTransaction("Withdrew Rs." + to_string(amt));
        cout << "  Done. Balance: Rs." << fixed << setprecision(2) << getBalance() << "\n";
    }

    double calculateInterest() const override { return 0.0; }

    string getAccountType() const override { return "Current"; }

    void display() const override {
        Account::display();
        cout << "  Business  : " << (businessName.empty() ? "N/A" : businessName) << "\n";
        cout << "  Overdraft : Rs." << fixed << setprecision(2) << overdraftLimit << "\n";
        cout << "  ==============================\n";
    }
};


// ---------- BANK ----------
// Owns all accounts via vector<unique_ptr<Account>>.
// Storing base class pointers is what makes virtual dispatch work.

class Bank {

private:
    string bankName;
    vector<unique_ptr<Account>> accounts;

    Account* findAccount(int accNo) {
        for (auto& acc : accounts)
            if (acc->getAccNo() == accNo)
                return acc.get();
        return nullptr;
    }

public:
    Bank(string name) : bankName(name) {}

    void createSavingsAccount(string name, double bal) {
        accounts.push_back(make_unique<SavingsAccount>(name, bal));
        cout << "  Account created. No: " << accounts.back()->getAccNo() << "\n";
    }

    void createCurrentAccount(string name, double bal, string biz = "") {
        accounts.push_back(make_unique<CurrentAccount>(name, bal, biz));
        cout << "  Account created. No: " << accounts.back()->getAccNo() << "\n";
    }

    void deposit(int accNo, double amt) {
        Account* acc = findAccount(accNo);
        if (!acc) throw AccountNotFoundException();
        acc->deposit(amt);
        cout << "  New balance: Rs." << fixed << setprecision(2) << acc->getBalance() << "\n";
    }

    void withdraw(int accNo, double amt) {
        Account* acc = findAccount(accNo);
        if (!acc) throw AccountNotFoundException();
        acc->withdraw(amt);   // calls SavingsAccount or CurrentAccount version automatically
    }

    void displayAccount(int accNo) {
        Account* acc = findAccount(accNo);
        if (!acc) throw AccountNotFoundException();
        acc->display();
        acc->showTransactions();
    }

    void displayAll() const {
        cout << "\n  ====== " << bankName << " ======\n";
        if (accounts.empty()) { cout << "  No accounts.\n"; return; }
        for (const auto& acc : accounts)
            cout << *acc << "\n";
        cout << "  ==========================\n";
    }

    void deleteAccount(int accNo) {
        for (auto it = accounts.begin(); it != accounts.end(); ++it) {
            if ((*it)->getAccNo() == accNo) {
                accounts.erase(it);
                cout << "  Account " << accNo << " deleted.\n";
                return;
            }
        }
        throw AccountNotFoundException();
    }

    double totalBalance() const {
        double total = 0;
        for (const auto& acc : accounts) total += acc->getBalance();
        return total;
    }

    // Polymorphism demo — same loop works for both account types.
    // calculateInterest() is virtual, so savings gets credited, current gets 0.
    void applyInterestToAll() {
        cout << "\n  Applying interest...\n";
        bool any = false;
        for (auto& acc : accounts) {
            double interest = acc->calculateInterest();
            if (interest > 0) {
                acc->deposit(interest);
                cout << "  Rs." << fixed << setprecision(2) << interest
                     << " credited to Acc " << acc->getAccNo() << "\n";
                any = true;
            }
        }
        if (!any) cout << "  No accounts eligible.\n";
    }
};


// ---------- MAIN ----------

void showMenu() {
    cout << "\n  ===== BANKING SYSTEM =====\n";
    cout << "  1.  New Savings Account\n";
    cout << "  2.  New Current Account\n";
    cout << "  3.  Deposit\n";
    cout << "  4.  Withdraw\n";
    cout << "  5.  View Account\n";
    cout << "  6.  View All\n";
    cout << "  7.  Apply Interest\n";
    cout << "  8.  Delete Account\n";
    cout << "  9.  Total Balance\n";
    cout << "  10. Total Accounts Created\n";
    cout << "  0.  Exit\n";
    cout << "  Choice: ";
}

int main() {
    Bank bank("My Bank");

    

    int choice;
    do {
        showMenu();
        cin >> choice;

        try {
            if (choice == 1) {
                string name; double bal;
                cout << "  Name: ";             cin >> name;
                cout << "  Opening balance: ";  cin >> bal;
                bank.createSavingsAccount(name, bal);
            }
            else if (choice == 2) {
                string name, biz; double bal;
                cout << "  Name: ";             cin >> name;
                cout << "  Opening balance: ";  cin >> bal;
                cout << "  Business name: ";    cin >> biz;
                bank.createCurrentAccount(name, bal, biz);
            }
            else if (choice == 3) {
                int accNo; double amt;
                cout << "  Account No: ";  cin >> accNo;
                cout << "  Amount: ";      cin >> amt;
                bank.deposit(accNo, amt);
            }
            else if (choice == 4) {
                int accNo; double amt;
                cout << "  Account No: ";  cin >> accNo;
                cout << "  Amount: ";      cin >> amt;
                bank.withdraw(accNo, amt);
            }
            else if (choice == 5) {
                int accNo;
                cout << "  Account No: ";  cin >> accNo;
                bank.displayAccount(accNo);
            }
            else if (choice == 6)  { bank.displayAll(); }
            else if (choice == 7)  { bank.applyInterestToAll(); }
            else if (choice == 8) {
                int accNo;
                cout << "  Account No: ";  cin >> accNo;
                bank.deleteAccount(accNo);
            }
            else if (choice == 9) {
                cout << "  Total: Rs." << fixed << setprecision(2)
                     << bank.totalBalance() << "\n";
            }
            else if (choice == 10) {
                cout << "  Total accounts created: "
                     << Account::getTotalAccounts() << "\n";
            }
            else if (choice != 0) { cout << "  Invalid choice.\n"; }
        }
        catch (const InsufficientFundsException& e) {
            cout << "\n  ERROR: " << e.what() << "\n";
        }
        catch (const AccountNotFoundException& e) {
            cout << "\n  ERROR: " << e.what() << "\n";
        }
        catch (const exception& e) {
            cout << "\n  ERROR: " << e.what() << "\n";
        }

    } while (choice != 0);

    cout << "\n  Goodbye!\n";
    return 0;
}