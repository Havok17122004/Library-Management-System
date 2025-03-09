# Library Management System

## Setup Instructions

### Install Dependencies

Ensure that the following components are installed on your system:

- **C++ Compiler:** G++ (for Linux/macOS) or MinGW (for Windows)
- **CMake:** Required for building the application
- **Filesystem Support:** C++17 or later is required

#### Installation on Linux/macOS

```sh
sudo apt update && sudo apt install build-essential cmake
```

#### Installation on Windows

1. Install [MinGW-w64](https://www.mingw-w64.org/)
2. Install [CMake](https://cmake.org/download/)
3. Add both MinGW and CMake to your system `PATH`

---

## Build and Run Instructions

### Linux/macOS

Execute the following commands in the terminal:

```sh
chmod +x run.sh
./run.sh
```

### Windows

Execute the following command in PowerShell:

```powershell
./run.bat
```

**Note:** The program has been primarily tested on Ubuntu 22.04. If issues occur on Windows, consider running it on Ubuntu.

---

## Application Overview

When the program starts, the initial screen displays:

- Clear instructions on the required inputs.
- The library is initialized with the following data.

     ## Library Books

| ISBN          | Title                                      | Author(s)           | Publisher      | Year | Status    |
| ------------- | ------------------------------------------ | ------------------- | -------------- | ---- | --------- |
| 9780131101630 | The C Programming Language                 | Kernighan & Ritchie | Prentice Hall  | 1988 | Available |
| 9780262033848 | Introduction to Algorithms                 | Cormen et al.       | MIT Press      | 2009 | Available |
| 9780131103627 | Computer Networks                          | Andrew Tanenbaum    | Prentice Hall  | 2010 | Available |
| 9780201633610 | Design Patterns                            | Gamma et al.        | Addison-Wesley | 1994 | Available |
| 9780132350884 | Clean Code                                 | Robert C. Martin    | Prentice Hall  | 2008 | Available |
| 9780321356680 | Effective C++                              | Scott Meyers        | Addison-Wesley | 2005 | Available |
| 9780596007126 | Head First Design Patterns                 | Eric Freeman        | O'Reilly       | 2004 | Available |
| 9780201616224 | The Pragmatic Programmer                   | Andy Hunt           | Addison-Wesley | 1999 | Available |
| 9780133594140 | Artificial Intelligence: A Modern Approach | Stuart Russell      | Pearson        | 2015 | Available |
| 9781492052203 | System Design Interview                    | Alex Xu             | ByteByteGo     | 2020 | Available |

---

## Library Accounts

| Account ID | Role      | Outstanding Dues |
|------------|----------|-----------------|
| 1          | Student  | ₹0              |
| 2          | Student  | ₹0              |
| 3          | Student  | ₹0              |
| 4          | Student  | ₹0              |
| 5          | Student  | ₹0              |
| 6          | Faculty  | ₹0              |
| 7          | Faculty  | ₹0              |
| 8          | Faculty  | ₹0              |
| 9          | Librarian | ₹0              |


### Login Procedure

- **Login:** Press `1` to log in. The application will ask you to select a role (student, faculty, or librarian) and enter your assigned ID.
- **Librarian Account Creation:** On the main page, press `2` to create a new librarian account. The system will assign a unique ID after confirmation.
- **Student/Faculty Account Creation:** New accounts for students or faculty cannot be created directly; a librarian must add these accounts through the librarian menu.
- **Additional Options:**
    - Increment the day (for testing purposes) via an option in the home menu.
    - Reset the data through the menu. This is meant for initializing the library with the predefined data, and getting rid of the changes made by the users until now.

---

## Descriptions

### Books

A book is considered available if it is neither reserved nor borrowed by any user. In this implementation, a reserved book is designated for in-library use only and cannot be borrowed. Only a librarian has the authority to reserve or unreserve a book. Each book is uniquely identified by its ISBN.

### Student Login

After logging in as a student, the following functionalities are available:

- **Dues Overview:**
    - Display of pending dues, including fines for late returns.
    - Display of the accumulating fine for overdue borrowed books (payment is permitted only after the book is returned).
- **Book Management:**
    - View available books for borrowing.
    - View books currently borrowed.
    - View and pay pending dues (partial payments are supported; dues for overdue books that have not been returned cannot be paid).
    - View the history of previously borrowed and returned books.
- **Transaction Functions:**
    - Borrow a book by entering its ISBN (librarian authorization is not required, assuming user honesty).
    - Return a borrowed book.
- **Account Options:**
    - Logout or exit the program.

### Faculty Login

The faculty login offers functionality similar to that of the student, with the exception that faculty members are not subject to fines.

### Librarian Login

When logged in as a librarian, the following functionalities are provided:

- **Library Overview:**
    - View statistics such as the total number of books and counts of available, borrowed, and reserved books.
- **Book Management:**
    - View available books.
    - View books currently borrowed by users.
    - View reserved books.
    - Reserve or unreserve a book.
    - Add, remove, or edit book details. _(Note: The ISBN, as the unique identifier, cannot be modified during an edit. To change the ISBN, delete the book and add a new entry with the correct ISBN.)_
- **User Management:**
    - Add or delete user accounts. Each user is assigned a unique ID required for login.
    - Manage user accounts for offline transactions. For example, if a user needs help borrowing or returning books in person, the librarian can handle everything directly. He can issue a book, process a return, or manage overdue fines, for the user. This ensures that users can access library services without any obstacles, even offline.

---

## Data Structure

The program uses a set of JSON files to maintain its data. I have provided this section for easy testing purposes. These JSON files are not meant to be changed explicitly by anyone. The directory structure is as follows:

```
data/
├── accounts.json
├── available_books.json
├── allbooks.json
├── reserved.json
├── current/
│   ├── 1.json
│   ├── 2.json
│   ├── 3.json
│   └── ...
├── history/
│   ├── 1.json
│   ├── 2.json
│   ├── 3.json
│   └── ...
└── dues/
    ├── 1.json
    ├── 2.json
    ├── 3.json
    └── ...
```

- **accounts.json:**  
    Stores user account information, including each user’s unique ID and any partial payment details. This file also tracks the final assigned ID to ensure sequential assignment and maintains the current system date.
    
- **Book Files:**
    
    - **available_books.json:** Contains details of books available for borrowing.
    - **reserved.json:** Contains details of books reserved for in-library use.
    - **current/{i}.json:** Contains details of books currently borrowed by the user with ID `{i}`.
    - **history/{i}.json:** Contains records of books that were previously borrowed and returned by the user with ID `{i}`.
    - **dues/{i}.json:** Contains details of dues associated with the user with ID `{i}`.
- **allbooks.json:**  
    Contains a consolidated record of all books in the library, including the ISBN, current status, and associated user ID.
    

All JSON files are updated successfully when the program exits without errors.

---
