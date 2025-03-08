#include <bits/stdc++.h>
#include "cli/home.h"
#include "utils.h"
#include "database.h"
#include <filesystem>
namespace fs = std::filesystem;

using namespace std;

int main() {
    // unordered_map<string, Book> books = {
    //     {"9780131101630", Book("The C Programming Language", "Kernighan & Ritchie", "Prentice Hall", 1988, "9780131101630", "available", -1, -1, -1)},
    //     {"9780262033848", Book("Introduction to Algorithms", "Cormen et al.", "MIT Press", 2009, "9780262033848", "available", -1, -1, -1)},
    //     {"9780131103627", Book("Computer Networks", "Andrew Tanenbaum", "Prentice Hall", 2010, "9780131103627", "available", -1, -1, -1)},
    //     {"9780201633610", Book("Design Patterns", "Gamma et al.", "Addison-Wesley", 1994, "9780201633610", "available", -1, -1, -1)},
    //     {"9780132350884", Book("Clean Code", "Robert C. Martin", "Prentice Hall", 2008, "9780132350884", "available", -1, -1, -1)},
    //     {"9780321356680", Book("Effective C++", "Scott Meyers", "Addison-Wesley", 2005, "9780321356680", "available", -1, -1, -1)},
    //     {"9780596007126", Book("Head First Design Patterns", "Eric Freeman", "O'Reilly", 2004, "9780596007126", "available", -1, -1, -1)},
    //     {"9780201616224", Book("The Pragmatic Programmer", "Andy Hunt", "Addison-Wesley", 1999, "9780201616224", "available", -1, -1, -1)},
    //     {"9780133594140", Book("Artificial Intelligence: A Modern Approach", "Stuart Russell", "Pearson", 2015, "9780133594140", "available", -1, -1, -1)},
    //     {"9781492052203", Book("System Design Interview", "Alex Xu", "ByteByteGo", 2020, "9781492052203", "available", -1, -1, -1)}
    // };

    // readBooksFrom("data/available_books.json", books);
    // writeBooksTo("data/available_books.json", books);
    // writeAccounts(0, accounts.size(), accounts);

    // cout << GREEN <<  "Library initialized with books and accounts!" <<RESET<< endl;
    int today_date = 0;

    FILEPATH = fs::current_path();
    vector<Account> accounts ;
    int accounts_num = 0;
    fetchAccounts(today_date, accounts_num, accounts); // newpath = FILEPATH/data/accounts.json
    setDate(today_date);
    Account::setAccountsNum(accounts_num);
    while (!displayHome(accounts, false)) {
        // Keep running until the user chooses to exit
    }
    writeAccounts(getDate(), Account::account_num, accounts);
    return 0;
    
}
/*
1) implement JSON functions
2) implement librarian functions
3) add debugging statements
4) testing
5) save before exitting
6) testing
*/