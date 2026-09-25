# Digital Wallet & Payment Management System

## 1. Project Overview

The Digital Wallet & Payment Management System is a C++ console-based
simulation of a digital payment ecosystem.

The system demonstrates how digital payment technology can be applied
through wallet management, money transfers, merchant payments, bill
payments and transaction records.

This project was developed as the interactive C++ program for the
Innovation Technology Life Cycle assignment.

---

## 2. Technology

- Programming Language: C++
- Standard: C++17
- Development Environment: Visual Studio Code
- Version Control: Git and GitHub

---

## 3. System Users

### Customer

Customers can:

- Register an account
- Login using a PIN
- Check wallet balance
- Top up wallet
- Transfer money
- Make merchant payments
- Pay bills
- View transaction history
- View account information
- Logout

### Merchant

Merchants can:

- Login
- View account information
- View received payments
- View transaction history
- View total sales
- Logout

### Administrator

Administrators can:

- View all users
- Search for users
- View all transactions
- View system statistics
- Deactivate accounts
- Logout

---

## 4. Main Features

### Account Management

The system supports customer and merchant registration with:

- Unique user ID
- Name
- Email
- Four-digit PIN
- Account status

### Digital Wallet

Customers can manage their wallet balance through:

- Top-up
- Money transfer
- Merchant payment
- Bill payment

### Transaction Management

The system records:

- Transaction ID
- Sender
- Receiver
- Transaction type
- Amount
- Description
- Date and time

### Security and Validation

The system includes:

- Four-digit PIN validation
- Maximum three incorrect PIN attempts
- Account status checking
- Email validation
- Amount validation
- Insufficient balance checking
- Self-transfer prevention
- Inactive account prevention
- Invalid menu input handling

---

## 5. Object-Oriented Design

The system uses object-oriented programming concepts.

Main classes include:

- `User`
- `Customer`
- `Merchant`
- `Admin`
- `Transaction`
- `DigitalWalletSystem`

Inheritance is used by the `Customer`, `Merchant` and `Admin`
classes, which inherit from the `User` class.

The system also uses encapsulation through class methods and
protected/private data members.

---

## 6. Demo Accounts

### Customer

User ID:
`U1001`

PIN:
`1111`

Name:
`Demo Customer`

Initial Balance:
`RM500.00`

### Merchant

User ID:
`M1001`

PIN:
`4321`

Name:
`Nexora Cafe`

### Administrator

User ID:
`A1001`

PIN:
`9999`

Name:
`System Administrator`

---

## 7. How to Compile

Open the terminal inside the project directory.

Run:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o DigitalWallet