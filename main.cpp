#include <iostream>
#include <iomanip>
#include <vector>
#include <memory>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <chrono>
#include <ctime>

using namespace std;
bool yesNo(const string& prompt) const {
    while (true) {
        cout << prompt << " (Y/N): ";
        string s; getline(cin, s);
        if (s == "Y" || s == "y") return true;
        if (s == "N" || s == "n") return false;
        cout << "Please enter Y or N.\n";
    }
}

void pause() const {
    cout << "\nPress ENTER to continue...";
    string s;
    getline(cin, s);
}
enum class UserType { Customer, Merchant, Admin };

class Transaction {
    string id, senderId, receiverId, type, description, timestamp;
    double amount;
public:
    Transaction(string id, string sender, string receiver, string type,
                double amount, string description, string timestamp)
        : id(move(id)), senderId(move(sender)), receiverId(move(receiver)),
          type(move(type)), description(move(description)),
          timestamp(move(timestamp)), amount(amount) {}

    const string& getId() const { return id; }
    const string& getSenderId() const { return senderId; }
    const string& getReceiverId() const { return receiverId; }
    const string& getType() const { return type; }
    const string& getDescription() const { return description; }
    double getAmount() const { return amount; }
    const string& getTimestamp() const { return timestamp; }
};

class User {
protected:
    string userId, name, email, pin;
    UserType type;
    double balance;
    bool active;
public:
    User(string id, string name, string email, string pin, UserType type,
         double balance = 0.0)
        : userId(move(id)), name(move(name)), email(move(email)),
          pin(move(pin)), type(type), balance(balance), active(true) {}
    virtual ~User() = default;

    const string& getId() const { return userId; }
    const string& getName() const { return name; }
    const string& getEmail() const { return email; }
    const string& getPin() const { return pin; }
    UserType getType() const { return type; }
    double getBalance() const { return balance; }
    bool isActive() const { return active; }
    void setBalance(double value) { balance = value; }
    void setActive(bool value) { active = value; }

    string typeName() const {
        if (type == UserType::Customer) return "Customer";
        if (type == UserType::Merchant) return "Merchant";
        return "Admin";
    }
};

class Customer : public User {
public:
    Customer(string id, string name, string email, string pin, double balance = 0)
        : User(move(id), move(name), move(email), move(pin), UserType::Customer, balance) {}
};

class Merchant : public User {
public:
    Merchant(string id, string name, string email, string pin, double balance = 0)
        : User(move(id), move(name), move(email), move(pin), UserType::Merchant, balance) {}
};

class Admin : public User {
public:
    Admin(string id, string name, string email, string pin)
        : User(move(id), move(name), move(email), move(pin), UserType::Admin, 0) {}
};

class DigitalWalletSystem {
    vector<unique_ptr<User>> users;
    vector<Transaction> transactions;
    int nextCustomer = 1002, nextMerchant = 1002, nextTransaction = 1001;

    void line(char c = '=', int n = 62) const { cout << string(n, c) << '\n'; }
    void title(const string& text) const {
        cout << '\n'; line(); cout << "           " << text << '\n'; line();
    }

    string now() const {
        time_t t = chrono::system_clock::to_time_t(chrono::system_clock::now());
        tm local{};
#ifdef _WIN32
        localtime_s(&local, &t);
#else
        localtime_r(&t, &local);
#endif
        ostringstream out;
        out << put_time(&local, "%Y-%m-%d %H:%M:%S");
        return out.str();
    }

    string money(double value) const {
        ostringstream out;
        out << fixed << setprecision(2) << value;
        return out.str();
    }

    string nonEmpty(const string& prompt) const {
        while (true) {
            cout << prompt;
            string s; getline(cin, s);
            if (!s.empty()) return s;
            cout << "Input cannot be empty. Please try again.\n";
        }
    }

    int integer(const string& prompt, int min, int max) const {
        while (true) {
            cout << prompt;
            string s; getline(cin, s);
            stringstream ss(s); int value; char extra;
            if (ss >> value && !(ss >> extra) && value >= min && value <= max) return value;
            cout << "Invalid choice. Enter a number from " << min << " to " << max << ".\n";
        }
    }

    double amount(const string& prompt) const {
    while (true) {
        cout << prompt;

        string s;
        getline(cin, s);

        stringstream ss(s);
        double value;
        char extra;

        if (ss >> value &&
            !(ss >> extra) &&
            isfinite(value) &&
            value > 0 &&
            value <= 1000000) {
            return value;
        }

        cout << "Invalid amount. Enter a valid value greater than RM0 "
             << "and no more than RM1,000,000.\n";
    }
}

    bool yesNo(const string& prompt) const {
        while (true) {
            cout << prompt << " (Y/N): ";
            string s; getline(cin, s);
            if (s == "Y" || s == "y") return true;
            if (s == "N" || s == "n") return false;
            cout << "Please enter Y or N.\n";
        }
    }

    bool validPin(const string& pin) const {
        return pin.size() == 4 && all_of(pin.begin(), pin.end(),
            [](unsigned char c) { return isdigit(c); });
    }

    bool validEmail(const string& email) const {
    size_t at = email.find('@');
    size_t dot = email.rfind('.');

    if (at == string::npos ||
        dot == string::npos ||
        at == 0 ||
        dot <= at + 1 ||
        dot >= email.size() - 1) {
        return false;
    }

    if (email.find(' ') != string::npos) {
        return false;
    }

    return true;
}

    User* findUser(const string& id) {
        for (auto& u : users) if (u->getId() == id) return u.get();
        return nullptr;
    }
    const User* findUser(const string& id) const {
        for (const auto& u : users) if (u->getId() == id) return u.get();
        return nullptr;
    }

    bool emailExists(const string& email) const {
        for (const auto& u : users) if (u->getEmail() == email) return true;
        return false;
    }

    void addTransaction(const string& sender, const string& receiver,
                        const string& type, double amount, const string& description) {
        transactions.emplace_back("T" + to_string(nextTransaction++), sender, receiver,
                                  type, amount, description, now());
    }

    void showTransaction(const Transaction& t, const User* current = nullptr) const {
        string value = money(t.getAmount());
        if (current) {
            if (t.getSenderId() == current->getId()) value = "-RM" + value;
            else if (t.getReceiverId() == current->getId()) value = "+RM" + value;
        }
        cout << left << setw(8) << t.getId() << setw(18) << t.getType()
             << setw(30) << t.getDescription() << setw(14) << value
             << t.getTimestamp() << '\n';
    }

    void registerCustomer() {
        title("CUSTOMER REGISTRATION");
        string name = nonEmpty("Enter full name: ");
        string email;
        while (true) {
            email = nonEmpty("Enter email: ");
            if (!validEmail(email)) { cout << "Invalid email format.\n"; continue; }
            if (emailExists(email)) { cout << "Email is already registered.\n"; continue; }
            break;
        }
        string pin;
        while (true) {
            pin = nonEmpty("Create 4-digit PIN: ");
            if (validPin(pin)) break;
            cout << "PIN must contain exactly 4 digits.\n";
        }
        string id = "U" + to_string(nextCustomer++);
        users.push_back(make_unique<Customer>(id, name, email, pin));
        cout << "\nRegistration successful!\nYour Customer ID is: " << id << '\n';
    }

    void registerMerchant() {
        title("MERCHANT REGISTRATION");
        string name = nonEmpty("Enter business/merchant name: ");
        string email;
        while (true) {
            email = nonEmpty("Enter business email: ");
            if (!validEmail(email)) { cout << "Invalid email format.\n"; continue; }
            if (emailExists(email)) { cout << "Email is already registered.\n"; continue; }
            break;
        }
        string pin;
        while (true) {
            pin = nonEmpty("Create 4-digit PIN: ");
            if (validPin(pin)) break;
            cout << "PIN must contain exactly 4 digits.\n";
        }
        string id = "M" + to_string(nextMerchant++);
        users.push_back(make_unique<Merchant>(id, name, email, pin));
        cout << "\nMerchant registration successful!\nYour Merchant ID is: " << id << '\n';
    }

    void registrationMenu() {
        title("REGISTER ACCOUNT");
        cout << "1. Customer\n2. Merchant\n3. Back\n";
        int c = integer("Enter your choice: ", 1, 3);
        if (c == 1) registerCustomer();
        else if (c == 2) registerMerchant();
    }

    User* login() {
        title("LOGIN");
        string id = nonEmpty("Enter User ID: ");
        User* user = findUser(id);
        if (!user) { cout << "User account not found.\n"; return nullptr; }
        if (!user->isActive()) { cout << "This account has been deactivated.\n"; return nullptr; }

        for (int attempt = 1; attempt <= 3; ++attempt) {
            string pin = nonEmpty("Enter 4-digit PIN: ");
            if (pin == user->getPin()) {
                cout << "\nLogin successful. Welcome, " << user->getName() << "!\n";
                return user;
            }
            cout << "Incorrect PIN. Attempts remaining: " << 3 - attempt << '\n';
        }
        cout << "Too many incorrect attempts. Login blocked.\n";
        return nullptr;
    }

    void checkBalance(User* u) const {
        title("WALLET BALANCE");
        cout << "Current Balance: RM" << money(u->getBalance()) << '\n';
    }

    void topUp(User* u) {
        title("TOP UP WALLET");
        double value = amount("Enter top-up amount: RM");
        if (!yesNo("Confirm top-up of RM" + money(value))) { cout << "Top-up cancelled.\n"; return; }
        u->setBalance(u->getBalance() + value);
        addTransaction("BANK", u->getId(), "TOP UP", value, "Wallet top-up");
        cout << "\nTop-up successful! New Balance: RM" << money(u->getBalance()) << '\n';
    }

    void transfer(User* sender) {
        title("TRANSFER MONEY");
        string id = nonEmpty("Enter recipient User ID: ");
        User* receiver = findUser(id);
        if (!receiver) { cout << "Recipient account not found.\n"; return; }
        if (!receiver->isActive()) { cout << "Recipient account is inactive.\n"; return; }
        if (receiver == sender) { cout << "You cannot transfer money to yourself.\n"; return; }
        if (receiver->getType() == UserType::Admin) { cout << "Transfers to admin accounts are not allowed.\n"; return; }
        cout << "Recipient: " << receiver->getName() << '\n';
        double value = amount("Enter transfer amount: RM");
        if (value > sender->getBalance()) { cout << "Insufficient balance. Transfer cancelled.\n"; return; }
        if (!yesNo("Confirm transfer of RM" + money(value) + " to " + receiver->getName())) { cout << "Transfer cancelled.\n"; return; }
        sender->setBalance(sender->getBalance() - value);
        receiver->setBalance(receiver->getBalance() + value);
        addTransaction(sender->getId(), receiver->getId(), "TRANSFER", value, "Transfer to " + receiver->getName());
        cout << "Transfer successful! Remaining Balance: RM" << money(sender->getBalance()) << '\n';
    }

    void payment(User* customer) {
        title("MAKE PAYMENT");
        string id = nonEmpty("Enter Merchant ID: ");
        User* merchant = findUser(id);
        if (!merchant || merchant->getType() != UserType::Merchant) { cout << "Merchant account not found.\n"; return; }
        if (!merchant->isActive()) { cout << "Merchant account is inactive.\n"; return; }
        cout << "Merchant: " << merchant->getName() << '\n';
        double value = amount("Enter payment amount: RM");
        if (value > customer->getBalance()) { cout << "Insufficient balance. Payment cancelled.\n"; return; }
        if (!yesNo("Confirm payment of RM" + money(value) + " to " + merchant->getName())) { cout << "Payment cancelled.\n"; return; }
        customer->setBalance(customer->getBalance() - value);
        merchant->setBalance(merchant->getBalance() + value);
        addTransaction(customer->getId(), merchant->getId(), "PAYMENT", value, "Payment to " + merchant->getName());
        cout << "Payment successful! New Balance: RM" << money(customer->getBalance()) << '\n';
    }

    void bills(User* customer) {
        title("PAY BILLS");
        cout << "1. Electricity\n2. Water\n3. Internet\n4. Mobile Phone\n5. Back\n";
        int c = integer("Enter your choice: ", 1, 5);
        if (c == 5) return;
        string bill = (c == 1 ? "Electricity" : c == 2 ? "Water" : c == 3 ? "Internet" : "Mobile Phone");
        string account = nonEmpty("Enter bill account number: ");
        double value = amount("Enter bill amount: RM");
        if (value > customer->getBalance()) { cout << "Insufficient balance. Bill payment cancelled.\n"; return; }
        if (!yesNo("Confirm " + bill + " bill payment of RM" + money(value))) { cout << "Bill payment cancelled.\n"; return; }
        customer->setBalance(customer->getBalance() - value);
        addTransaction(customer->getId(), "BILLING", "BILL PAYMENT", value, bill + " bill (" + account + ")");
        cout << "Bill payment successful! New Balance: RM" << money(customer->getBalance()) << '\n';
    }

    void history(User* u) const {
    title("TRANSACTION HISTORY");

    bool found = false;
    int transactionCount = 0;
    double incoming = 0.0;
    double outgoing = 0.0;

    cout << left
         << setw(8) << "ID"
         << setw(18) << "TYPE"
         << setw(30) << "DESCRIPTION"
         << setw(14) << "AMOUNT"
         << "DATE/TIME\n";

    line('-', 105);

    for (const auto& t : transactions) {
        if (t.getSenderId() == u->getId() ||
            t.getReceiverId() == u->getId()) {

            showTransaction(t, u);
            found = true;
            ++transactionCount;

            if (t.getReceiverId() == u->getId()) {
                incoming += t.getAmount();
            }

            if (t.getSenderId() == u->getId()) {
                outgoing += t.getAmount();
            }
        }
    }

    if (!found) {
        cout << "No transactions found.\n";
        return;
    }

    line('-', 105);

    cout << "\nTRANSACTION SUMMARY\n";
    cout << "Number of Transactions : " << transactionCount << '\n';
    cout << "Total Incoming         : RM" << money(incoming) << '\n';
    cout << "Total Outgoing         : RM" << money(outgoing) << '\n';
}

    void account(User* u) const {
        title("ACCOUNT INFORMATION");
        cout << "Name       : " << u->getName() << '\n'
             << "User ID    : " << u->getId() << '\n'
             << "Email      : " << u->getEmail() << '\n'
             << "User Type  : " << u->typeName() << '\n'
             << "Balance    : RM" << money(u->getBalance()) << '\n'
             << "Status     : " << (u->isActive() ? "Active" : "Inactive") << '\n';
    }

    void receivedPayments(User* merchant) const {
        title("RECEIVED PAYMENTS");
        double total = 0; bool found = false;
        cout << left << setw(12) << "Customer ID" << setw(22) << "Customer" << setw(16) << "Amount" << "DATE/TIME\n";
        line('-', 75);
        for (const auto& t : transactions) {
            if (t.getReceiverId() == merchant->getId() && t.getType() == "PAYMENT") {
                const User* customer = findUser(t.getSenderId());
                cout << left << setw(12) << t.getSenderId() << setw(22)
                     << (customer ? customer->getName() : "Unknown")
                     << "RM" << setw(14) << money(t.getAmount()) << t.getTimestamp() << '\n';
                total += t.getAmount(); found = true;
            }
        }
        if (!found) cout << "No payments received yet.\n";
        else { line('-', 75); cout << "Total Received: RM" << money(total) << '\n'; }
    }

    void totalSales(User* merchant) const {
        title("TOTAL SALES");
        double total = 0; int count = 0;
        for (const auto& t : transactions)
            if (t.getReceiverId() == merchant->getId() && t.getType() == "PAYMENT") { total += t.getAmount(); ++count; }
        cout << "Number of Payments : " << count << '\n';
        cout << "Total Sales        : RM" << money(total) << '\n';
    }

    void allUsers() const {
        title("ALL USERS");
        cout << left << setw(8) << "ID" << setw(22) << "Name" << setw(14) << "Type" << "Status\n";
        line('-', 60);
        for (const auto& u : users)
            cout << left << setw(8) << u->getId() << setw(22) << u->getName()
                 << setw(14) << u->typeName() << (u->isActive() ? "Active" : "Inactive") << '\n';
    }

    void searchUser() const {
        title("SEARCH USER");
        string id = nonEmpty("Enter User ID: ");
        const User* u = findUser(id);
        if (!u) { cout << "User not found.\n"; return; }
        cout << "Name       : " << u->getName() << '\n'
             << "ID         : " << u->getId() << '\n'
             << "Email      : " << u->getEmail() << '\n'
             << "Type       : " << u->typeName() << '\n'
             << "Balance    : RM" << money(u->getBalance()) << '\n'
             << "Status     : " << (u->isActive() ? "Active" : "Inactive") << '\n';
    }

    void allTransactions() const {
        title("ALL SYSTEM TRANSACTIONS");
        if (transactions.empty()) { cout << "No transactions found.\n"; return; }
        cout << left << setw(8) << "ID" << setw(18) << "TYPE" << setw(30) << "DESCRIPTION"
             << setw(14) << "AMOUNT" << "DATE/TIME\n";
        line('-', 105);
        for (const auto& t : transactions) showTransaction(t);
    }

    void statistics() const {
        title("SYSTEM STATISTICS");
        int customer = 0, merchant = 0, admin = 0, active = 0;
        double topups = 0, payments = 0, transfers = 0;
        for (const auto& u : users) {
            if (u->isActive()) ++active;
            if (u->getType() == UserType::Customer) ++customer;
            else if (u->getType() == UserType::Merchant) ++merchant;
            else ++admin;
        }
        for (const auto& t : transactions) {
            if (t.getType() == "TOP UP") topups += t.getAmount();
            else if (t.getType() == "PAYMENT") payments += t.getAmount();
            else if (t.getType() == "TRANSFER") transfers += t.getAmount();
        }
        cout << "Total Users          : " << users.size() << '\n'
             << "Active Users         : " << active << '\n'
             << "Customer Accounts    : " << customer << '\n'
             << "Merchant Accounts    : " << merchant << '\n'
             << "Admin Accounts       : " << admin << '\n'
             << "Total Transactions   : " << transactions.size() << '\n'
             << "Total Money Top Up   : RM" << money(topups) << '\n'
             << "Total Payments       : RM" << money(payments) << '\n'
             << "Total Transfers      : RM" << money(transfers) << '\n';
    }

    void deactivate() {
        title("DEACTIVATE USER");
        string id = nonEmpty("Enter User ID: ");
        User* u = findUser(id);
        if (!u) { cout << "User not found.\n"; return; }
        if (u->getType() == UserType::Admin) { cout << "Admin accounts cannot be deactivated.\n"; return; }
        if (!u->isActive()) { cout << "Account is already inactive.\n"; return; }
        if (!yesNo("Deactivate " + u->getName() + "'s account")) { cout << "Action cancelled.\n"; return; }
        u->setActive(false);
        cout << "Account successfully deactivated.\n";
    }

    void customerMenu(User* u) {
    while (true) {
        title("CUSTOMER MENU");
        cout << "Welcome, " << u->getName() << "!\n\n"
             << "1. Check Balance\n2. Top Up Wallet\n3. Transfer Money\n"
             << "4. Make Payment\n5. Pay Bills\n6. Transaction History\n"
             << "7. Account Information\n8. Logout\n";

        int c = integer("Enter your choice: ", 1, 8);

        if (c == 1) {
            checkBalance(u);
            pause();
        }
        else if (c == 2) {
            topUp(u);
            pause();
        }
        else if (c == 3) {
            transfer(u);
            pause();
        }
        else if (c == 4) {
            payment(u);
            pause();
        }
        else if (c == 5) {
            bills(u);
            pause();
        }
        else if (c == 6) {
            history(u);
            pause();
        }
        else if (c == 7) {
            account(u);
            pause();
        }
        else {
            cout << "Logging out...\n";
            return;
        }
    }
}

    void merchantMenu(User* u) {
        while (true) {
            title("MERCHANT MENU");
            cout << "Welcome, " << u->getName() << "!\n\n"
                 << "1. Account Information\n2. View Received Payments\n"
                 << "3. Transaction History\n4. View Total Sales\n5. Logout\n";
            int c = integer("Enter your choice: ", 1, 5);
            if (c == 1) account(u);
            else if (c == 2) receivedPayments(u);
            else if (c == 3) history(u);
            else if (c == 4) totalSales(u);
            else { cout << "Logging out...\n"; return; }
        }
    }

    void adminMenu(User* u) {
    while (true) {
        title("ADMIN MENU");
        cout << "Welcome, " << u->getName() << "!\n\n"
             << "1. View All Users\n2. Search User\n3. View All Transactions\n"
             << "4. System Statistics\n5. Deactivate User\n6. Logout\n";

        int c = integer("Enter your choice: ", 1, 6);

        if (c == 1) {
            allUsers();
            pause();
        }
        else if (c == 2) {
            searchUser();
            pause();
        }
        else if (c == 3) {
            allTransactions();
            pause();
        }
        else if (c == 4) {
            statistics();
            pause();
        }
        else if (c == 5) {
            deactivate();
            pause();
        }
        else {
            cout << "Logging out...\n";
            return;
        }
    }
}

    void route(User* u) {
        if (u->getType() == UserType::Customer) customerMenu(u);
        else if (u->getType() == UserType::Merchant) merchantMenu(u);
        else adminMenu(u);
    }

    void seedData() {
        users.push_back(make_unique<Admin>("A1001", "System Administrator", "admin@digitalwallet.com", "9999"));
        users.push_back(make_unique<Merchant>("M1001", "Nexora Cafe", "merchant@nexora.com", "4321"));
        users.push_back(make_unique<Customer>("U1001", "Demo Customer", "demo@digitalwallet.com", "1111", 500.00));
    }

public:
    DigitalWalletSystem() { seedData(); }

    void run() {
        while (true) {
            title("DIGITAL WALLET & PAYMENT MANAGEMENT SYSTEM");
            cout << "1. Register Account\n2. Login\n3. Exit\n";
            int c = integer("Enter your choice: ", 1, 3);
            if (c == 1) registrationMenu();
            else if (c == 2) {
                User* u = login();
                if (u) route(u);
            } else {
                cout << "\nThank you for using the Digital Wallet & Payment Management System.\nGoodbye!\n";
                break;
            }
        }
    }
};

int main() {
    DigitalWalletSystem system;
    system.run();
    return 0;
}
