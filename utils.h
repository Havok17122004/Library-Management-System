# ifndef UTILS_H
# define UTILS_H

# include <bits/stdc++.h>
# include "transaction.h"
# include "users.h"
#include <filesystem>


using namespace std;
int TODAYDATE ;

Book getBookDetailsFromUser(bool newbook) {
    string title, author, publisher, isbn;
    int year;
    cout << RESET << "Enter book title: ";
        getline(cin >> ws, title);
    cout << "Enter author: ";
        getline(cin >> ws, author);
    cout << "Enter publisher: ";
        getline(cin >> ws, publisher);
    cout << "Enter publication year: ";
    while (true) {
        cin >> year;
        if (!cin.fail()) break;
        cin.clear();  // Clear error state
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << RED << "Invalid year. Enter a valid year: " << RESET;
    }
    if (newbook) {
    cout << "Enter ISBN : ";
        while (true) {
            cin >> isbn;
            if (!cin.fail()) break;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << RED << "Invalid ISBN. Enter a valid ISBN " << RESET;
        }
    }

    return Book(title, author, publisher, year, isbn);
}



void setDate(int date) {
    TODAYDATE = date;
}

int getDate() {
    return TODAYDATE;
}

#endif