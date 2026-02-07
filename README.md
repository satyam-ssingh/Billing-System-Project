# Billing System in C++

## Project Description
This is a Billing System project developed in C++ using file handling concepts.  
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

---

## Technologies Used
- Programming Language: C++  
- Concepts: File Handling, Functions, Loops, Conditions  
- Platform: Windows + Linux  
- Compiler: Turbo C++ / GCC  

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

## How to Run the Project
1. Open the project in Turbo C++, VS Code, or Code::Blocks  
2. Compile the program using: g++ Billing_System.cpp -o Billing_System
3. Billing_System

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
- All data is stored using file handling
- Password data stored in encrypted format (Password.dat)
- Billing records stored in text format (Bill_History.txt)
- Billing summary exported to CSV for Excel usage
- Bill numbers auto-managed using Last_Bill_Number.txt

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

