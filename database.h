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
#include "/usr/include/nlohmann/json.hpp"  // Include the JSON library
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
    string isbn;
    string status;
    int user_id;
};

void fetchAccounts( int& date, int &accounts_num, vector<Account>& accounts) {
    auto accountsPath = FILEPATH / "data" / "accounts.json";
    fs::path parentDir = accountsPath.parent_path();
    if (!fs::exists(parentDir)) {
        fs::create_directories(parentDir);  // Create "data/" if missing
    }
    if (!fs::exists(accountsPath)) {
        ofstream newFile(accountsPath);
        newFile << "0\n0\n[]";  // Initialize with empty values
        newFile.close();
    }
    ifstream file(accountsPath.string());
    if (!file.is_open()) {
        cerr << "Error opening " << accountsPath.string() << endl;
        return;
    }
    string dateStr, accNumStr, jsonStr;
    if (!getline(file, dateStr) || !getline(file, accNumStr)) {
        cerr << "Error: accounts.json is not formatted correctly." << endl;
        return;
    }

    try {
        date = stoi(dateStr);
        accounts_num = stoi(accNumStr);
    } catch (...) {
        cerr << "Error: Invalid date or accounts_num in accounts.json" << endl;
        return;
    }

    json j;
    file >> j;  // Read the JSON array
    for (const auto& acc : j) {
        accounts.push_back(Account(acc["id"], acc["user_type"], acc["payment"]));
    }
    file.close();
}

void writeAccounts(int date, int accounts_num, const vector<Account>& accounts) {
    auto accountsPath = FILEPATH / "data" / "accounts.json";
    ofstream file(accountsPath.string());
    if (!file.is_open()) {
        cerr << "Error opening " << accountsPath.string() << " for writing." << endl;
        return;
    }
    file << date << endl;  // Write the first line (date)
    file << accounts_num << endl;
    json j = json::array();
    for (const auto& acc : accounts) {
        j.push_back({{"id", acc.id}, {"user_type", acc.user_type}, {"payment", acc.amount_paid}});
    }
    
    file << j.dump(4); 
    file.close();
}


/*
[
    { "isbn": "9781234567897", "status": "available", "user_id_associated": -1 },
    { "isbn": "9789876543210", "status": "borrowed", "user_id_associated": 2 }
]
*/

void readAllBooks(vector<LibraryEntry>& entries) { //make changes
    auto path = FILEPATH / "data" / "allbooks.json";
    ifstream file(path.string());
    if (!file.is_open()) {
        cerr << "Error opening " << path.string() << endl;
        return;
    }
    json j;
    file >> j;
    for (const auto& book : j) {
        entries.push_back({book["isbn"], book["status"], book["user_id"]});
    }
    file.close();
}

void writeAllBooks(const vector<LibraryEntry>& entries) {
    auto path = FILEPATH / "data" / "allbooks.json";
    ofstream file(path.string());
    if (!file.is_open()) {
        cerr << "Error opening " << path.string() << " for writing." << endl;
        return;
    }
    json j = json::array();
    for (const auto& entry : entries) {
        j.push_back({{"isbn", entry.isbn}, {"status", entry.status}, {"user_id", entry.user_id}});
    }
    file << j.dump(4);
    file.close();
}

void readBooksFrom(const string& filename, unordered_map <string, Book>& books) {
    fs::path filePath = filename;
    fs::path parentDir = filePath.parent_path();
    if (!fs::exists(parentDir)) {
        fs::create_directories(parentDir);  // Create "data/" if missing
    }
    if (!fs::exists(filename)) {
        ofstream newFile(filename);
        newFile << "[]";  // Initialize as an empty JSON array
        newFile.close();
    }
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening " << filename << endl;
        return;
    }
    json j;
    file >> j;
    for (const auto& book : j) {
        Book newbook(book["title"], book["author"], book["publisher"],book["year"],book["isbn"],book["status"],book["borrowing_user"],book["borrow_date"],book["due_date"]);
        books.emplace(newbook.isbn, newbook);
    }

    file.close();
}

void writeBooksTo(const string& filename, unordered_map <string, Book>& books) {
    fs::path filePath = filename;
    fs::path parentDir = filePath.parent_path();
    if (!fs::exists(parentDir)) {
        fs::create_directories(parentDir);  // Create "data/" if missing
    }
    if (!fs::exists(filePath)) {
        ofstream newFile(filePath);
        if (newFile.is_open()) {
            newFile << "[]";  // Initialize with an empty JSON array
            newFile.close();
        } else {
            cerr << "Error creating " << filename << endl;
            return;
        }
    }
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening " << filename << " for writing." << endl;
        return;
    }

    json j = json::array();
    for (const auto& book : books) {
        j.push_back({{"title", book.second.title}, {"author", book.second.author}, {"publisher", book.second.publisher}, {"year", book.second.year}, {"isbn", book.second.isbn}, {"status", book.second.status}, {"borrowing_user", book.second.borrowing_user}, {"borrow_date", book.second.borrow_date}, {"due_date", book.second.due_date}});
    }
    
    file << j.dump(4);
    file.close();
}
void readOnlyBooksFrom(const string& filename, vector <Book>& books) {
    fs::path filePath = filename;
    fs::path parentDir = filePath.parent_path();
    if (!fs::exists(parentDir)) {
        fs::create_directories(parentDir);  // Create "data/" if missing
    }
    if (!fs::exists(filename)) {
        ofstream newFile(filename);
        newFile << "[]";  // Initialize as an empty JSON array
        newFile.close();
    }
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening " << filename << endl;
        return;
    }
    json j;
    file >> j;
    for (const auto& book : j) {
        Book newbook(book["title"], book["author"], book["publisher"],book["year"],book["isbn"],book["status"],book["borrowing_user"],book["borrow_date"],book["due_date"]);
        books.push_back(newbook);
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
    auto path = FILEPATH / "data" / "dues" / x;
    fs::path parentDir = path.parent_path();
    if (!fs::exists(parentDir)) {
        fs::create_directories(parentDir);
    }
    if (!fs::exists(path)) {
        ofstream newFile(path);
        newFile << "[]";  // Initialize with an empty JSON array
        newFile.close();
    }
    ifstream file(path.string());
    if (!file.is_open()) {
        cerr << "Error opening " << path.string() << endl;
        return;
    }
    json j;
    file >> j;
    for (const auto& due : j) {
        dues.push_back(Dues(due["isbn"], due["start"], due["end"]));
    }
    file.close();
}

void writeDues(int id, const vector<Dues>& dues) {
    string x = to_string(id) + ".json";
    auto path = FILEPATH / "data" / "dues" / x;
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