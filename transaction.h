#ifndef TRANSACTION_H
#define TRANSACTION_H
#define BLUE "\033[34m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"


#include <bits/stdc++.h>
using namespace std;

class Book {
    public:
    string title;
    string author;
    string publisher;
    int year;
    string isbn; // 13 digit isbn = 44 bit number. can use long long.
    string status;
    int borrowing_user;
    int borrow_date;
    int due_date;

    Book(string title = "", string author = "", string publisher = "", int year = 0, string isbn = "", string status = "available", int borrowing_user = -1, int borrow_date = -1, int due_date = -1){
        this->title = title;
        this->author = author;
        this->publisher = publisher;
        this->year = year;
        this->isbn = isbn;
        this->status = status;
        this->borrow_date = borrow_date;
        this->borrowing_user = borrowing_user;
        this->due_date = due_date;
    }
    void printBook() {
        cout << BLUE;
        cout << "ISBN: " << isbn << "\n";
        cout << "Title: " << title << "\n";
        cout << "Author: " << author << "\n";
        cout << "Publisher: " << publisher << "\n";
        cout << "Year: " << year << "\n";
        cout << "Status: " << status << RESET << "\n";

        if (status == "borrowed") {
            cout << YELLOW;
            cout << "Borrowing User: " << borrowing_user << "\n";
            cout << "Borrow Date: " << borrow_date << "\n";
            cout << "Due Date: " << due_date << RESET << "\n";
        }
        cout << endl << endl;
    }

    

};


class Account{
    public:
    int id;
    string user_type;
    int amount_paid;
    static int account_num;

    Account() {
        id = -1;
        user_type = "";
        amount_paid = 0;
    }

    static void setAccountsNum(int accounts_num);

    Account(int id, string user_type="", int amount_paid=0) {
        this->id = id;
        this->user_type = user_type;
        this->amount_paid = amount_paid;
    }

};

int Account::account_num = 0;
void Account::setAccountsNum (int accounts_num) {
    Account::account_num = accounts_num;
}

#endif