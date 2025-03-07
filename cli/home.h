#ifndef HOME_H
#define HOME_H

#include <iostream>
#include "login_stud_fac.h"
#include "login_lib.h"
#include "../transaction.h"

using namespace std;

bool displayLogin(vector <Account> &accounts) {
    while(true){
        int choice, id;
        cout << BLUE;
        cout << "Login as:\n";
        cout << "Options:\n";
        cout << " 1 : Student\n";
        cout << " 2 : Faculty\n";
        cout << " 3 : Librarian\n";
        cout << " 4 : Go back to homepage\n" << RESET;    
        cout << "Enter your choice: ";

        while (true) {
            cin >> choice;
            if (!cin.fail()) break;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice. Enter a valid choice ";
        }
        if (choice == 4) return false;
        if (choice < 1 || choice > 3) {
            cout << BLUE << "Invalid choice. Please try again.\n" << RESET;
            continue;
        }
        string inp;
        cout << "\n(Enter 'goback' to go back)\nEnter your user-id: ";
        cin >> inp;
        if (inp == "goback") {
            continue;
        } else {
            id = stoi(inp);
        }
        switch (choice) {
            case 1:
                return loginStudFac(id, accounts,true); // returns true if want to exit. returns false if want to display homescreen
                break;
            case 2:
                return loginStudFac(id, accounts, false);
                break;
            case 3:
                return loginLib(id, accounts);
                break;
        }
    }
}

bool displayCreateAccount(vector <Account> &accounts){
    while (true) {
        int choice;
        cout << BLUE;
        cout << "1 : Librarian (Create Account)\n";
        cout << "2 : Go back to Homepage\n";
        cout << "3 : Exit\n";
        cout << YELLOW << "NOTE: Students and Faculty should contact a Librarian to create an account.\n\n" << RESET;
        cout << "Enter your choice: ";

        while (true) {
            cin >> choice;
            if (!cin.fail()) break;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice. Enter a valid choice ";
        }

        switch (choice) {
            case 1:
            {
                Librarian newLibrarian;

                if (newLibrarian.account.id == -1) {
                    cout << RED << "Account not created successfully. Try again." << RESET << endl;
                    continue;
                } else {
                    accounts.push_back(newLibrarian.account);
                    cout << GREEN << "Account created successfully!!!\n Your account as a Librarian with id " << newLibrarian.account.id << " has been created.\n" << RESET;
                }
                return false;
            }
            case 2:
                return false;
            case 3:
                return true;
            default:
                cout << BLUE << "Invalid choice. Please try again.\n" << RESET;
                break;
        }
    }
}

bool displayHome(vector <Account> &accounts, int exit) {
    if (exit) {
        return true;
    }
    int choice;
    
    cout << BLUE;
    cout << "=========================================\n";
    cout << "    WELCOME TO LIBRARY MANAGEMENT SYSTEM\n";
    cout << "=========================================\n\n";
    cout << "Today's date : " << getDate() << endl;
    cout << "Options:\n";
    cout << "  1 : Login\n";
    cout << "  2 : Create an Account\n";
    cout << "  3 : Increment Day\n";
    cout << "  4 : Exit\n";
    cout << "\nEnter your choice: ";
    cout << RESET;
    while (true) {
        cin >> choice;
        if (!cin.fail()) break;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid choice. Enter a valid choice ";
    }

    switch (choice) {
        case 1:
            return displayLogin(accounts); // returns true if want to exit, false if want to display homescreen
            break;
        case 2:
            return displayCreateAccount(accounts);
            break;
        case 3:
            setDate(getDate()+1);
            cout << "Current Date = " << getDate() << endl;
            break;
        case 4:
            return true;
            break;
        default:
            cout << BLUE << "\nInvalid choice. Please try again.\n" << RESET;
            break;
    }
    return false;
}


#endif
