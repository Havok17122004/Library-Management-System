#ifndef LOGIN_LIB_H
#define LOGIN_LIB_H
#include <bits/stdc++.h>
using namespace std;

bool loginLib(int &id, vector <Account> &accounts) {
    bool found = false;
    for(int i = 0; i < accounts.size(); i++) {
        if (accounts[i].id == id && accounts[i].user_type == "librarian") {
            found = true;
        } 
    }

    if (!found) {  
        cout << RED << "Log in unsuccessful\nPlease try again\n" << RESET;
        return false;
    }
    bool exit;
    Librarian librarian(id); 
    while (true) {
        int choice;
        cout << YELLOW << "Current Date = " << getDate() << endl << RESET;
        cout << BLUE;
        cout << "Logged in as: Librarian - " << id << endl;
        cout << "Total books in library: " << librarian.library.size() << endl;
        cout << "Available books: " << librarian.available_books.size() << endl;
        cout << "Borrowed books: " << librarian.borrowed_books_num << endl;
        cout << "Reserved books: " << librarian.reserved_books_num << endl;

        cout << "\nOptions:\n";
        cout << "  1 : View available books\n";
        cout << "  2 : View borrowed books\n";
        cout << "  3 : View reserved books\n";
        cout << "  4 : Add Book\n";
        cout << "  5 : Remove Book\n";
        cout << "  6 : Edit Book\n";
        cout << "  7 : Reserve Book\n";
        cout << "  8 : Un-Reserve Book\n";
        cout << "  9 : Manage user\n";
        cout << " 10 : Add user\n";
        cout << " 11 : Delete user\n";
        cout << " 12 : Logout\n";
        cout << " 13 : Logout and exit\n\n" << RESET;

        cout << "Enter your choice: ";
        
        while (true) {
            cin >> choice;
            if (!cin.fail()) break;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << RED << "Invalid choice. Enter a valid choice " << RESET;
        }
        if (choice == 12) {
            exit = false;
            break;
        } else if (choice == 13) {
            exit = true;
            break;
        }
        switch (choice) {
            case 1:
                librarian.viewAvailable();
                break;
            case 2:
                librarian.viewBorrowedBooks();
                break;
            case 3:
                librarian.viewReservedBooks();
                break;
            case 4:
            {
                while (true) {
                    Book newBook = getBookDetailsFromUser(true);
                    newBook.printBook();
                    auto iter = librarian.library.find(newBook.isbn);

                    if (iter != librarian.library.end()) {
                        cout << "Another book with the same ISBN already exists. Please enter a different book.\n";
                        continue;
                    }
                    cout << BLUE << "Confirm book addition?\n";
                    cout << "  1: Yes\n";
                    cout << "  2: No, enter details again\n";
                    cout << "  3: Return to homepage\n" << RESET;
                    
                    int choice;
                    while (true) {
                        cin >> choice;
                        if (!cin.fail()) break;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << RED << "Invalid choice. Enter a valid choice " << RESET;
                    }

                    if (choice == 1) {
                        librarian.addBook(newBook);
                        cout << GREEN << "Book successfully added!\n" << RESET;
                        break;
                    } else if (choice == 2) {
                        continue;  // Ask for details again
                    } else if (choice == 3) {
                        return false;  // Return to homepage
                    } else {
                        cout << RED << "Invalid input. Please try again.\n" << RESET;
                    }
                }
            }
            break;
            case 5:
                {
                    string isbn;
                    cout << "\nCan only remove the book that is currently available in the library\n(Enter 'goback' to go back)\nEnter ISBN of the book to remove: ";
                    string inp;
                    cin >> inp;
                    if (inp == "goback") {
                        break;
                    } else {
                        isbn = inp;
                    }
                    librarian.removeBook(isbn);
                }
                break;
            case 6:
                {
                    string isbn;
                    cout << "\n(Enter 'goback' to go back)\nEnter ISBN of the book to edit: ";
                    string inp;
                    cin >> inp;
                    if (inp == "goback") {
                        break;
                    } else {
                        isbn = inp;
                    }
                    librarian.editBook(isbn);
                }
                break;
            case 7:
                {
                    string isbn;
                    cout << "\n(Enter 'goback' to go back)\nEnter ISBN of the book to reserve: ";
                    string inp;
                    cin >> inp;
                    if (inp == "goback") {
                        break;
                    } else {
                        isbn = inp;
                    }
                    librarian.reserveBook(isbn);
                }
                break;
            case 8:
                {
                    string isbn;
                    cout << "\n(Enter 'goback' to go back)\nEnter ISBN of the book to unreserve: ";
                    string inp;
                    cin >> inp;
                    if (inp == "goback") {
                        break;
                    } else {
                        isbn = inp;
                    }
                    librarian.unreserveBook(isbn);
                }
                break;

            case 9:
                {
                    int user_id;
                    cout << "\n(Enter 'goback' to go back)\nEnter user ID to manage: ";
                    string inp;
                    cin >> inp;
                    if (inp == "goback") {
                        break;
                    } else {
                        user_id = stoi(inp);
                    }
                    librarian.manageUser(accounts, user_id);
                }
                break;
            case 10:
                librarian.addUser(accounts);
                break;
            case 11: // cannot delete himself.
                {
                    int user_id;
                    cout << "\n(Enter 'goback' to go back)\nEnter user ID to delete: ";
                    string inp;
                    cin >> inp;
                    if (inp == "goback") {
                        break;
                    } else {
                        user_id = stoi(inp);
                    }
                    if (user_id == librarian.account.id) {
                        cout << "You yourself cannot delete your account. You need to do this using some other librarian's account.\n";
                        break;
                    }
                    librarian.removeUser(accounts, user_id);
                }
                break;
            // case 11:
            //     return false;
            // case 12:
            //     return true;
            default:
                cout << RED << "Invalid choice. Please try again.\n" << RESET;
                break;
        }
    }
    librarian.saveData();
    return exit;
}

#endif