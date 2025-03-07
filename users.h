#ifndef USERS_H
#define USERS_H
#include "utils.h"
#include "transaction.h"
#include "database.h"
/*
can remove available_num, borrowed_num, etc.

*/

class Client;

void updateStudentDues(Client* clientPtr, string isbn, int borrow_date, int return_date);

class User{
    public:
    Account account;
    unordered_map <string, Book> available_books;

    User() {
        fetchAvailable();
    }

    void viewAvailable() {
        for (auto& pair : available_books) {
            pair.second.printBook();
        }
    }

    protected :
    void saveAvailable() {
        string filepath = FILEPATH / "data" / "available_books.json";
        writeBooksTo(filepath, available_books);
    }

    void fetchAvailable() {
        fs::path filePath = FILEPATH / "data" / "available_books.json";
        if (!fs::exists(filePath)) {
            ofstream file(filePath);
            file << "[]"; // Initialize empty JSON array
            file.close();
        }
        readBooksFrom(filePath.string(), available_books);
    }
};

class Client: public User{
    private:
    bool fetched_history;

    public:
    int MAX_BORROWABLE_BOOKS;
    int MAX_BORROWABLE_TIME;
    unordered_map <string, Book> borrowed_books;
    vector <Book> history_books;

    Client() {
        fetched_history = false;
    }

    void viewBorrowed() {
        // use this.id and this.user_type
        for (auto& pair : borrowed_books) {
            pair.second.printBook();
        }
    }

    void viewHistory(){
        if(!fetched_history) {
            fetchHistory();
        }
        for (auto& book : history_books) {
            book.printBook();
        }
    }

    void borrowBook() { // can display available books first. isbn --> title
        while(true) {
            string isbn;
            cout << "\n(Enter 'goback' to go back)\nEnter book ISBN (ensure that book is available): ";
            string inp;
            cin >> inp;
            if (inp == "goback") {
                return;
            } else {
                isbn = inp;
            }
            auto iter = available_books.find(isbn);
            if (iter != available_books.end()) {
                Book newbook = iter->second;
                newbook.printBook();

                cout << BLUE << "Are you sure you want to borrow this book?\n";
                cout << "  1: Yes\n";
                cout << "  2: No\n" << RESET;
                int choice;
                while (true) {
                    cin >> choice;
                    if (!cin.fail()) break;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid choice. Enter a valid choice ";
                }

                if (choice == 1) {
                    available_books.erase(iter);

                    newbook.status = "borrowed";
                    newbook.borrowing_user = account.id;
                    newbook.borrow_date = getDate();
                    newbook.due_date = newbook.borrow_date + MAX_BORROWABLE_TIME; 

                    borrowed_books[isbn] = newbook;
                    cout << GREEN << "Book successfully borrowed!\n" << RESET;
                    return;
                } 
            } else { 
                cout << "Book not found!" << endl;
                return;
            }
        }
    }

    void returnBook(string& isbn) { // can display borrowed books first. isbn --> title
        while(true) {
            auto iter = borrowed_books.find(isbn);
            if (iter != borrowed_books.end()) {
                Book newbook = iter->second;
                newbook.printBook();

                cout << BLUE << "Are you sure you want to return this book?\n";
                cout << "  1: Yes\n";
                cout << "  2: No\n" << RESET;
                int choice;
                while (true) {
                    cin >> choice;
                    if (!cin.fail()) break;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid choice. Enter a valid choice ";
                }

                if (choice == 1) {
                    if (account.user_type == "student" && getDate() > newbook.due_date) {
                        updateStudentDues(this, newbook.isbn, newbook.due_date, getDate());
                    }
                    borrowed_books.erase(iter);
                    history_books.push_back(newbook);
                    newbook.status = "available";
                    newbook.borrowing_user = -1;
                    newbook.borrow_date = -1;
                    newbook.due_date = -1; 

                    available_books[isbn] = newbook;

                    cout << GREEN << "Book successfully returned!\n" << RESET;
                    return;
                } 
            } else { 
                cout << "Book not borrowed earlier!" << endl;
                return;
            }
        }
        
    }

    protected:
    void fetchHistory() {
        if (!fetched_history){
            string parentDir = FILEPATH / "data" / "history";
            fs::create_directories(parentDir);
            for (const auto& entry : fs::directory_iterator(parentDir)) {
                if (entry.is_regular_file() && entry.path().extension() == ".json") {
                    readOnlyBooksFrom(entry.path().string(), history_books);
                }
            }
        }
        fetched_history = true;
    }

    void fetchBorrowed() { 
        string parentDir = FILEPATH / "data" / "current";
        fs::create_directories(parentDir);
        for (const auto& entry : fs::directory_iterator(parentDir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".json") {
                readBooksFrom(entry.path().string(), borrowed_books);
            }
        }
    }

    void saveBorrowed(int id) {
        string filepath = FILEPATH / "data" / "current" / (to_string(id) + ".json");
        writeBooksTo(filepath, borrowed_books);
    }

    void saveHistory(int id) {
        string filepath = FILEPATH / "data" / "history" / (to_string(id) + ".json");
        writeBooksTo(filepath, borrowed_books);
    }
};


class Student: public Client{
    public:
    int FINE_PER_BOOK_PER_DAY; 
    int total_fine;
    vector <Dues> dues;

    Student() {
        Account::account_num++;
        MAX_BORROWABLE_BOOKS = 3;
        MAX_BORROWABLE_TIME = 15;
        FINE_PER_BOOK_PER_DAY = 10;
        total_fine = 0;
        account.amount_paid = 0;
        account.id = Account::account_num;
        account.user_type = "student";
    }

    Student(int &id, int payment) {
        account.id = id;
        account.user_type = "student";
        account.amount_paid = payment;
        MAX_BORROWABLE_BOOKS = 3;
        MAX_BORROWABLE_TIME = 15;
        FINE_PER_BOOK_PER_DAY = 10;
        fetchBorrowed();
        fetchDues();
    }
    // make a new file for each user which contains the dues for each unpaid book.

    void viewPayDues() { // consider we can only pay those books which we have returned.
        int history_fine = 0;
        // for (const auto& due : this->dues) {
        //     if (due.return_date > due.due_date) {
        //         int fine = (due.return_date - due.due_date) * FINE_PER_BOOK_PER_DAY;
        //         history_fine += fine;
        //     }    
        // }
        total_fine = 0;
        cout << "You can only pay the fines for the books you have already returned." << endl;
        for (auto &due : dues) {
            if (getDate() > due.due_date){
            cout << "isbn: " << due.isbn << ", due date: " << due.due_date << ", Return date: " << due.return_date << endl;
            cout << RED << "Fine: " << (due.return_date - due.due_date)*FINE_PER_BOOK_PER_DAY << endl << RESET;
            total_fine += (due.return_date - due.due_date)*FINE_PER_BOOK_PER_DAY;
            }
        }
        if (total_fine + history_fine == 0) {
            cout << GREEN << "You don't have any remaining dues!\n" << RESET << endl;
            return;
        } 
        cout << YELLOW << "Therefore the total fine is " << total_fine + history_fine<< endl;
        if (account.amount_paid != 0) cout << "You have already paid " << account.amount_paid << endl;
        cout << "Fees remaining: " << total_fine +history_fine - account.amount_paid << RESET << endl; 
        cout << "\n\n Would you like to pay the dues?\n  1: Yes\n  2: No\n";
        int choice;
        while (true) {
            cin >> choice;
            if (!cin.fail()) break;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice. Enter a valid choice ";
        }
        if (choice == 1) {
            cout << BLUE << "Enter the payment to be done: " << RESET;
            int new_payment;
            string inp;
            cin >> inp;
            if (inp == "goback") {
                return;
            } else {
                new_payment = stoi(inp);
            }

            if (new_payment <= 0) {
                cout << RED << "Invalid payment amount. Please enter a positive value." << RESET << endl;
            } else if (new_payment > total_fine+history_fine) {
                cout << RED << "Payment exceeds the outstanding fine. Please enter a valid amount." << RESET << endl;
            } else {
                account.amount_paid += new_payment;
                cout << GREEN << "Payment of ₹" << new_payment << " successful. Remaining due: ₹" 
                    << total_fine + history_fine- account.amount_paid << RESET << endl;
                if (total_fine + history_fine - account.amount_paid == 0){
                    total_fine = 0;
                    account.amount_paid = 0;
                    dues.clear();
                }
            }
        } else if (choice == 2) {
            return;
        } else {
            cout << BLUE << "Invalid choice. Redirecting to homepage.\n" << RESET;
            return;
        }
    }
    void saveData() {
        // payment not updated in accounts part.
        saveBorrowed(this->account.id);
        saveAvailable();
        saveDues();
        saveHistory(this->account.id);
    }
    private :
    void saveDues() {
        writeDues(this->account.id, dues);
    }

    void fetchDues() {
        readDues(this->account.id, dues);
        total_fine = 0;
        for(auto &due:dues) {
            total_fine += (due.return_date - due.due_date)* FINE_PER_BOOK_PER_DAY;
        }
    }
}; 

void updateStudentDues(Client* clientPtr, string isbn, int due_date, int return_date) {
    Student* studentPtr = static_cast<Student*>(clientPtr);
    studentPtr->dues.push_back(Dues(isbn, due_date, return_date));
    cout << RED << "Late return detected! Dues updated for ISBN: " << isbn << RESET << endl;
    return;
}


class Faculty: public Client{
    public:
    int MAX_DUE_PERIOD_ALLOWABLE;
    int max_due_period;
    Faculty() {
        Account::account_num++;
        MAX_BORROWABLE_BOOKS = 5;
        MAX_BORROWABLE_TIME = 30;
        MAX_DUE_PERIOD_ALLOWABLE = 60;
        max_due_period = 0;
        account.id = Account::account_num;
        account.user_type = "faculty";
    }

    Faculty(int& id) {
        account.id = id;
        account.user_type = "faculty";
        MAX_BORROWABLE_BOOKS = 5;
        MAX_BORROWABLE_TIME = 30;
        MAX_DUE_PERIOD_ALLOWABLE = 60;
        fetchBorrowed();
        max_due_period = 0; // Initialize to 0

        // Iterate over borrowed_books to find the max due period
        for (const auto& pair : borrowed_books) {
            const Book& book = pair.second;
            int due_period = book.due_date - book.borrow_date;
            max_due_period = max(max_due_period, due_period);
        }
    }
    void saveData() {
        saveBorrowed(this->account.id);
        saveAvailable();
        saveHistory(this->account.id);
    }
};

class Librarian: public User{
    public:
    unordered_map <string, Book> reserved_books;
    int total_books;
    int borrowed_books_num;
    int reserved_books_num;
    vector <LibraryEntry> library;

    Librarian() {
        Account::account_num++;
        account.id = Account::account_num;
        account.user_type = "librarian";
        fetched_reserved = false;
        borrowed_books_num = 0;
        total_books = 0;
        reserved_books_num = 0;
        for(auto& i : library) {
            if(i.status == "borrowed") {
                borrowed_books_num++;
            } else if (i.status == "reserved") {
                reserved_books_num++;
            }
            total_books++;
        }
    }

    Librarian(int& id) {
        account.id = id;
        account.user_type = "librarian";
        fetched_reserved = false;
        borrowed_books_num = 0;
        total_books = 0;
        reserved_books_num = 0;
        for(auto& i : library) {
            if(i.status == "borrowed") {
                borrowed_books_num++;
            } else if (i.status == "reserved") {
                reserved_books_num++;
            }
            total_books++;
        }
    }

    void viewBorrowedBooks() {
        string parentDir = FILEPATH / "data" / "current";
        fs::create_directories(parentDir);
        for (const auto& entry : fs::directory_iterator(parentDir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".json") {
                unordered_map <string, Book>books;
                readBooksFrom(entry.path().string(), books);
                for(auto& i : books) {
                    i.second.printBook();
                }
            }
        }
    }

    void saveData() {
        // save reserved books
        saveReserved();
        saveAvailable();
        // save available books
    }

    void viewReservedBooks() {
        for (auto &i : reserved_books) {
            i.second.printBook();
        }
    }

    void addBook(Book& book) {
        available_books.emplace(book.isbn, book);
    }

    void removeBook(string& isbn) {
        auto iter = available_books.find(isbn);
        if (iter == available_books.end()) {
            cout << "Book not found!" << endl;
            return;
        }

        while (true) {
            cout << "Are you sure you want to delete this book? (1: Yes, 2: No): ";
            int choice;
            while (true) {
                cin >> choice;
                if (!cin.fail()) break;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid choice. Enter a valid choice ";
            }
            if (choice == 1) {
                available_books.erase(iter);
                cout << "Book successfully removed!" << endl;
                this->total_books--;
                break;
            } else if (choice == 2) {
                cout << "Operation canceled." << endl;
                break;
            } else {
                cout << "Invalid choice. Please enter 1 or 2." << endl;
            }
        }
    }


    void editBook(string& isbn) {
        auto iter = available_books.find(isbn);
        if (iter == available_books.end()) {
            cout << RED << "Book not found!\n" << RESET;
            return;
        }
        while (true) {
            Book updatedBook = getBookDetailsFromUser();
            updatedBook.printBook();
            cout << BLUE << "Confirm book update?\n";
            cout << "  1: Yes\n";
            cout << "  2: No, enter details again\n";
            cout << "  3: Return to homepage\n" << RESET;
            int choice;
            while (true) {
                cin >> choice;
                if (!cin.fail()) break;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid choice. Enter a valid choice ";
            }
            if (choice == 1) {
                available_books.erase(iter);
                available_books[updatedBook.isbn] = updatedBook;
                cout << GREEN << "Book updated successfully!\n" << RESET;
                break;
            } else if (choice == 2) {
                continue;  
            } else if (choice == 3) {
                return;  
            } else {
                cout << RED << "Invalid input. Please try again.\n" << RESET;
            }
        }
    }


    void reserveBook(string& isbn) {
        if (!fetched_reserved) {
            fetchReserved();
        }
        auto iter = available_books.find(isbn);
        if (iter == available_books.end()) {
            cout << RED << "Book not found\n" << RESET;
            return;
        }
        while (true) {
            cout << BLUE << "Confirm book reservation?\n";
            cout << "  1: Yes\n";
            cout << "  2: No, go back\n";
            cout << RESET;

            int choice;
            while (true) {
                cin >> choice;
                if (!cin.fail()) break;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid choice. Enter a valid choice ";
            }

            if (choice == 1) {
                Book reservedBook = iter->second;
                reservedBook.status = "reserved";

                // Move book from available to reserved
                available_books.erase(iter);
                reserved_books[isbn] = reservedBook;
                this->reserved_books_num++;
                cout << GREEN << "Book successfully reserved!\n" << RESET;
                break;
            } else if (choice == 2) {
                return;
            } else {
                cout << RED << "Invalid input. Please try again.\n" << RESET;
            }
        }
    }
        
    void unreserveBook(string& isbn) {
        if (!fetched_reserved) {
            fetchReserved();
        }

        auto iter = reserved_books.find(isbn);
        if (iter == reserved_books.end()) {
            cout << RED << "Book not found in reserved books\n" << RESET;
            return;
        }

        while (true) {
            cout << BLUE << "Confirm book unreservation?\n";
            cout << "  1: Yes\n";
            cout << "  2: No, go back\n";
            cout << RESET;

            int choice;
            while (true) {
                cin >> choice;
                if (!cin.fail()) break;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid choice. Enter a valid choice: ";
            }

            if (choice == 1) {
                Book unreservedBook = iter->second;
                unreservedBook.status = "available";

                // Move book from reserved to available
                reserved_books.erase(iter);
                available_books[isbn] = unreservedBook;
                this->reserved_books_num--;

                cout << GREEN << "Book successfully unreserved!\n" << RESET;
                break;
            } else if (choice == 2) {
                return;
            } else {
                cout << RED << "Invalid input. Please try again.\n" << RESET;
            }
        }
    }


    void addUser(vector<Account> &accounts) {
        while (true) {
            cout << BLUE << "Select user type to add:\n";
            cout << "  1: Student\n";
            cout << "  2: Faculty\n";
            cout << "  3: Go back\n" << RESET;
            int choice;
            while (true) {
                cin >> choice;
                if (!cin.fail()) break;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid choice. Enter a valid choice ";
            }
            string userType;
            if (choice == 1) {
                userType = "student";
            } else if (choice == 2) {
                userType = "faculty";
            } else if (choice == 3) {
                return;  // Go back
            } else {
                cout << RED << "Invalid choice. Please try again.\n" << RESET;
                continue;
            }
            cout << BLUE << "Are you sure you want to create a new account for a " << userType << "?\n";
            cout << "  1: Yes\n";
            cout << "  2: No, go back\n" << RESET;
            int confirmChoice;
            while (true) {
                cin >> confirmChoice;
                if (!cin.fail()) break;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid choice. Enter a valid choice ";
            }
            if (confirmChoice == 2) {
                continue; 
            } else if (confirmChoice != 1) {
                cout << RED << "Invalid input. Please try again.\n" << RESET;
                continue;
            }
            int userId;
            if (userType == "student") {
                Student newStudent;
                accounts.push_back(newStudent.account);
                userId = newStudent.account.id;
            } else {
                Faculty newFaculty;
                accounts.push_back(newFaculty.account);
                userId = newFaculty.account.id;
            }

            cout << GREEN << userType << " account created successfully with ID: " << userId << "\n" << RESET;
            vector<string> folders = {"data/current/", "data/history/", "data/dues/"};
            for (const auto &folder : folders) {
                fs::path filePath = FILEPATH / folder / (to_string(userId) + ".json");
                if (!fs::exists(filePath)) {
                    ofstream file(filePath);
                    file << "[]";
                    file.close();
                }
            }
            break; 
        }
    }


    void removeUser(vector <Account> &accounts, int& id) {
        for (auto it = accounts.begin(); it != accounts.end(); ++it) {
            if (it->id == id) { 
                accounts.erase(it);
                vector<fs::path> userFiles = {
                    FILEPATH / "data" / "current" / (to_string(id) + ".json"),
                    FILEPATH / "data" / "history" / (to_string(id) + ".json"),
                    FILEPATH / "data" / "dues" / (to_string(id) + ".json")
                };
                for (const auto &file : userFiles) {
                    if (fs::exists(file)) {
                        fs::remove(file);
                    }
                }
                cout << "User with ID " << id << " removed successfully!" << endl;
                return;
            }
        }
        cout << "User does not exist!" << endl;
    }

    void manageUser(vector<Account> &accounts, int id) {
        Account* userAccount = nullptr;
        for (auto &acc : accounts) {
            if (acc.id == id && (acc.user_type == "student" || acc.user_type == "faculty")) {
                userAccount = &acc;
                break;
            }
        }
        if (!userAccount) {
            cout << "User does not exist!" << endl;
            return;
        }
        if (userAccount->user_type == "student") {
            Student student(id, userAccount->amount_paid); 
            while (true) {
                cout << "\nManage Student Menu:\n";
                cout << "1. Borrow Book\n";
                cout << "2. Return Book\n";
                cout << "3. View/Pay Dues\n";
                cout << "4. Exit\n";
                cout << "Enter your choice: ";
                int choice;
                while (true) {
                    cin >> choice;
                    if (!cin.fail()) break;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid choice. Enter a valid choice ";
                }

                if(choice == 4) break;

                switch (choice) {
                    case 1:
                        if (student.borrowed_books.size() >= student.MAX_BORROWABLE_BOOKS) {
                            cout << RED << "You have reached the maximum borrowing limit of " 
                                << student.MAX_BORROWABLE_BOOKS << " books. Please return at least one book to borrow more." 
                                << RESET << endl;
                        } else if (student.total_fine - student.account.amount_paid > 0) {
                            cout << RED << "You have an outstanding fine of ₹" << student.total_fine - student.account.amount_paid
                                << ". Please clear your dues before borrowing new books." 
                                << RESET << endl;
                        } else {
                            student.borrowBook();
                        }
                        break;
                    case 2:
                        if (student.borrowed_books.size() == 0) {
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
                            student.returnBook(isbn);
                        }
                        break;
                    case 3:
                        student.viewPayDues();
                        break;
                    default:
                        cout << "Invalid choice! Try again.\n";
                }
            }
            student.saveData();
        } else if (userAccount->user_type == "faculty") {
            Faculty faculty(id); 
            while (true) {
                cout << "\nManage Faculty Menu:\n";
                cout << "1. Borrow Book\n";
                cout << "2. Return Book\n";
                cout << "3. Exit\n";
                cout << "Enter your choice: ";
                int choice;
                while (true) {
                    cin >> choice;
                    if (!cin.fail()) break;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid choice. Enter a valid choice ";
                }
                if (choice == 3) break;
                switch (choice) {
                    case 1:
                        if (faculty.borrowed_books.size() >= faculty.MAX_BORROWABLE_BOOKS) {
                            cout << RED << "You have reached the maximum borrowing limit of " 
                                << faculty.MAX_BORROWABLE_BOOKS << " books. Please return at least one book to borrow more." 
                                << RESET << endl;
                        } else {
                            faculty.borrowBook();
                        }
                        break;
                    case 2:
                        if (faculty.borrowed_books.size() == 0) {
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
                            faculty.returnBook(isbn);
                        }
                        break;
                    default:
                        cout << "Invalid choice! Try again.\n";
                }
            }
            faculty.saveData();
        }
    }

    // borrowbook, returnbook, paydues to be implemented here.
    private:
    bool fetched_reserved;
    void fetchReserved() {
        if (!fetched_reserved) {
            string filepath = FILEPATH / "data" / "reserved.json";
            readBooksFrom(filepath, reserved_books);
            fetched_reserved = true;
        }
    }


    void saveReserved() {
        string filepath = FILEPATH / "data" / "reserved.json";
        writeBooksTo(filepath, reserved_books);
    }
};

#endif

