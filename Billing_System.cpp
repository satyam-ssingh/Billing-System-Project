#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <ctime>
#include <vector>
#include <sstream>
#include <limits>
#include <iomanip>

#ifdef _WIN32
    #include <conio.h>      // Windows: _getch()
    #include <windows.h>    // Windows: Sleep()
#else
    #include <termios.h>    // Linux: Terminal settings
    #include <unistd.h>     // Linux: usleep(), STDIN_FILENO
#endif

using namespace std;

// ======================== GLOBAL UTILITY FUNCTIONS ========================

// Encryption/Decryption function: XOR is symmetric (same for both)
string securityEngine(string data) {
    char key = 'S'; // Secret XOR Key
    string output = data;
    for (int i = 0; i < data.length(); i++) {
        output[i] = data[i] ^ key;
    }
    return output;
}

// Check if file exists
bool isFileExists(string name) {
    ifstream f(name.c_str());
    if (f.good()) {
        f.close();
        return true;
    }
    return false;
}

// ======================== SECURITY QUESTIONS SYSTEM ========================

struct SecurityQuestion {
    string question;
    string answer;
};

// Save security questions to file
void saveSecurityQuestions(SecurityQuestion q1, SecurityQuestion q2) {
    ofstream outFile("Security.dat");
    if (outFile.is_open()) {
        outFile << securityEngine(q1.question) << endl;
        outFile << securityEngine(q1.answer) << endl;
        outFile << securityEngine(q2.question) << endl;
        outFile << securityEngine(q2.answer) << endl;
        outFile.close();
    }
}

// Load security questions from file
bool loadSecurityQuestions(SecurityQuestion &q1, SecurityQuestion &q2) {
    ifstream inFile("Security.dat");
    if (!inFile.is_open()) {
        return false;
    }
    
    string line;
    int count = 0;
    while (getline(inFile, line) && count < 4) {
        if (count == 0) q1.question = securityEngine(line);
        else if (count == 1) q1.answer = securityEngine(line);
        else if (count == 2) q2.question = securityEngine(line);
        else if (count == 3) q2.answer = securityEngine(line);
        count++;
    }
    inFile.close();
    
    return (count == 4);
}

// Password Recovery Function
bool recoverPassword() {
    cout << "\n========================================" << endl;
    cout << "|      PASSWORD RECOVERY SYSTEM        |" << endl;
    cout << "========================================\n" << endl;
    
    SecurityQuestion q1, q2;
    
    if (!loadSecurityQuestions(q1, q2)) {
        cout << "[ERROR] No security questions found!" << endl;
        cout << "[INFO] Password recovery is not available." << endl;
        cout << "[INFO] Please contact system administrator." << endl;
        return false;
    }
    
    cout << "Answer the following security questions:\n" << endl;
    
    // Question 1
    cout << "Question 1: " << q1.question << endl;
    cout << "Your Answer: ";
    string ans1;
    getline(cin, ans1);
    
    // Question 2
    cout << "\nQuestion 2: " << q2.question << endl;
    cout << "Your Answer: ";
    string ans2;
    getline(cin, ans2);
    
    // Verify answers (case-insensitive)
    for(int i = 0; i < ans1.length(); i++) ans1[i] = tolower(ans1[i]);
    for(int i = 0; i < ans2.length(); i++) ans2[i] = tolower(ans2[i]);
    
    string correctAns1 = q1.answer;
    string correctAns2 = q2.answer;
    for(int i = 0; i < correctAns1.length(); i++) correctAns1[i] = tolower(correctAns1[i]);
    for(int i = 0; i < correctAns2.length(); i++) correctAns2[i] = tolower(correctAns2[i]);
    
    if (ans1 == correctAns1 && ans2 == correctAns2) {
        cout << "\n[SUCCESS] Security questions verified!" << endl;
        
        // Allow password reset
        cout << "\nEnter New Password: ";
        string newPass;
        getline(cin, newPass);
        
        if (newPass.empty()) {
            cout << "[ERROR] Password cannot be empty!" << endl;
            return false;
        }
        
        cout << "Confirm New Password: ";
        string confirmPass;
        getline(cin, confirmPass);
        
        if (newPass != confirmPass) {
            cout << "[ERROR] Passwords do not match!" << endl;
            return false;
        }
        
        // Save new password
        string encryptedPass = securityEngine(newPass);
        ofstream outFile("Password.dat");
        if (outFile.is_open()) {
            outFile << encryptedPass;
            outFile.close();
            cout << "\n[SUCCESS] Password has been reset successfully!" << endl;
            cout << "[INFO] Please restart the system to use your new password." << endl;
            return true;
        } else {
            cout << "[ERROR] Failed to update password!" << endl;
            return false;
        }
    } else {
        cout << "\n[ERROR] Incorrect answers!" << endl;
        cout << "[INFO] Password recovery failed." << endl;
        return false;
    }
}

// ======================== PASSWORD MANAGER FUNCTIONS ========================

void passwordManager() {
    int choice;
    string pass, oldPass, newPass, savedPass;

    while (true) {
        cout << "\n ________________________________________" << endl;
        cout << "|   OFFICIAL ADMIN CREDENTIAL MANAGER    |" << endl;
        cout << "|========================================|" << endl;
        cout << "| 1. Create System Password (First Time) |" << endl;
        cout << "| 2. Change Existing Password            |" << endl;
        cout << "| 3. Verify Current Password             |" << endl;
        cout << "| 4. Setup/Update Security Questions     |" << endl;
        cout << "| 5. Back to Main Menu                   |" << endl;
        cout << "|________________________________________|" << endl;
        cout << "Select Option: ";
        
        // Input validation
        if (!(cin >> choice)) {
            cout << "\n[ERROR] Invalid input! Please enter a number between 1-5." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        // Range validation
        if (choice < 1 || choice > 5) {
            cout << "\n[ERROR] Invalid option! Please choose a number between 1-5." << endl;
            continue;
        }

        if (choice == 1) {
            // Option 1: Create New Password
            if (isFileExists("Password.dat")) {
                cout << "\n[!] Error: Password already exists. Use Option 2 to Change." << endl;
            } else {
                cout << "Enter New Admin Password: ";
                getline(cin, pass);
                
                if (pass.empty()) {
                    cout << "[ERROR] Password cannot be empty!" << endl;
                    continue;
                }
                
                cout << "Confirm Password: ";
                string confirmPass;
                getline(cin, confirmPass);
                
                if (pass != confirmPass) {
                    cout << "[ERROR] Passwords do not match!" << endl;
                    continue;
                }
                
                // Encrypt password
                string encryptedPass = securityEngine(pass);
                
                // Save to file
                ofstream outFile("Password.dat");
                if (outFile.is_open()) {
                    outFile << encryptedPass;
                    outFile.close();
                    cout << "[SUCCESS] Password.dat created successfully!" << endl;
                    cout << "[INFO] Your password has been saved securely." << endl;
                    
                    // Prompt to setup security questions
                    cout << "\n[IMPORTANT] Would you like to setup security questions for password recovery? (Y/N): ";
                    char setupSecurity;
                    cin >> setupSecurity;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    
                    if (setupSecurity == 'Y' || setupSecurity == 'y') {
                        // Setup security questions
                        SecurityQuestion q1, q2;
                        
                        cout << "\nSecurity Question 1: What is your mother's maiden name?" << endl;
                        q1.question = "What is your mother's maiden name?";
                        cout << "Your Answer: ";
                        getline(cin, q1.answer);
                        
                        cout << "\nSecurity Question 2: What is your favorite pet's name?" << endl;
                        q2.question = "What is your favorite pet's name?";
                        cout << "Your Answer: ";
                        getline(cin, q2.answer);
                        
                        if (!q1.answer.empty() && !q2.answer.empty()) {
                            saveSecurityQuestions(q1, q2);
                            cout << "\n[SUCCESS] Security questions saved successfully!" << endl;
                        } else {
                            cout << "\n[WARNING] Security questions not saved (empty answers)." << endl;
                        }
                    }
                } else {
                    cout << "[ERROR] Failed to create Password.dat file!" << endl;
                }
            }
        } 
        else if (choice == 2) {
            // Option 2: Change Password
            if (!isFileExists("Password.dat")) {
                cout << "\n[!] Error: No password file found. Use Option 1 to create password first." << endl;
            } else {
                cout << "Enter Old Password: ";
                getline(cin, oldPass);
                
                // Read encrypted password from file
                ifstream inFile("Password.dat");
                if (!inFile.is_open()) {
                    cout << "[ERROR] Cannot open Password.dat!" << endl;
                    continue;
                }
                
                getline(inFile, savedPass);
                inFile.close();

                // Decrypt and compare
                string decryptedPass = securityEngine(savedPass);
                
                if (oldPass == decryptedPass) {
                    cout << "Enter New Password: ";
                    getline(cin, newPass);
                    
                    if (newPass.empty()) {
                        cout << "[ERROR] Password cannot be empty!" << endl;
                        continue;
                    }
                    
                    cout << "Confirm New Password: ";
                    string confirmPass;
                    getline(cin, confirmPass);
                    
                    if (newPass != confirmPass) {
                        cout << "[ERROR] Passwords do not match!" << endl;
                        continue;
                    }
                    
                    // Encrypt new password
                    string encryptedNewPass = securityEngine(newPass);
                    
                    // Save to file
                    ofstream outFile("Password.dat");
                    if (outFile.is_open()) {
                        outFile << encryptedNewPass;
                        outFile.close();
                        cout << "[SUCCESS] Password updated in Password.dat!" << endl;
                    } else {
                        cout << "[ERROR] Failed to update password!" << endl;
                    }
                } else {
                    cout << "[ERROR] Wrong old password!" << endl;
                }
            }
        }
        else if (choice == 3) {
            // Option 3: Verify Password
            if (!isFileExists("Password.dat")) {
                cout << "\n[!] Error: Password file not found! Use Option 1 to create password first." << endl;
            } else {
                cout << "Verify Password: ";
                getline(cin, pass);
                
                ifstream inFile("Password.dat");
                if (!inFile.is_open()) {
                    cout << "[ERROR] Cannot open Password.dat!" << endl;
                    continue;
                }
                
                getline(inFile, savedPass);
                inFile.close();

                // Decrypt and compare
                string decryptedPass = securityEngine(savedPass);
                
                if (pass == decryptedPass) {
                    cout << "[MATCHED] Credential is correct." << endl;
                } else {
                    cout << "[FAILED] Incorrect password." << endl;
                }
            }
        }
        else if (choice == 4) {
            // Option 4: Setup/Update Security Questions
            cout << "\n========================================" << endl;
            cout << "|   SECURITY QUESTIONS SETUP/UPDATE    |" << endl;
            cout << "========================================\n" << endl;
            
            SecurityQuestion q1, q2;
            
            cout << "Security Question 1: What is your mother's maiden name?" << endl;
            q1.question = "What is your mother's maiden name?";
            cout << "Your Answer: ";
            getline(cin, q1.answer);
            
            cout << "\nSecurity Question 2: What is your favorite pet's name?" << endl;
            q2.question = "What is your favorite pet's name?";
            cout << "Your Answer: ";
            getline(cin, q2.answer);
            
            if (!q1.answer.empty() && !q2.answer.empty()) {
                saveSecurityQuestions(q1, q2);
                cout << "\n[SUCCESS] Security questions saved successfully!" << endl;
                cout << "[INFO] You can now use 'FORGOT' option during login to recover password." << endl;
            } else {
                cout << "\n[ERROR] Answers cannot be empty!" << endl;
            }
        }
        else if (choice == 5) {
            // Option 5: Exit
            cout << "\nReturning to Main Menu..." << endl;
            break;
        }
    }
}

// Function to get hidden password input
string getHiddenPassword() {
    string password;
    
#ifdef _WIN32
    // Windows implementation
    char ch;
    while ((ch = _getch()) != '\r') { // '\r' is Enter key
        if (ch == '\b') { // Backspace
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b"; // Remove last asterisk
            }
        } else {
            password += ch;
            cout << '*';
        }
    }
#else
    // Linux/Unix implementation
    termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    termios newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    getline(cin, password);
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    
    // Print asterisks for visual feedback
    for(size_t i = 0; i < password.length(); i++) {
        cout << '*';
    }
#endif
    
    cout << endl;
    return password;
}

// Check if a string contains only numeric digits
bool isNumeric(const string& str) {
    if (str.empty()) return false;
    for (size_t i = 0; i < str.length(); i++) {
        if (!isdigit(str[i])) return false;
    }
    return true;
}

// Check if a string is a valid number (supports decimals)
bool isValidNumber(const string& str) {
    if (str.empty()) return false;
    bool hasDecimal = false;
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] == '.' && !hasDecimal) {
            hasDecimal = true;
        } else if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

// Check if a string contains only alphabets and spaces (for names)
bool isValidName(const string& str) {
    if (str.empty()) return false;
    for (size_t i = 0; i < str.length(); i++) {
        if (!isalpha(str[i]) && str[i] != ' ') {
            return false;
        }
    }
    return true;
}

// Get a validated name (only alphabets and spaces allowed)
string getValidatedName(const string& prompt) {
    string name;
    while (true) {
        cout << prompt;
        getline(cin, name);
        
        if (isValidName(name) && !name.empty()) {
            return name;
        } else {
            cout << "Invalid Name! Please enter only alphabets and spaces.\n";
        }
    }
}

// Get a validated 10-digit phone number from user
string getValidatedPhone() {
    string phone;
    while (true) {
        cout << "Enter Phone Number: ";
        getline(cin, phone);
        
        if (phone.length() == 10 && isNumeric(phone)) {
            return phone;
        } else {
            cout << "Invalid Phone Number! Please enter exactly 10 digits.\n";
        }
    }
}

// Get a validated integer input from user with error handling and maximum limit
int getValidatedInt(const string& prompt, int maxLimit = -1) {
    string input;
    while (true) {
        if (!prompt.empty()) {
            cout << prompt;
        }
        getline(cin, input);
        
        if (isNumeric(input) && !input.empty()) {
            try {
                int value = stoi(input);
                if (maxLimit > 0 && value > maxLimit) {
                    cout << "Invalid Input! Maximum allowed value is " << maxLimit << ".\n";
                    continue;
                }
                return value;
            } catch (...) {
                cout << "Invalid Input! Please enter a valid number.\n";
            }
        } else {
            cout << "Invalid Input! Please enter a valid number.\n";
        }
    }
}

// Get a validated float input from user with error handling
float getValidatedFloat(const string& prompt, float maxLimit = -1) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        
        if (isValidNumber(input) && !input.empty()) {
            try {
                float value = stof(input);
                if (maxLimit > 0 && value > maxLimit) {
                    cout << "Invalid Input! Maximum allowed value is " << maxLimit << ".\n";
                    continue;
                }
                return value;
            } catch (...) {
                cout << "Invalid Input! Please enter a valid number.\n";
            }
        } else {
            cout << "Invalid Input! Please enter a valid number.\n";
        }
    }
}

// ======================== ITEM CLASS ========================
class Item {
private:
    string name;
    int quantity;
    float price;

public:
    // Default constructor
    Item() : name(""), quantity(0), price(0.0) {}
    
    // Parameterized constructor
    Item(string n, int q, float p) : name(n), quantity(q), price(p) {}

    // Get item name
    string getName() const { return name; }
    
    // Get item quantity
    int getQuantity() const { return quantity; }
    
    // Get item price
    float getPrice() const { return price; }
    
    // Calculate and return total price (quantity * price)
    float getTotal() const { return quantity * price; }

    // Set item name
    void setName(string n) { name = n; }
    
    // Set item quantity
    void setQuantity(int q) { quantity = q; }
    
    // Set item price
    void setPrice(float p) { price = p; }

    // Input item details from user with validation
    void inputDetails() {
        name = getValidatedName("Enter Item Name: ");
        quantity = getValidatedInt("Enter Quantity: ", 10000);
        price = getValidatedFloat("Enter Price: ");
    }
};

// ======================== BILL SUMMARY CLASS ========================
class BillSummary {
private:
    string name;
    string phone;
    string date;
    string billNum;
    float total;

public:
    // Default constructor
    BillSummary() : name(""), phone(""), date(""), billNum(""), total(0.0) {}

    // Get customer name
    string getName() const { return name; }
    
    // Get phone number
    string getPhone() const { return phone; }
    
    // Get bill date
    string getDate() const { return date; }
    
    // Get bill number
    string getBillNum() const { return billNum; }
    
    // Get total amount
    float getTotal() const { return total; }

    // Set customer name
    void setName(string n) { name = n; }
    
    // Set phone number
    void setPhone(string p) { phone = p; }
    
    // Set bill date
    void setDate(string d) { date = d; }
    
    // Set bill number
    void setBillNum(string b) { billNum = b; }
    
    // Set total amount
    void setTotal(float t) { total = t; }
};

// ======================== LOGIN RECORD CLASS ========================
class LoginRecord {
private:
    string operatorName;
    string operatorPhone;
    string loginTime;

public:
    LoginRecord() : operatorName(""), operatorPhone(""), loginTime("") {}
    
    string getOperatorName() const { return operatorName; }
    string getOperatorPhone() const { return operatorPhone; }
    string getLoginTime() const { return loginTime; }
    
    void setOperatorName(string n) { operatorName = n; }
    void setOperatorPhone(string p) { operatorPhone = p; }
    void setLoginTime(string t) { loginTime = t; }
};

// ======================== PRODUCT SUMMARY CLASS ========================
class ProductSummary {
private:
    string billNum;
    string dateTime;
    string productName;
    int quantity;
    float rate;
    float subtotal;

public:
    ProductSummary() : billNum(""), dateTime(""), productName(""), 
                       quantity(0), rate(0.0), subtotal(0.0) {}
    
    string getBillNum() const { return billNum; }
    string getDateTime() const { return dateTime; }
    string getProductName() const { return productName; }
    int getQuantity() const { return quantity; }
    float getRate() const { return rate; }
    float getSubtotal() const { return subtotal; }
    
    void setBillNum(string b) { billNum = b; }
    void setDateTime(string d) { dateTime = d; }
    void setProductName(string p) { productName = p; }
    void setQuantity(int q) { quantity = q; }
    void setRate(float r) { rate = r; }
    void setSubtotal(float s) { subtotal = s; }
};

// ======================== BILL NUMBER MANAGER CLASS ========================
class BillNumberManager {
private:
    static int currentBillNumber;
    static bool initialized;

    // Load the last bill number from file
    static void loadLastBillNumber() {
        ifstream file("Last_Bill_Number.txt");
        if (file.is_open()) {
            file >> currentBillNumber;
            file.close();
        } else {
            currentBillNumber = 0;
        }
        initialized = true;
    }

    // Save the current bill number to file
    static void saveCurrentBillNumber() {
        ofstream file("Last_Bill_Number.txt");
        if (file.is_open()) {
            file << currentBillNumber;
            file.close();
        }
    }

public:
    // Get next bill number and increment
    static string getNextBillNumber() {
        if (!initialized) {
            loadLastBillNumber();
        }
        currentBillNumber++;
        saveCurrentBillNumber();
        
        // Modern C++ way using stringstream - NO SPRINTF ERROR
        ostringstream oss;
        oss << "BILL" << setfill('0') << setw(3) << currentBillNumber;
        return oss.str();
    }
    // Update bill number after deletion (recalculate from history)
    static void recalculateFromHistory() {
        ifstream file("Bill_History.txt");
        string line;
        int maxNum = 0;
        
        while (getline(file, line)) {
            size_t pos = line.find("Bill Number   : BILL");
            if (pos != string::npos) {
                string numStr = line.substr(pos + 20);
                size_t endPos = numStr.find(" ");
                if (endPos != string::npos) {
                    numStr = numStr.substr(0, endPos);
                }
                if (!numStr.empty()) {
                    try {
                        int num = stoi(numStr);
                        if (num > maxNum) maxNum = num;
                    } catch (...) {
                        continue;
                    }
                }
            }
        }
        file.close();

        currentBillNumber = maxNum;
        saveCurrentBillNumber();
    }
};

// Initialize static members
int BillNumberManager::currentBillNumber = 0;
bool BillNumberManager::initialized = false;

// ======================== BILL CLASS ========================
class Bill {
private:
    string customerName;
    string phoneNumber;
    string billNumber;
    string dateTime;
    vector<Item> items;
    float subtotal;
    float gstAmount;
    float discountPercent;
    float discountAmount;
    float grandTotal;
    const float GST_RATE;

    // Calculate all totals including GST and discount
    void calculateTotal() {
        subtotal = 0;
        for (size_t i = 0; i < items.size(); i++) {
            subtotal += items[i].getTotal();
        }
        
        // Calculate GST
        gstAmount = subtotal * (GST_RATE / 100.0);
        
        // Get discount from user
        discountPercent = getValidatedFloat("Enter Discount Percentage (0-100): ", 100.0);
        discountAmount = (subtotal + gstAmount) * (discountPercent / 100.0);
        
        // Calculate grand total
        grandTotal = subtotal + gstAmount - discountAmount;
    }

    // Get current system date and time
    string getCurrentDateTime() {
        time_t now = time(0);
        string dt = ctime(&now);
        if (!dt.empty()) {
            dt.pop_back();
        }
        return dt;
    }

    // Save bill to CSV format file
    void saveToCSV() const {
        bool fileExists = false;
        ifstream checkFile("Excel_History.csv");
        if (checkFile.good()) {
            checkFile.seekg(0, ios::end);
            fileExists = (checkFile.tellg() > 0);
        }
        checkFile.close();
        
        ofstream file("Excel_History.csv", ios::app);
        
        // Add header if file is empty
        if (!fileExists) {
            file << "Bill Number,Date,Customer Name,Phone Number,Grand Total\n";
        }
        
        // Extract only date from dateTime
        string dateOnly = dateTime.substr(4, 6);
        string year = dateTime.substr(dateTime.length() - 4, 4);
        dateOnly = dateOnly + " " + year;
        
        file << billNumber << ","
             << dateOnly << ","
             << customerName << ","
             << phoneNumber << ","
             << fixed << setprecision(2) << grandTotal << "\n";
        
        file.close();
    }

public:
    // Default constructor
    Bill() : customerName(""), phoneNumber(""), billNumber(""), dateTime(""), 
             subtotal(0.0), gstAmount(0.0), discountPercent(0.0), 
             discountAmount(0.0), grandTotal(0.0), GST_RATE(18.0) {}

    // Input all bill details from user with validation
    void inputBillDetails() {
        customerName = getValidatedName("Enter Customer Name: ");
        phoneNumber = getValidatedPhone();
        
        int n = getValidatedInt("Enter number of items: ", 10000);

        items.resize(n);
        for (int i = 0; i < n; i++) {
            cout << "\nItem " << (i+1) << ":\n";
            items[i].inputDetails();
        }

        dateTime = getCurrentDateTime();
        billNumber = BillNumberManager::getNextBillNumber();
        calculateTotal();
    }

    // Display bill on console in formatted box style
    void printToConsole() const {
        cout << "\n ______________________________________________________\n";
        cout << "|                        BILL                          |\n";
        cout << "|======================================================|\n";
        cout << "| Customer Name : " << left << setw(35) << customerName << "  |\n";
        cout << "| Phone Number  : " << left << setw(35) << phoneNumber << "  |\n";
        cout << "| Date & Time   : " << left << setw(35) << dateTime << "  |\n";
        cout << "| Bill Number   : " << left << setw(35) << billNumber << "  |\n";
        cout << "|------------------------------------------------------|\n";
        cout << "| Item Name          | Qty  | Price    | Total         |\n";
        cout << "|--------------------|------|----------|---------------|\n";

        for (size_t i = 0; i < items.size(); i++) {
            cout << "| " << left << setw(18) << items[i].getName()
                 << " | " << right << setw(4) << items[i].getQuantity()
                 << " | " << setw(8) << fixed << setprecision(2) << items[i].getPrice()
                 << " | " << setw(13) << items[i].getTotal() << " |\n";
        }

        cout << "|------------------------------------------------------|\n";
        cout << "| Subtotal      : " << right << setw(36) << fixed << setprecision(2) << subtotal << " |\n";
        cout << "| GST (18%)     : " << right << setw(36) << fixed << setprecision(2) << gstAmount << " |\n";
        
        // Only show discount if it's greater than 0
        if (discountPercent > 0) {
            ostringstream discLabel;
            discLabel << "Discount (" << fixed << setprecision(2) << discountPercent << "%)";
            cout << "| " << left << setw(14) << discLabel.str() << ": " 
                 << right << setw(34) << fixed << setprecision(2) << discountAmount << " |\n";
        }
        
        cout << "|------------------------------------------------------|\n";
        cout << "| Grand Total   : " << right << setw(36) << fixed << setprecision(2) << grandTotal << " |\n";
        cout << "|______________________________________________________|\n";
    }

    // Save bill to file for future reference
    void saveToFile() const {
        ofstream file("Bill_History.txt", ios::app);
        file << " ______________________________________________________\n";
        file << "|                    BILL HISTORY                      |\n";
        file << "|======================================================|\n";
        file << "| Customer Name : " << left << setw(35) << customerName << "  |\n";
        file << "| Phone Number  : " << left << setw(35) << phoneNumber << "  |\n";
        file << "| Date & Time   : " << left << setw(35) << dateTime << "  |\n";
        file << "| Bill Number   : " << left << setw(35) << billNumber << "  |\n";
        file << "|------------------------------------------------------|\n";
        file << "| Item Name          | Qty  | Price    | Total         |\n";
        file << "|--------------------|------|----------|---------------|\n";

        for (size_t i = 0; i < items.size(); i++) {
            file << "| " << left << setw(18) << items[i].getName()
                 << " | " << right << setw(4) << items[i].getQuantity()
                 << " | " << setw(8) << fixed << setprecision(2) << items[i].getPrice()
                 << " | " << setw(13) << items[i].getTotal() << " |\n";
        }

        file << "|------------------------------------------------------|\n";
        file << "| Subtotal      : " << right << setw(36) << fixed << setprecision(2) << subtotal << " |\n";
        file << "| GST (18%)     : " << right << setw(36) << fixed << setprecision(2) << gstAmount << " |\n";
        
        // Only show discount if it's greater than 0
        if (discountPercent > 0) {
            ostringstream discLabel;
            discLabel << "Discount (" << fixed << setprecision(2) << discountPercent << "%)";
            file << "| " << left << setw(14) << discLabel.str() << ": " 
                 << right << setw(34) << fixed << setprecision(2) << discountAmount << " |\n";
        }
        
        file << "|------------------------------------------------------|\n";
        file << "| Grand Total   : " << right << setw(36) << fixed << setprecision(2) << grandTotal << " |\n";
        file << "|______________________________________________________|\n\n";
        file.close();

        // Save to CSV format file
        saveToCSV();

        cout << "\nBill Saved Successfully (Bill No: " << billNumber << ")\n";
    }
    
    // Get bill number
    string getBillNumber() const { return billNumber; }
    
    // Get bill date time
    string getDateTime() const { return dateTime; }
    
    // Get items
    vector<Item> getItems() const { return items; }
};

// ======================== BILL MANAGER CLASS ========================
class BillManager {
private:
    // Search and retrieve all bills for a specific phone number
    vector<BillSummary> getBillsByPhone(const string& phone) {
        vector<BillSummary> summaries;
        ifstream file("Bill_History.txt");
        string line;
        vector<string> currentBill;
        bool inBill = false;

        while (getline(file, line)) {
            if (line.find("______________________________________________________") != string::npos) {
                if (inBill && !currentBill.empty()) {
                    // Check if this bill has the phone
                    bool foundPhone = false;
                    for (size_t i = 0; i < currentBill.size(); i++) {
                        if (currentBill[i].find("Phone Number  : " + phone) != string::npos ||
                            (currentBill[i].find("Phone Number  : ") != string::npos && 
                             currentBill[i].substr(18).find(phone) != string::npos)) {
                            foundPhone = true;
                            break;
                        }
                    }
                    
                    if (foundPhone) {
                        BillSummary bs;
                        for (size_t i = 0; i < currentBill.size(); i++) {
                            if (currentBill[i].find("Customer Name : ") != string::npos) {
                                string name = currentBill[i].substr(18, 35);
                                name.erase(name.find_last_not_of(" |") + 1);
                                bs.setName(name);
                            }
                            if (currentBill[i].find("Date & Time   : ") != string::npos) {
                                string date = currentBill[i].substr(18, 35);
                                date.erase(date.find_last_not_of(" |") + 1);
                                bs.setDate(date);
                            }
                            if (currentBill[i].find("Bill Number   : ") != string::npos) {
                                string billNum = currentBill[i].substr(18, 35);
                                billNum.erase(billNum.find_last_not_of(" |") + 1);
                                bs.setBillNum(billNum);
                            }
                            if (currentBill[i].find("Phone Number  : ") != string::npos) {
                                string ph = currentBill[i].substr(18, 35);
                                ph.erase(ph.find_last_not_of(" |") + 1);
                                bs.setPhone(ph);
                            }
                            if (currentBill[i].find("Grand Total   : ") != string::npos) {
                                try {
                                    string totalStr = currentBill[i].substr(currentBill[i].find_last_of("0123456789.") - 10, 15);
                                    bs.setTotal(stof(totalStr));
                                } catch (...) {
                                    bs.setTotal(0.0);
                                }
                            }
                        }
                        summaries.push_back(bs);
                    }
                }
                currentBill.clear();
                inBill = true;
            }
            if (inBill) {
                currentBill.push_back(line);
            }
        }
        file.close();
        return summaries;
    }
    // Display list of bills in formatted table
    void displayBillList(const vector<BillSummary>& bills) {
        if (bills.empty()) {
            cout << "No Bills Found for this Phone Number\n";
            return;
        }
        cout << "\nFound " << bills.size() << " Bill(s):\n";
        cout << " ___________________________________________________________________________________\n";
        cout << "|                                    BILL CHART                                     |\n";
        cout << "|===================================================================================|\n";
        cout << "| S.No | Name                 | Date                   | Bill No  | Total           |\n";
        cout << "|------|----------------------|------------------------|----------|-----------------|\n";
        for (size_t i = 0; i < bills.size(); ++i) {
            cout << "| " << left << setw(4) << (i+1)
                 << " | " << setw(20) << bills[i].getName().substr(0,20)
                 << " | " << setw(22) << bills[i].getDate().substr(0,22)
                 << " | " << setw(8) << bills[i].getBillNum()
                 << " | " << right << setw(14) << fixed << setprecision(2) << bills[i].getTotal() << "  |\n";
        }
        cout << "|___________________________________________________________________________________|\n";
    }

    // Remove a specific bill from file by bill number
    bool removeBillByNumber(const string& billNum) {
        ifstream in("Bill_History.txt");
        if (!in) return false;

        vector<string> lines;
        string line;
        vector<string> currentBill;
        bool foundTarget = false;

        while (getline(in, line)) {
            currentBill.push_back(line);
            
            if (line.find("|______________________________________________________|") != string::npos) {
                bool isTargetBill = false;
                for (size_t i = 0; i < currentBill.size(); i++) {
                    if (currentBill[i].find("Bill Number   : " + billNum) != string::npos) {
                        isTargetBill = true;
                        foundTarget = true;
                        break;
                    }
                }
                
                if (!isTargetBill) {
                    for (size_t i = 0; i < currentBill.size(); i++) {
                        lines.push_back(currentBill[i]);
                    }
                    lines.push_back("");
                }
                
                currentBill.clear();
            }
        }
        in.close();

        if (foundTarget) {
            ofstream out("Bill_History.txt");
            for (size_t i = 0; i < lines.size(); ++i) {
                out << lines[i];
                if (i < lines.size() - 1) out << endl;
            }
            out.close();
            
            // Also remove from CSV
            removeFromCSV(billNum);
            
            // Recalculate bill numbers after deletion
            BillNumberManager::recalculateFromHistory();
        }
        return foundTarget;
    }

    // Remove bill from CSV file
    void removeFromCSV(const string& billNum) {
        ifstream in("Excel_History.csv");
        if (!in) return;

        vector<string> lines;
        string line;
        
        while (getline(in, line)) {
            // Keep header and lines that don't contain the bill number
            if (line.find("Bill Number") != string::npos || 
                line.find(billNum) == string::npos) {
                lines.push_back(line);
            }
        }
        in.close();

        ofstream out("Excel_History.csv");
        for (size_t i = 0; i < lines.size(); ++i) {
            out << lines[i];
            if (i < lines.size() - 1) out << endl;
        }
        out.close();
    }

    // Display a specific bill by bill number
    void printBillByNumber(const string& billNum) {
        ifstream file("Bill_History.txt");
        string line;
        vector<string> currentBill;
        bool found = false;

        while (getline(file, line)) {
            currentBill.push_back(line);
            
            if (line.find("|______________________________________________________|") != string::npos) {
                for (size_t i = 0; i < currentBill.size(); i++) {
                    if (currentBill[i].find("Bill Number   : " + billNum) != string::npos) {
                        found = true;
                        break;
                    }
                }
                
                if (found) {
                    cout << "\n";
                    for (size_t i = 0; i < currentBill.size(); i++) {
                        cout << currentBill[i] << endl;
                    }
                    cout << endl;
                    file.close();
                    return;
                }
                
                currentBill.clear();
            }
        }
        file.close();
        
        if (!found) {
            cout << "Bill Not Found!\n";
        }
    }

    // Get product sales data from bills
    vector<ProductSummary> getProductSalesData(int days) {
        vector<ProductSummary> products;
        ifstream file("Bill_History.txt");
        string line;
        bool inBill = false;
        bool inItems = false;
        
        string currentBillNum = "";
        string currentDateTime = "";

        while (getline(file, line)) {
            if (line.find("______________________________________________________") != string::npos) {
                inBill = true;
                inItems = false;
            }
            
            if (inBill) {
                // Extract bill number
                if (line.find("Bill Number   : ") != string::npos) {
                    currentBillNum = line.substr(18, 35);
                    currentBillNum.erase(currentBillNum.find_last_not_of(" |") + 1);
                }
                
                // Extract date time
                if (line.find("Date & Time   : ") != string::npos) {
                    currentDateTime = line.substr(18, 35);
                    currentDateTime.erase(currentDateTime.find_last_not_of(" |") + 1);
                }
                
                // Detect items section
                if (line.find("|--------------------|------|----------|---------------|") != string::npos) {
                    inItems = true;
                    continue;
                }
                
                // End of items section
                if (inItems && line.find("|------------------------------------------------------|") != string::npos) {
                    inItems = false;
                    inBill = false;
                }
                
                // Parse item lines
                if (inItems && line.length() > 10 && line[0] == '|' && line.find("Item Name") == string::npos) {
                    try {
                        // Parse: | Item Name          | Qty  | Price    | Total        |
                        size_t pos1 = 2;
                        size_t pos2 = line.find("|", pos1);
                        string itemName = line.substr(pos1, pos2 - pos1);
                        itemName.erase(itemName.find_last_not_of(" ") + 1);
                        
                        pos1 = pos2 + 2;
                        pos2 = line.find("|", pos1);
                        string qtyStr = line.substr(pos1, pos2 - pos1);
                        qtyStr.erase(0, qtyStr.find_first_not_of(" "));
                        qtyStr.erase(qtyStr.find_last_not_of(" ") + 1);
                        
                        pos1 = pos2 + 2;
                        pos2 = line.find("|", pos1);
                        string priceStr = line.substr(pos1, pos2 - pos1);
                        priceStr.erase(0, priceStr.find_first_not_of(" "));
                        priceStr.erase(priceStr.find_last_not_of(" ") + 1);
                        
                        pos1 = pos2 + 2;
                        pos2 = line.find("|", pos1);
                        string totalStr = line.substr(pos1, pos2 - pos1);
                        totalStr.erase(0, totalStr.find_first_not_of(" "));
                        totalStr.erase(totalStr.find_last_not_of(" ") + 1);
                        
                        if (!itemName.empty() && !qtyStr.empty() && !priceStr.empty()) {
                            ProductSummary ps;
                            ps.setBillNum(currentBillNum);
                            ps.setDateTime(currentDateTime);
                            ps.setProductName(itemName);
                            ps.setQuantity(stoi(qtyStr));
                            ps.setRate(stof(priceStr));
                            ps.setSubtotal(stof(totalStr));
                            products.push_back(ps);
                        }
                    } catch (...) {
                        // Skip malformed lines
                        continue;
                    }
                }
            }
        }
        file.close();
        
        return products;
    }

    // Display product sales report
    void displayProductSales(int days) {
        vector<ProductSummary> products = getProductSalesData(days);
        
        if (products.empty()) {
            cout << "\nNo product sales data found!\n";
            return;
        }
        
        cout << "\n _________________________________________________________________________________________\n";
        cout << "|                                   PRODUCT SALES REPORT                                  |\n";
        cout << "|=========================================================================================|\n";
        cout << "| Period: Last " << left << setw(73) << (to_string(days) + " days") << "  |\n";
        cout << "|-----------------------------------------------------------------------------------------|\n";
        cout << "| Bill No  | Date & Time              | Product Name       | Qty  | Rate     | Subtotal   |\n";
        cout << "|----------|--------------------------|--------------------|------|----------|------------|\n";

        float totalAmount = 0.0;
        int totalQuantity = 0;

    for (size_t i = 0; i < products.size(); i++) {
    string dateTime = products[i].getDateTime();
    // Remove newline if present
        if (!dateTime.empty() && dateTime.back() == '\n') {
            dateTime.pop_back();
        }
    
    cout << "| " << left << setw(8) << products[i].getBillNum()
         << " | " << setw(22) << dateTime.substr(0, 24)  // Changed to 24 to get full timestamp
         << " | " << setw(18) << products[i].getProductName().substr(0, 18)
         << " | " << right << setw(4) << products[i].getQuantity()
         << " | " << setw(8) << fixed << setprecision(2) << products[i].getRate()
         << " | " << setw(10) << products[i].getSubtotal() << " |\n";
    
    totalAmount += products[i].getSubtotal();
    totalQuantity += products[i].getQuantity();
}

    cout << "|-----------------------------------------------------------------------------------------|\n";
    cout << "| Total Products Sold: " << left << setw(65) << totalQuantity << "  |\n";

    ostringstream amountStream;
    amountStream << fixed << setprecision(2) << totalAmount << " Rs";
    cout << "| Total Sales Amount : " << left << setw(65) << amountStream.str() << "  |\n";
    cout << "|_________________________________________________________________________________________|\n";
    }
    
public:
    // Create a new bill with user input
    void createBill() {
        Bill bill;
        bill.inputBillDetails();
        bill.printToConsole();
        bill.saveToFile();
    }

    // Check and view existing bills by phone number
    void checkBill() {
        string phone = getValidatedPhone();

        vector<BillSummary> bills = getBillsByPhone(phone);
        displayBillList(bills);

        if (bills.empty()) return;

        string billNum;
        cout << "\nEnter Bill Number to view: ";
        getline(cin, billNum);

        printBillByNumber(billNum);
    }

    // Update an existing bill by removing old one and creating new
    void updateBill() {
        string phone = getValidatedPhone();

        vector<BillSummary> bills = getBillsByPhone(phone);
        displayBillList(bills);

        if (bills.empty()) return;

        string billNum;
        cout << "\nEnter Bill Number to update: ";
        getline(cin, billNum);

        if (removeBillByNumber(billNum)) {
            cout << "\nOld bill removed. Now enter new bill details:\n\n";
            createBill();
        } else {
            cout << "Bill not found!\n";
        }
    }

    // Delete a bill permanently from the system
    void deleteBill() {
        string phone = getValidatedPhone();

        vector<BillSummary> bills = getBillsByPhone(phone);
        displayBillList(bills);

        if (bills.empty()) return;

        string billNum;
        cout << "\nEnter Bill Number to delete: ";
        getline(cin, billNum);

        if (removeBillByNumber(billNum)) {
            cout << "Bill Deleted Successfully!\n";
        } else {
            cout << "Bill not found!\n";
        }
    }
    
    // Check product sales
    void checkProductSales() {
        int days = getValidatedInt("Enter number of days to check product sales: ", 365);
        displayProductSales(days);
    }
};
// ======================== LOGIN LOGGER CLASS ========================
class LoginLogger {
private:
    string operatorName;
    string operatorPhone;
    string loginTime;

    string getCurrentDateTime() {
        time_t now = time(0);
        string dt = ctime(&now);
        if (!dt.empty()) {
            dt.pop_back();
        }
        return dt;
    }

public:
    void recordLogin() {
        cout << " ______________________________________________________\n";
        cout << "|                   OPERATOR LOGIN                     |\n";
        cout << "|======================================================|\n";
        
        operatorName = getValidatedName("Enter Your Name: ");
        operatorPhone = getValidatedPhone();
        loginTime = getCurrentDateTime();

        // Print login details to console
        cout << "\n ______________________________________________________\n";
        cout << "|                 LOGIN SUCCESSFUL                     |\n";
        cout << "|======================================================|\n";
        cout << "| Operator Name : " << left << setw(35) << operatorName << "  |\n";
        cout << "| Phone Number  : " << left << setw(35) << operatorPhone << "  |\n";
        cout << "| Login Time    : " << left << setw(35) << loginTime << "  |\n";
        cout << "|______________________________________________________|\n\n";

        // Save to login.txt
        ofstream file("login.txt", ios::app);
        file << " ______________________________________________________\n";
        file << "|                    LOGIN RECORD                      |\n";
        file << "|======================================================|\n";
        file << "| Operator Name : " << left << setw(35) << operatorName << "  |\n";
        file << "| Phone Number  : " << left << setw(35) << operatorPhone << "  |\n";
        file << "| Login Time    : " << left << setw(35) << loginTime << "  |\n";
        file << "|______________________________________________________|\n\n";
        file.close();
    }

    // Check login history for a specific phone number
    void checkLoginHistory() {
        string phone = getValidatedPhone();
        int days = getValidatedInt("Enter number of days to check history: ", 365);

        ifstream file("login.txt");
        if (!file) {
            cout << "\nNo login history found!\n";
            return;
        }

        vector<LoginRecord> records;
        string line;
        vector<string> currentRecord;
        bool inRecord = false;

        while (getline(file, line)) {
            if (line.find("______________________________________________________") != string::npos) {
                if (inRecord && !currentRecord.empty()) {
                    LoginRecord lr;
                    for (size_t i = 0; i < currentRecord.size(); i++) {
                        if (currentRecord[i].find("Operator Name : ") != string::npos) {
                            string name = currentRecord[i].substr(18, 35);
                            name.erase(name.find_last_not_of(" |") + 1);
                            lr.setOperatorName(name);
                        }
                        if (currentRecord[i].find("Phone Number  : ") != string::npos) {
                            string ph = currentRecord[i].substr(18, 35);
                            ph.erase(ph.find_last_not_of(" |") + 1);
                            lr.setOperatorPhone(ph);
                        }
                        if (currentRecord[i].find("Login Time    : ") != string::npos) {
                            string lt = currentRecord[i].substr(18, 35);
                            lt.erase(lt.find_last_not_of(" |") + 1);
                            lr.setLoginTime(lt);
                        }
                    }

                    // Check if phone matches
                    if (lr.getOperatorPhone() == phone) {
                        records.push_back(lr);
                    }
                }
                currentRecord.clear();
                inRecord = true;
            }
            if (inRecord) {
                currentRecord.push_back(line);
            }
        }
        file.close();    

        // Display results
        if (records.empty()) {
            cout << "\n ______________________________________________________\n";
            cout << "|                   LOGIN HISTORY                      |\n";
            cout << "|======================================================|\n";
            cout << "| No login records found for phone: " << left << setw(16) << phone << "   |\n";
            cout << "|______________________________________________________|\n";
        } else {
            cout << "\n ________________________________________________________\n";
            cout << "|                      LOGIN HISTORY                     |\n";
            cout << "|========================================================|\n";
            cout << "| Phone Number  : " << left << setw(35) << phone << "    |\n";
            cout << "| Period        : Last " << left << setw(26) << (to_string(days) + " days") << "        |\n";
            cout << "| Total Logins  : ";
            string loginText = to_string(records.size()) + (records.size() == 1 ? " time" : " times");
            cout << left << setw(35) << loginText << "    |\n";
            cout << "|--------------------------------------------------------|\n";
            cout << "| S.No | Operator Name        | Login Time               |\n";
            cout << "|------|----------------------|--------------------------|\n";

    for (size_t i = 0; i < records.size(); ++i) {
        string loginTime = records[i].getLoginTime();
        // Remove the newline character if present
        if (!loginTime.empty() && loginTime.back() == '\n') {
            loginTime.pop_back();
        }
    
        cout << "| " << left << setw(4) << (i+1)
         << " | " << setw(20) << records[i].getOperatorName().substr(0, 20)
         << " | " << setw(22) << loginTime.substr(0, 24) << " |\n";
}

        cout << "|________________________________________________________|\n";
        }
    }
};

// ======================== AUTHENTICATION CLASS ========================
class Authentication {
private:
    string password;
    int failedAttempts;
    int lockoutLevel;

    // Load password from Password.dat file
    bool loadPasswordFromFile() {
        ifstream inFile("Password.dat");
        if (!inFile.is_open()) {
            cout << "\n[ERROR] Password.dat file not found!" << endl;
            cout << "[INFO] Using default password for first time setup." << endl;
            password = "admin123";  // Default password
            return true;
        }
        
        string encryptedPass;
        getline(inFile, encryptedPass);
        inFile.close();
        
        if (encryptedPass.empty()) {
            cout << "\n[ERROR] Password.dat is empty!" << endl;
            password = "admin123";  // Default password
            return true;
        }
        
        // Decrypt password using global securityEngine function
        password = securityEngine(encryptedPass);
        return true;
    }

    // Cross-platform delay function
    void crossPlatformSleep(int milliseconds) {
        #ifdef _WIN32
            Sleep(milliseconds);  // Windows
        #else
            usleep(milliseconds * 1000);  // Linux/Unix (microseconds)
        #endif
    }

    // Simple delay function with timer
    void simpleDelay(int seconds) {
        int initial_mins = seconds / 60;
        int min_width = std::to_string(initial_mins).length();

        cout << "\n ______________________________________________________\n";
        cout << "|                   SYSTEM LOCKED!                     |\n";
        cout << "|======================================================|\n";
        cout << "| Too many failed attempts.                            |\n";
        cout << "| Please wait " << seconds/60 << " minute(s) to try again.                |\n";
        cout << "|______________________________________________________|\n\n";
        
        for (int i = seconds; i > 0; i--) {
            int mins = i / 60;
            int secs = i % 60;
            cout << "\rTime Remaining: " << setfill('0') << setw(min_width) << mins 
                 << ":" << setw(2) << secs << "   " << flush;
            
            crossPlatformSleep(1000);  // 1 second
        }
        
        cout << "\rYou can now try again!                    \n\n";
    }

public:
    Authentication() : password(""), failedAttempts(0), lockoutLevel(0) {}

    bool authenticate() {
        // Load password from file
        if (!loadPasswordFromFile()) {
            return false;
        }
        
        const int MAX_ATTEMPTS = 3;
        
        while (true) {
            int remainingAttempts = MAX_ATTEMPTS - (failedAttempts % MAX_ATTEMPTS);
            
            cout << " ______________________________________________________\n";
            cout << "|                   PASSWORD REQUIRED                  |\n";
            cout << "|======================================================|\n";
            cout << "| Remaining Attempts: " << remainingAttempts << " / " << MAX_ATTEMPTS << "                            |\n";
            cout << "|______________________________________________________|\n";
            
            cout << "\nEnter Password (or type 'FORGOT' to recover): ";
            string inputPassword = getHiddenPassword();

            // Check if user wants to recover password
            string upperInput = inputPassword;
            for(int i = 0; i < upperInput.length(); i++) {
                upperInput[i] = toupper(upperInput[i]);
            }
            
            if (upperInput == "FORGOT") {
                cout << "\n[INFO] Initiating password recovery...\n" << endl;
                if (recoverPassword()) {
                    cout << "\nPassword reset successful! Please restart the program.\n" << endl;
                    return false;  // Exit to restart
                } else {
                    cout << "\n[INFO] Returning to login screen...\n" << endl;
                    continue;
                }
            }

            if (inputPassword == password) {
                cout << "\nAuthentication Successful!\n\n";
                failedAttempts = 0;
                lockoutLevel = 0;
                return true;
            } else {
                failedAttempts++;
                remainingAttempts--;
                
                if (remainingAttempts > 0) {
                    cout << "\nWrong Password! " << remainingAttempts << " attempt(s) remaining.\n\n";
                } else {
                    lockoutLevel++;
                    
                    if (lockoutLevel == 1) {
                        simpleDelay(120);  // 2 minutes
                    } else if (lockoutLevel == 2) {
                        simpleDelay(300);  // 5 minutes
                    } else if (lockoutLevel == 3) {
                        simpleDelay(600);  // 10 minutes
                    } else {
                        simpleDelay(86400);  // 24 hours
                    }
                    
                    failedAttempts = 0;
                }
            }
        }
    }
};

// ======================== BILLING SYSTEM CLASS ========================
class BillingSystem {
private:
    BillManager billManager;
    Authentication auth;
    LoginLogger loginLogger;

    void displayMenu() {
        cout << " ______________________________________\n";
        cout << "|           BILLING SYSTEM             |\n";
        cout << "|======================================|\n";
        cout << "| 1. New Bill                          |\n";
        cout << "| 2. Check Bill                        |\n";
        cout << "| 3. Update Bill                       |\n";
        cout << "| 4. Delete Bill                       |\n";
        cout << "| 5. Check Operator Login History      |\n";
        cout << "| 6. Product Checking                  |\n";
        cout << "| 7. Password Manager                  |\n";
        cout << "| 8. Exit                              |\n";
        cout << "|______________________________________|\n";
    }

public:
    void run() {
        // First record operator login
        loginLogger.recordLogin();
        
        // Then authenticate
        if (!auth.authenticate()) {
            return;
        }

        int choice;
        do {
            displayMenu();
            choice = getValidatedInt("Enter Choice: ");

            switch (choice) {
                case 1: 
                    billManager.createBill(); 
                    break;
                case 2: 
                    billManager.checkBill(); 
                    break;
                case 3: 
                    billManager.updateBill(); 
                    break;
                case 4: 
                    billManager.deleteBill(); 
                    break;
                case 5:
                    loginLogger.checkLoginHistory();
                    break;
                case 6:
                    billManager.checkProductSales();
                    break;
                case 7:
                    passwordManager();
                    break;
                case 8:
                    cout << "\n _____________________________________________\n";
                    cout << "|             ABOUT THE DEVELOPER             |\n";
                    cout << "|=============================================|\n";
                    cout << "| This Billing System has been designed and   | \n";
                    cout << "| developed by:                               |\n";
                    cout << "|                                             |\n";
                    cout << "| Name          : Satyam Kumar Singh          | \n";
                    cout << "| Date of Birth : 12/03/2006                  |\n";
                    cout << "| From          : Bihar , Bhojpur , Ara       | \n";
                    cout << "|                                             |\n";
                    cout << "|_____________________________________________|\n";
                    cout << "| Thank you for using the Billing System!     |\n";
                    cout << "|_____________________________________________|\n";
                    break;
                default: 
                    cout << "Invalid Choice! Please enter a number between 1 and 8.\n";
            }
            cout << "\n";
        } while (choice != 8);
    }
};

// ------------------ MAIN ------------------
int main() {
    // Check if password file exists, if not, prompt to create one
    if (!isFileExists("Password.dat")) {
        cout << "\n ______________________________________" << endl;
        cout << "| FIRST TIME SETUP - PASSWORD REQUIRED |" << endl;
        cout << "|======================================|" << endl;
        cout << "| No password found. Please create one.|" << endl;
        cout << "|______________________________________|\n"<<endl;
        passwordManager();
        cout << "\nSetup complete! Please restart the program to login.\n" << endl;
        return 0;
    }
    
    BillingSystem system;
    system.run();
    return 0;
}