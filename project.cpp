#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <iomanip>
using namespace std;

class Account {
    int acno;
    string name;
    int deposit;
    char type;

public:
    void create_account() {
        cout << "\nEnter The Account No.: ";
        cin >> acno;
        cin.ignore(); // To consume newline character

        cout << "\nEnter The Name of The Account Holder: ";
        getline(cin, name);

        cout << "\nEnter Type of The Account (C/S): ";
        cin >> type;
        type = toupper(type);

        cout << "\nEnter The Initial Amount (>=500 for Saving and >=1000 for Current): ";
        cin >> deposit;

        cout << "\n\nAccount Created..";
    }

    void show_account() const {
        cout << "\nAccount No.: " << acno;
        cout << "\nAccount Holder Name: " << name;
        cout << "\nType of Account: " << type;
        cout << "\nBalance Amount: " << deposit;
    }

    void modify_account() {
        cout << "\nAccount No.: " << acno;

        cout << "\nModify Account Holder Name: ";
        cin.ignore(); // To consume newline character
        getline(cin, name);

        cout << "\nModify Type of Account: ";
        cin >> type;
        type = toupper(type);

        cout << "\nModify Balance Amount: ";
        cin >> deposit;
    }

    void deposit_amount(int x) {
        deposit += x;
    }

    void withdraw_amount(int x) {
        deposit -= x;
    }

    void report() const {
        cout << setw(10) << acno << setw(20) << name << setw(10) << type << setw(10) << deposit << endl;
    }

    int retacno() const {
        return acno;
    }

    int retdeposit() const {
        return deposit;
    }

    char rettype() const {
        return type;
    }
};

// Global declaration for file stream object and account
fstream fp;
Account ac;

void write_account() {
    fp.open("account.dat", ios::out | ios::app);
    ac.create_account();
    fp.write(reinterpret_cast<char*>(&ac), sizeof(Account));
    fp.close();
}

void display_sp(int n) {
    cout << "\nBALANCE DETAILS\n";
    int flag = 0;
    fp.open("account.dat", ios::in);
    while (fp.read(reinterpret_cast<char*>(&ac), sizeof(Account))) {
        if (ac.retacno() == n) {
            ac.show_account();
            flag = 1;
        }
    }
    fp.close();
    if (flag == 0)
        cout << "\n\nAccount number does not exist";
}

void modify_account() {
    int no, found = 0;
    cout << "\n\n\tTo Modify";
    cout << "\n\n\tEnter The Account No. of The Account: ";
    cin >> no;
    fp.open("account.dat", ios::in | ios::out);
    while (fp.read(reinterpret_cast<char*>(&ac), sizeof(Account)) && found == 0) {
        if (ac.retacno() == no) {
            ac.show_account();
            cout << "\nEnter The New Details of Account:\n";
            ac.modify_account();
            int pos = -1 * static_cast<int>(sizeof(ac));
            fp.seekp(pos, ios::cur);
            fp.write(reinterpret_cast<char*>(&ac), sizeof(Account));
            cout << "\n\n\tRecord Updated";
            found = 1;
        }
    }
    fp.close();
    if (found == 0)
        cout << "\n\nRecord Not Found";
}

void delete_account() {
    int no;
    cout << "\n\n\tDelete Record";
    cout << "\n\nEnter The Account No. of the Customer You Want To Delete: ";
    cin >> no;
    fp.open("account.dat", ios::in | ios::out);
    fstream fp2;
    fp2.open("Temp.dat", ios::out);
    while (fp.read(reinterpret_cast<char*>(&ac), sizeof(Account))) {
        if (ac.retacno() != no) {
            fp2.write(reinterpret_cast<char*>(&ac), sizeof(Account));
        }
    }
    fp2.close();
    fp.close();
    remove("account.dat");
    rename("Temp.dat", "account.dat");
    cout << "\n\n\tRecord Deleted..";
}

void display_all() {
    cout << "\n\n\t\tACCOUNT HOLDER LIST\n\n";
    cout << "====================================================\n";
    cout << "A/c no.       NAME                Type      Balance\n";
    cout << "====================================================\n";
    fp.open("account.dat", ios::in);
    while (fp.read(reinterpret_cast<char*>(&ac), sizeof(Account))) {
        ac.report();
    }
    fp.close();
}

void deposit_withdraw(int option) {
    int no, amt, found = 0;
    cout << "\n\n\tEnter The Account No.: ";
    cin >> no;
    fp.open("account.dat", ios::in | ios::out);
    while (fp.read(reinterpret_cast<char*>(&ac), sizeof(Account)) && found == 0) {
        if (ac.retacno() == no) {
            ac.show_account();
            if (option == 1) {
                cout << "\n\n\tTO DEPOSIT AMOUNT";
                cout << "\n\nEnter The Amount to be Deposited: ";
                cin >> amt;
                ac.deposit_amount(amt);
            } else if (option == 2) {
                cout << "\n\n\tTO WITHDRAW AMOUNT";
                cout << "\n\nEnter The Amount to be Withdrawn: ";
                cin >> amt;
                int bal = ac.retdeposit() - amt;
                if ((bal < 500 && ac.rettype() == 'S') || (bal < 1000 && ac.rettype() == 'C')) {
                    cout << "Insufficient Balance";
                } else {
                    ac.withdraw_amount(amt);
                }
            }
            int pos = -1 * static_cast<int>(sizeof(ac));
            fp.seekp(pos, ios::cur);
            fp.write(reinterpret_cast<char*>(&ac), sizeof(Account));
            cout << "\n\n\tRecord Updated";
            found = 1;
        }
    }
    fp.close();
    if (found == 0)
        cout << "\n\nRecord Not Found";
}

void intro() {
    cout << "\n\tBANKING";
    cout << "\n\tTRANSACTIONS";
    cout << "\n\tSYSTEM";
    cout << "\n\nMADE BY : Triyasa Das";
    cout << "\n\nCOLLEGE : Haldia Institute Of Technology";
}

int main() {
    char ch;
    intro();
    do {
        cout << "\n\n\n\tMAIN MENU";
        cout << "\n\n\t01. NEW ACCOUNT";
        cout << "\n\n\t02. DEPOSIT AMOUNT";
        cout << "\n\n\t03. WITHDRAW AMOUNT";
        cout << "\n\n\t04. BALANCE ENQUIRY";
        cout << "\n\n\t05. ALL ACCOUNT HOLDER LIST";
        cout << "\n\n\t06. CLOSE AN ACCOUNT";
        cout << "\n\n\t07. MODIFY AN ACCOUNT";
        cout << "\n\n\t08. EXIT";
        cout << "\n\n\tSelect Your Option (1-8): ";
        cin >> ch;
        switch (ch) {
        case '1':
            write_account();
            break;
        case '2':
            deposit_withdraw(1);
            break;
        case '3':
            deposit_withdraw(2);
            break;
        case '4': {
            int num;
            cout << "\n\n\tEnter The Account No.: ";
            cin >> num;
            display_sp(num);
            break;
        }
        case '5':
            display_all();
            break;
        case '6':
            delete_account();
            break;
        case '7':
            modify_account();
            break;
        case '8':
            cout << "\nExiting...";
            break;
        default:
            cout << "\a";
        }
    } while (ch != '8');
    return 0;
}