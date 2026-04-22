#include <bits/stdc++.h>
#include<vector>
#include<string>
using namespace std;

class Account
{
public:
    int accNo;
    string name;
    string type;   //savings or current
    double balance;

    Account()
    {
        accNo=0;
        name="";
        type="Savings";
        balance=0;
    }

    Account(int no, string n, string t, double bal)
    {
        accNo=no;
        name=n;
        type=t;
        balance=bal;
    }

    void deposit(double amt)
    {
        if(amt<=100)
        {
            cout<<"\nAmount should be more than 100!"<< endl;
            return;
        }
        balance=balance+amt;
        cout<<"\nRs."<<amt<<"deposited successfully"<<endl;
        cout<<"Current Balance : Rs."<<balance<<endl;
    }

    void withdraw(double amt)
    {
        if(amt<=100)
        {
            cout<<"\nAmount should be more than 100!"<<endl;
            return;
        }
        if(amt>balance)
        {
            cout<<"\nSorry! Not enough balance."<<endl;
            cout<<"Your balance is only Rs."<<balance<<endl;
            return;
        }
        balance=balance-amt;
        cout<<"\nRs."<<amt<<"withdrawn successfully"<<endl;
        cout<<"Remaining Balance : Rs."<<balance<<endl;
    }

    void showDetails()
    {
        cout<<"\n  ----------------------------------"<<endl;
        cout<<"  Account No  : "<<accNo<<endl;
        cout<<"  Name        : " <<name<<endl;
        cout<<"  Type        : " <<type<<endl;
        cout<<"  Balance     : Rs." << fixed << setprecision(2) << balance << endl;
        cout << "  ----------------------------------" << endl;
    }
};


//file functions

void saveAccounts(vector<Account> &accounts)
{
    ofstream file("accounts.txt");

    for(int i = 0; i < accounts.size(); i++)
    {
        file << accounts[i].accNo << "\n";
        file << accounts[i].name << "\n";
        file << accounts[i].type << "\n";
        file << accounts[i].balance << "\n";
        file << "***" << "\n";   
    }

    file.close();
}

void loadAccounts(vector<Account> &accounts)
{
    ifstream file("accounts.txt");

    if(!file.is_open())
        return;

    accounts.clear();

    while(!file.eof())
    {
        string line;
        Account a;
        string accStr, balStr;

        if(!getline(file, accStr)) break;
        if(accStr == "***" || accStr == "") continue;

        a.accNo = stoi(accStr);
        getline(file, a.name);
        getline(file, a.type);
        getline(file, balStr);
        a.balance = stod(balStr);
        getline(file, line);

        accounts.push_back(a);
    }

    file.close();
}


int nextAccNumber(vector<Account> &accounts)
{
    int max = 1000;
    for(int i = 0; i < accounts.size(); i++)
    {
        if(accounts[i].accNo > max)
            max = accounts[i].accNo;
    }
    return max + 1;
}

//search account by account number and return its position
//returns -1 if not found
int searchByAccNo(vector<Account> &accounts, int no)
{
    for(int i = 0; i < accounts.size(); i++)
    {
        if(accounts[i].accNo == no)
            return i;
    }
    return -1;
}


void createAccount(vector<Account> &accounts)
{
    cout << "\n  ---- Create New Account ----" << endl;

    string name, type;
    double bal;
    int ch;

    cin.ignore();
    cout << "  Enter Your Name   : ";
    getline(cin, name);

    if(name == "")
    {
        cout << "  Name cannot be blank!" << endl;
        return;
    }

    cout << "  Account Type :" << endl;
    cout << "  1. Savings" << endl;
    cout << "  2. Current" << endl;
    cout << "  Choice : ";
    cin >> ch;

    if(ch == 1)
        type = "Savings";
    else if(ch == 2)
        type = "Current";
    else
    {
        cout << "  Wrong choice, setting Savings by default." << endl;
        type = "Savings";
    }

    cout << "  Opening Balance (Rs.) : ";
    cin >> bal;

    if(bal < 0)
    {
        cout << "  Balance cant be negative!" << endl;
        return;
    }

    int newNo = nextAccNumber(accounts);
    Account newAcc(newNo, name, type, bal);
    accounts.push_back(newAcc);

    saveAccounts(accounts);

    cout << "\n  Account created!" << endl;
    cout << "  Your Account Number is : " << newNo << endl;
    cout << "  Please remember this number." << endl;
}

void depositMoney(vector<Account> &accounts)
{
    cout << "\n  ---- Deposit Money ----" << endl;

    int no;
    double amt;

    cout << "  Enter Account Number : ";
    cin >> no;

    int pos = searchByAccNo(accounts, no);
    if(pos == -1)
    {
        cout << "  Account not found!" << endl;
        return;
    }

    cout << "  Enter Amount : Rs.";
    cin >> amt;

    accounts[pos].deposit(amt);
    saveAccounts(accounts);
}

void withdrawMoney(vector<Account> &accounts)
{
    cout << "\n  ---- Withdraw Money ----" << endl;

    int no;
    double amt;

    cout << "  Enter Account Number : ";
    cin >> no;

    int pos = searchByAccNo(accounts, no);
    if(pos == -1)
    {
        cout << "  Account not found!" << endl;
        return;
    }

    cout << "  Enter Amount : Rs.";
    cin >> amt;

    accounts[pos].withdraw(amt);
    saveAccounts(accounts);
}

void checkBalance(vector<Account> &accounts)
{
    cout << "\n  ---- Check Balance ----" << endl;

    int no;
    cout << "  Enter Account Number : ";
    cin >> no;

    int pos = searchByAccNo(accounts, no);
    if(pos == -1)
    {
        cout << "  Account not found!" << endl;
        return;
    }

    accounts[pos].showDetails();
}

void showAllAccounts(vector<Account> &accounts)
{
    cout << "\n  ---- All Accounts ----" << endl;

    if(accounts.size() == 0)
    {
        cout << "  No accounts yet!" << endl;
        return;
    }

    cout << "\n  " << left << setw(12) << "Acc.No"
         << setw(20) << "Name"
         << setw(12) << "Type"
         << "Balance" << endl;
    cout << "  " << string(52, '-') << endl;

    for(int i = 0; i < accounts.size(); i++)
    {
        cout << "  " << left
             << setw(12) << accounts[i].accNo
             << setw(20) << accounts[i].name
             << setw(12) << accounts[i].type
             << "Rs." << fixed << setprecision(2) << accounts[i].balance << endl;
    }

    cout << "  " << string(52, '-') << endl;
    cout << "  Total : " << accounts.size() << " accounts" << endl;
}

void searchAccount(vector<Account> &accounts)
{
    cout << "\n  ---- Search Account ----" << endl;

    int no;
    cout << "  Enter Account Number : ";
    cin >> no;

    int pos = searchByAccNo(accounts, no);
    if(pos == -1)
    {
        cout << "  No account found with this number." << endl;
        return;
    }

    accounts[pos].showDetails();
}

void deleteAccount(vector<Account> &accounts)
{
    cout << "\n  ---- Delete Account ----" << endl;

    int no;
    cout << "  Enter Account Number : ";
    cin >> no;

    int pos = searchByAccNo(accounts, no);
    if(pos == -1)
    {
        cout << "  Account not found!" << endl;
        return;
    }

    cout << "\n  Account Details :";
    accounts[pos].showDetails();

    char ch;
    cout << "  Confirm delete? (y/n) : ";
    cin >> ch;

    if(ch == 'y' || ch == 'Y')
    {
        accounts.erase(accounts.begin() + pos);
        saveAccounts(accounts);
        cout << "  Account deleted." << endl;
    }
    else
    {
        cout << "  Cancelled." << endl;
    }
}

void showMenu()
{
    cout << "\n  ============================" << endl;
    cout << "       BANKING SYSTEM        " << endl;
    cout << "  ============================" << endl;
    cout << "  1. Create Account" << endl;
    cout << "  2. Deposit Money" << endl;
    cout << "  3. Withdraw Money" << endl;
    cout << "  4. Check Balance" << endl;
    cout << "  5. View All Accounts" << endl;
    cout << "  6. Search Account" << endl;
    cout << "  7. Delete Account" << endl;
    cout << "  8. Exit" << endl;
    cout << "  ============================" << endl;
    cout << "  Enter choice : ";
}


int main()
{
    vector<Account> accounts;

    loadAccounts(accounts);  // load old data when program starts

    cout << "\n  Welcome to Banking System" << endl;

    int choice;

    while(true)
    {
        showMenu();
        cin >> choice;

        if(cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "  Please enter a valid number!" << endl;
            continue;
        }

        if(choice == 1)
            createAccount(accounts);
        else if(choice == 2)
            depositMoney(accounts);
        else if(choice == 3)
            withdrawMoney(accounts);
        else if(choice == 4)
            checkBalance(accounts);
        else if(choice == 5)
            showAllAccounts(accounts);
        else if(choice == 6)
            searchAccount(accounts);
        else if(choice == 7)
            deleteAccount(accounts);
        else if(choice == 8)
        {
            cout << "\nBye!" << endl;
            break;
        }
        else
        {
            cout << "  Invalid choice! Try again." << endl;
        }
    }

    return 0;
}