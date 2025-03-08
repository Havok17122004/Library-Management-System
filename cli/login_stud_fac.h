#ifndef LOGIN_STUD_FAC_H
#define LOGIN_STUD_FAC_H

// ANSI escape codes for blue color and reset
#define BLUE "\033[34m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"
#define GREEN "\033[32m"

#include <bits/stdc++.h>
#include "../transaction.h"
#include "../users.h"

using namespace std;
// returns false if want to go to the homepage
// returns true if want to exit
bool loginStudFac(int &id, vector <Account> &accounts, bool student) {
    bool found = false;
    bool exit;
    if (student) {
        int payment = 0;
        int x;
        for(int i = 0; i < accounts.size(); i++) {
            if (accounts[i].id == id && accounts[i].user_type == "student") {
                found = true;
                x = i;
                payment = accounts[i].amount_paid;
                break;
            } 
        }
        
        if (!found) {
            cout << RED << "Log in unsuccessful\nPlease try again\n" << RESET;
            // displayHome(false);
            return false;
        }
        Student stud(id, payment);
        cout << GREEN << "Log in successful\n Your account as a Student with id " << stud.account.id << " has been logged in.\n" << RESET;
        
        
        while (true) {
            int choice;
            cout << YELLOW << "Current Date = " << getDate() << endl << RESET;
            cout << BLUE;
            cout << "Logged in as: Student - " << id << endl;
            cout << "Total borrowed books: " << YELLOW << stud.borrowed_books.size() << BLUE << ". You can borrow up to " 
                 << YELLOW << stud.MAX_BORROWABLE_BOOKS - stud.borrowed_books.size() << BLUE<< " more books.\n\n";
            cout << "Options:\n";
            cout << "  1 : View borrowed books\n";
            cout << "  2 : View available books\n";
            cout << "  3 : View current dues/ Pay dues\n";
            cout << "  4 : Borrow Book\n";
            cout << "  5 : Return Book\n";
            // cout << "  6 : Pay dues\n";
            cout << "  6 : View history\n";
            cout << "  7 : Logout\n";
            cout << "  8 : Logout and exit\n\n" << RESET;

            int net = 0;
            for (const auto& pair : stud.borrowed_books) {
                const Book& book = pair.second;
                if (getDate() > book.due_date) {
                    int fine = (getDate() - book.due_date) * stud.FINE_PER_BOOK_PER_DAY;
                    net += fine;

                    cout << YELLOW << "Book with ISBN: " << book.isbn << " is overdue!" << RESET << endl;
                    cout << YELLOW << "Due Date: " << book.due_date << " | Current Date: " << getDate() << " | Overdue Period: " <<  -book.due_date+ getDate() <<  RESET << endl;
                    cout << YELLOW << "Fine Accumulated: " << fine << " | Please return this book to prevent further fines.\n" << RESET << endl;
                }
            }
            int history_fine = 0;
            for (const auto& due : stud.dues) {
                if (due.return_date > due.due_date) {
                    int fine = (due.return_date - due.due_date) * stud.FINE_PER_BOOK_PER_DAY;
                    history_fine += fine;

                    cout << YELLOW << "Book with ISBN: " << due.isbn << " had overdue fines!" << RESET << endl;
                    cout << YELLOW << "Due Date: " << due.due_date << " | Return Date: " << due.return_date 
                        << " | Overdue Period: " << (due.return_date - due.due_date) << " days" << RESET << endl;
                    cout << YELLOW << "Fine Accumulated: " << fine << " | If unpaid, it is included in your total fine.\n" << RESET << endl;
                    cout << YELLOW << "You have already paid: " << stud.account.amount_paid << ". Please pay the remaining amount as soon as possible\n\n" << RESET;
                }
            }

            if (net + history_fine != 0) cout << RED << "\nIncluding previous unpaid fines, your total fine is: " << net + history_fine << RESET << endl;
            cout << "Enter your choice: ";
            while (true) {
                cin >> choice;
                if (!cin.fail()) break;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << RED << "Invalid choice. Enter a valid choice " << RESET;
            }

            if (choice == 7) {
                exit = false;
                break;
            } else if (choice == 8) {
                exit = true;
                break;
            }
            
            switch (choice) {
                case 1:
                    stud.viewBorrowed();
                    break;
                case 2:
                    stud.viewAvailable();
                    break;
                case 3:
                    stud.viewPayDues();
                    break;
                case 4:
                {
                    if (stud.borrowed_books.size() >= stud.MAX_BORROWABLE_BOOKS) {
                        cout << RED << "You have reached the maximum borrowing limit of " 
                            << stud.MAX_BORROWABLE_BOOKS << " books. Please return at least one book to borrow more." 
                            << RESET << endl;
                    } else if (stud.total_fine > 0) {
                        cout << RED << "You have an outstanding fine of ₹" << stud.total_fine 
                            << ". Please clear your dues before borrowing new books." 
                            << RESET << endl;
                    } else {
                        stud.borrowBook();
                    }
                }
                break;

                case 5:
                {
                    if (stud.borrowed_books.size() == 0) {
                        cout << RED << "You cannot return a book as you haven't borrowed any." << RESET << endl;
                    } else {
                        string isbn;
                        cout << "\n(Enter 'goback' to go back)\nEnter book ISBN: ";
                        string inp;
                        cin >> inp;
                        if (inp == "goback") {
                            break;
                        } else {
                            isbn = inp;
                        }
                        stud.returnBook(isbn);
                    }
                }
                break;
                case 6:
                    stud.viewHistory();
                    break;
                default:
                    cout << BLUE << "Invalid choice. Please try again.\n" << RESET;
            }
        }
        accounts[x] = stud.account;
        stud.saveData();
    } else {
        int x;
        for(int i = 0; i < accounts.size(); i++) {
            if (accounts[i].id == id && accounts[i].user_type == "faculty") {
                found = true;
                x = i;
                break;
            } 
        }
        if (!found) {
            cout << RED << "Log in unsuccessful\nPlease try again\n" << RESET;
            // displayHome(false);
            return false;
        }
        Faculty fac(id);
        cout << GREEN << "Log in successful\n Your account as a Faculty with id " << fac.account.id << " has been logged in.\n" << RESET;

        while (true) {
            int choice;
            cout << YELLOW << "Current Date = " << getDate() << endl << RESET;
            cout << BLUE;
            cout << "Logged in as: Faculty - " << id << endl;
            cout << "Total borrowed books: " << YELLOW << fac.borrowed_books.size() << BLUE << ". You can borrow up to "  << YELLOW
                 << fac.MAX_BORROWABLE_BOOKS - fac.borrowed_books.size()  << BLUE << " more books.\n\n";
            cout << "Options:\n";
            cout << "  1 : View borrowed books\n";
            cout << "  2 : View available books\n";
            cout << "  3 : Borrow Book\n";
            cout << "  4 : Return Book\n";
            cout << "  5 : View history\n";
            cout << "  6 : Logout\n";
            cout << "  7 : Logout and exit\n\n" << RESET;
            for (const auto& pair : fac.borrowed_books) {
                const Book& book = pair.second;
                if (getDate() > book.due_date) {
                    cout << RED << "Book with ISBN: " << book.isbn << " is overdue!" << RESET << endl;
                    cout << RED << "Due Date: " << book.due_date << " | Current Date: " << getDate() << " | Overdue Period: " <<  -book.due_date+ getDate() << RESET << endl;
                }
            }
            cout << "Enter your choice: ";
            while (true) {
                cin >> choice;
                if (!cin.fail()) break;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << RED << "Invalid choice. Enter a valid choice " << RESET;
            }
            if (choice == 6) {
                exit = false;
                break;
            } else if (choice == 7) {
                exit = true;
                break;
            }
            switch (choice) {
                case 1:
                    fac.viewBorrowed();
                    break;
                case 2:
                    fac.viewAvailable();
                    break;
                case 3:
                    {
                    if (fac.borrowed_books.size() >= fac.MAX_BORROWABLE_BOOKS) {
                        cout << RED << "You have reached the maximum borrowing limit of " 
                            << fac.MAX_BORROWABLE_BOOKS << " books. Please return at least one book to borrow more." 
                            << RESET << endl;
                    } else if (fac.max_due_period > fac.MAX_DUE_PERIOD_ALLOWABLE){
                        cout << RED << "At least one of your book exceeds the maximum allowed overdue of " 
                            << fac.MAX_DUE_PERIOD_ALLOWABLE << " days. Please return that book to borrow more." 
                            << RESET << endl;
                    }else{
                        fac.borrowBook();
                    }
                }
                    break;
                case 4:
                    {
                    if (fac.borrowed_books.size() == 0) {
                        cout << RED << "You cannot return a book as you haven't borrowed any." << RESET << endl;
                    } else {
                        string isbn;
                        cout << "\n(Enter 'goback' to go back)\nEnter book ISBN: ";
                        string inp;
                        cin >> inp;
                        if (inp == "goback") {
                            break;
                        } else {
                            isbn = inp;
                        }
                        fac.returnBook(isbn);
                    }
                }
                    break;
                case 5:
                    fac.viewHistory();
                    break;
                default:
                    cout << RED << "Invalid choice. Please try again.\n" << RESET;
            }
        }
        accounts[x] = fac.account;
        fac.saveData();
    }
    return exit;
}

#endif