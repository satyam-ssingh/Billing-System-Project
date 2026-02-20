# Billing Management System in C++

## Project Description
This is a Billing Management System  developed in C++ using Object-Oriented Programming (OOP) and file handling concepts for real-world billing operations.
The project is designed to generate customer bills, manage billing records, maintain login security, and store data permanently in files.
It is useful for small shops, stores, and as a college or placement-level academic project.              

---

## Features
- Secure login system with password protection
- Bill generation with automatic bill number
- Stores bill history in text files
- Export billing data to CSV (Excel supported)
- Uses file handling for permanent data storage
- Simple console-based user interface
- Update existing bills
- Delete bills from system
- Search bills using phone number
- View bill details by bill number
- Track product sales and generate reports
- Maintain operator login history
- Records login history of users with date and time
- Allows checking and viewing login history records
- Password recovery using security questions
- Account lock system after multiple failed login attempts
- Input validation and error handling

---

## Technologies Used
- Programming Language: C++

## Concepts:
- Object-Oriented Programming (OOP)
- File Handling
- Functions & Modular Programming
- Loops & Conditional Statements
- Input Validation & Error Handling
- Data Structures (Vector, Arrays)
- Static Members
- Encapsulation
- Constructors & Destructors
- Menu-driven Program Design

## Security Features:
- XOR-based Encryption
- Password Protection System
- Hidden Password Input (Masked Input)
- Security Questions for Recovery
- Account Lock System (Multiple Failed Attempts)
- Basic Authentication System

## Data Management:
- Text File Handling
- CSV File Handling (Excel Compatible)
- Persistent Data Storage
- Record Management System (Add, Update, Delete, Search)
- File Parsing & Data Serialization

## Platform:
- Windows
- Linux

## Libraries:
- iostream
- fstream
- vector
- string
- cstdlib
- cstdio
- conio.h (Windows)
- termios (Linux)

## Tools & Development Environment:
- Visual Studio Code (VS Code)
- Command Line Interface (CLI)
- Git (Version Control - optional)

## Compiler:
- GCC (g++, MinGW for Windows)
- Turbo C++

---

## Project Files
- Billing_System.cpp – Main source code  
- Bill_History.txt – Stores all generated bills  
- Excel_History.csv – Billing data for Excel  
- Last_Bill_Number.txt – Stores last bill number  
- login.txt – Login details  
- Password.dat – Password file  
- Security.dat – Security related data  

---

## System Requirements
- Operating System: Windows / Linux
- Compiler: GCC / G++
- Minimum RAM: 2 GB
- Disk Space: 50 MB
- Terminal / Console Access

  ---

## Security Implementation
- Password protection with encrypted storage
- XOR-based encryption for password and security answers
- Hidden password input in console
- Account lock system after multiple failed login attempts
- Password recovery using security questions

  ---

## Data Storage
- All data is stored using file handling to ensure permanent storage.
- Password data is stored in encrypted format (Password.dat) for security.
- Billing records are stored in text files (Bill_History.txt) for easy access and management.
- Billing summary can be exported to CSV format for Excel compatibility.
- Bill numbers are automatically generated and managed using Last_Bill_Number.txt.
- Login history is maintained in a separate file (Login_History.txt) to track user activity.
- Deleted records can be managed or logged for tracking purposes (Deleted_Bills.txt).
- Data is structured in a readable format for easy parsing and retrieval.
- File-based database system is used instead of external database.
- Backup of important data can be created manually for data safety.
- Error handling is implemented to prevent data loss during file operations.
  
  ---

## Validation and Error Handling
- Input validation for names, phone numbers, quantity, and price
- Prevents invalid or empty inputs
- Handles file read/write errors safely
- Protects system from accidental data corruption

  ---

## Key Learning Outcomes
- Practical implementation of C++ file handling
- Cross-platform console programming (Windows & Linux)
- Secure password handling in C++
- Real-world billing system design
- Error handling and input validation

  ---
## How to Run the Project
1. Open the project in Turbo C++, VS Code, or Code::Blocks  
2. Compile the program using: g++ Billing_System.cpp -o Billing_System
3. Billing_System


