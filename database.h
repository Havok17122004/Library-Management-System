#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <filesystem>
#include "transaction.h"
#include "utils.h"
#include "users.h"
#include "json.hpp"  // Include the JSON library
#define GREEN "\033[32m"
namespace fs = std::filesystem;
using json = nlohmann::json;
using namespace std;
fs::path FILEPATH;
/*
(accounts.json)
123456  <-- First line (integer: date)
[
    { "id": 1, "user_type": "student", "payment": 500 },
    { "id": 2, "user_type": "faculty", "payment": 0 }
]
*/


class Dues {
    public:
    string isbn; int due_date; int return_date;
    Dues(string isbn, int due_date, int return_date) {
        this->isbn = isbn; this->due_date = due_date; this->return_date = return_date;
    }
};


class LibraryEntry {
    public:
    string status;
    int user_id;

    LibraryEntry() {
        // isbn = "";
        status = "available";
        user_id = -1;
    }

    LibraryEntry(string status, int user_id) {
        // this->isbn = isbn;
        this->status = status;
        this->user_id = user_id;
    }
};

void writeAccounts(int date, int accounts_num, const vector<Account>& accounts) {
    fs::path accountsPath = FILEPATH / "data" / "accounts.json";
    ofstream file(accountsPath.string());
    file << date << endl;  // Write the first line (date)
    file << accounts_num << endl;
    json j = json::array();
    for (const auto& acc : accounts) {
        j.push_back({{"id", acc.id}, {"user_type", acc.user_type}, {"payment", acc.amount_paid}});
    }
    file << j.dump(4);
    file.close();
}

void fetchAccounts(int& date, int& accounts_num, vector<Account>& accounts) {
    accounts.clear();
    fs::path accountsPath = FILEPATH / "data" / "accounts.json";
    fs::path parentDir = accountsPath.parent_path();
    if (!fs::exists(parentDir)) {
        fs::create_directories(parentDir);  // Create "data/" if missing
    }
    if (!fs::exists(accountsPath)) {
        date = 0;
        // accounts: 5 Students, 3 Faculty, 1 Librarian
        vector<Account> accounts_init = {
            Account(1, "student", 0),
            Account(2, "student", 0),
            Account(3, "student", 0),
            Account(4, "student", 0),
            Account(5, "student", 0),
            Account(6, "faculty", 0),
            Account(7, "faculty", 0),
            Account(8, "faculty", 0),
            Account(9, "librarian", 0)
        };
        accounts_num = accounts_init.size();
        writeAccounts(0, accounts_init.size(), accounts_init);
    }
    ifstream file(accountsPath.string());
    if (!file.is_open()) {
        cerr << "Error opening " << accountsPath.string() << endl;
        return;
    }
    string dateStr, accNumStr;
    if (!getline(file, dateStr) || !getline(file, accNumStr)) {
        cerr << "Error: accounts.json is not formatted correctly." << endl;
        return;
    }
        date = stoi(dateStr);
        accounts_num = stoi(accNumStr);
    json j;
    file >> j;  // Read the JSON array
    for (const auto& acc : j) {
        accounts.push_back(Account(acc["id"], acc["user_type"], acc["payment"]));
    }
    file.close();
}

void readAllBooks(unordered_map<string, LibraryEntry>& entries) {
    fs::path path = FILEPATH / "data" / "allbooks.json";
    if (!fs::exists(path.parent_path())) {
        fs::create_directories(path.parent_path());
    }
    if (!fs::exists(path)) {
        ofstream newFile(path.string());
        newFile << "[]";
        newFile.close();
    }
    ifstream file(path.string());
    json j;
    file >> j;
    file.close();
    for (const auto& book : j) {
        entries.emplace(book["isbn"].get<string>(), LibraryEntry(book["status"].get<string>(), book["user_id"].get<int>()));
    }
}

void writeAllBooks(unordered_map<string, LibraryEntry>& entries) {
    fs::path path = FILEPATH / "data" / "allbooks.json";
    if (!fs::exists(path.parent_path())) {
        fs::create_directories(path.parent_path());
    }
    ofstream file(path.string());
    json j = json::array();
    for (const auto& entry : entries) {
        j.push_back({{"isbn", entry.first}, {"status", entry.second.status}, {"user_id", entry.second.user_id}});
    }
    file << j.dump(4);
    file.close();
}

void writeBooksTo(const string& filename, unordered_map<string, Book>& books) {
    fs::path filePath = filename;
    fs::path parentDir = filePath.parent_path();
    if (!fs::exists(parentDir)) {
        fs::create_directories(parentDir);
    }
    if (!fs::exists(filePath)) {
        ofstream newFile(filePath.string());
        if (newFile.is_open()) {
            newFile << "[]";  // Initialize with an empty JSON array
            newFile.close();
        }
    }
    ofstream file(filename);
    json j = json::array();
    for (const auto& book : books) {
        j.push_back({
            {"title", book.second.title},
            {"author", book.second.author},
            {"publisher", book.second.publisher},
            {"year", book.second.year},
            {"isbn", book.second.isbn},
            {"status", book.second.status},
            {"borrowing_user", book.second.borrowing_user},
            {"borrow_date", book.second.borrow_date},
            {"due_date", book.second.due_date}
        });
    }
    file << j.dump(4);
    file.close();
}

bool hasEnding(const std::string& fullString, const std::string& ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

void readBooksFrom(const string& filename, unordered_map<string, Book>& books) {
    fs::path filePath = filename;
    fs::path parentDir = filePath.parent_path();
    if (!fs::exists(parentDir)) {
        fs::create_directories(parentDir);
    }
    if (!fs::exists(filePath)) {
        if (hasEnding(filename, "available_books.json")) {
            // Directly assign to the passed reference to avoid shadowing
            books = {
                {"9780131101630", Book("The C Programming Language", "Kernighan & Ritchie", "Prentice Hall", 1988, "9780131101630", "available", -1, -1, -1)},
                {"9780262033848", Book("Introduction to Algorithms", "Cormen et al.", "MIT Press", 2009, "9780262033848", "available", -1, -1, -1)},
                {"9780131103627", Book("Computer Networks", "Andrew Tanenbaum", "Prentice Hall", 2010, "9780131103627", "available", -1, -1, -1)},
                {"9780201633610", Book("Design Patterns", "Gamma et al.", "Addison-Wesley", 1994, "9780201633610", "available", -1, -1, -1)},
                {"9780132350884", Book("Clean Code", "Robert C. Martin", "Prentice Hall", 2008, "9780132350884", "available", -1, -1, -1)},
                {"9780321356680", Book("Effective C++", "Scott Meyers", "Addison-Wesley", 2005, "9780321356680", "available", -1, -1, -1)},
                {"9780596007126", Book("Head First Design Patterns", "Eric Freeman", "O'Reilly", 2004, "9780596007126", "available", -1, -1, -1)},
                {"9780201616224", Book("The Pragmatic Programmer", "Andy Hunt", "Addison-Wesley", 1999, "9780201616224", "available", -1, -1, -1)},
                {"9780133594140", Book("Artificial Intelligence: A Modern Approach", "Stuart Russell", "Pearson", 2015, "9780133594140", "available", -1, -1, -1)},
                {"9781492052203", Book("System Design Interview", "Alex Xu", "ByteByteGo", 2020, "9781492052203", "available", -1, -1, -1)}
            };
            writeBooksTo(filename, books);
            unordered_map<string, LibraryEntry> libraryelement;
            for (const auto &book : books) {
                libraryelement.emplace(book.first, LibraryEntry());
            }
            writeAllBooks(libraryelement);
        } else {
            ofstream newFile(filePath.string());
            newFile << "[]";  // Initialize as an empty JSON array
            newFile.close();
        }
    }
    ifstream file(filePath.string());
    json j;
    file >> j;
    for (const auto& book : j) {
        Book newbook(book["title"], book["author"], book["publisher"], book["year"], book["isbn"],
                     book["status"], book["borrowing_user"], book["borrow_date"], book["due_date"]);
        books.emplace(newbook.isbn, newbook);
    }
    file.close();
}

/*
[
    { "isbn": "9781234567897", "start": 1700000000, "end": 1702592000 },
    { "isbn": "9789876543210", "start": 1700001000, "end": 1702593000 }
]
*/

void readDues(int id, vector<Dues>& dues) {
    string x = to_string(id) + ".json";
    fs::path path = FILEPATH / "data" / "dues" / x;
    fs::path parentDir = path.parent_path();
    if (!fs::exists(parentDir)) {
        fs::create_directories(parentDir);
    }
    if (!fs::exists(path)) {
        ofstream newFile(path.string());
        newFile << "[]";  // Initialize with an empty JSON array
        newFile.close();
    }
    ifstream file(path.string());
    json j;
    file >> j;
    for (const auto& due : j) {
        dues.push_back(Dues(due["isbn"], due["start"], due["end"]));
    }
    file.close();
}

void writeDues(int id, const vector<Dues>& dues) {
    string x = to_string(id) + ".json";
    fs::path path = FILEPATH / "data" / "dues" / x;
    fs::path parentDir = path.parent_path();
    if (!fs::exists(parentDir)) {
        fs::create_directories(parentDir);
    }
    ofstream file(path.string());
    if (!file.is_open()) {
        cerr << "Error opening " << path.string() << " for writing." << endl;
        return;
    }
    json j = json::array();
    for (const auto& due : dues) {
        j.push_back({{"isbn", due.isbn}, {"start", due.due_date}, {"end", due.return_date}});
    }
    file << j.dump(4);
    file.close();
}

#endif