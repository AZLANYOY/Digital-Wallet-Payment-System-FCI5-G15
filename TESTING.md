# Digital Wallet & Payment Management System
## Testing Documentation

### 1. Introduction

The Digital Wallet & Payment Management System was tested to verify
that the system performs the required functions correctly and handles
invalid user input safely.

Testing covered customer, merchant and administrator functions.

---

## 2. Test Cases

| No. | Test Scenario | Expected Result | Status |
|-----|---------------|-----------------|--------|
| 1 | Customer login with correct PIN | Login successful | PASS |
| 2 | Login with incorrect PIN | Error message displayed | PASS |
| 3 | Three incorrect PIN attempts | Login blocked | PASS |
| 4 | Check wallet balance | Correct balance displayed | PASS |
| 5 | Valid wallet top-up | Balance increases | PASS |
| 6 | Negative top-up amount | Input rejected | PASS |
| 7 | Transfer to valid user | Money transferred successfully | PASS |
| 8 | Transfer to invalid user | Transaction rejected | PASS |
| 9 | Transfer with insufficient balance | Transaction rejected | PASS |
| 10 | Transfer to self | Transaction rejected | PASS |
| 11 | Payment to valid merchant | Payment successful | PASS |
| 12 | Payment with insufficient balance | Payment rejected | PASS |
| 13 | Bill payment | Bill transaction recorded | PASS |
| 14 | Transaction history | Transactions displayed | PASS |
| 15 | Merchant received payments | Payments displayed | PASS |
| 16 | Merchant total sales | Correct total displayed | PASS |
| 17 | Admin view all users | User list displayed | PASS |
| 18 | Admin search user | Correct user displayed | PASS |
| 19 | Admin view transactions | Transactions displayed | PASS |
| 20 | Admin system statistics | Statistics displayed | PASS |
| 21 | Admin deactivates user | User becomes inactive | PASS |
| 22 | Deactivated user login | Login rejected | PASS |
| 23 | Invalid menu option | Error message displayed | PASS |
| 24 | Empty input | Input rejected | PASS |
| 25 | Invalid email format | Registration rejected | PASS |

---

## 3. Security Testing

The system includes basic security controls for the simulation:

- Four-digit PIN authentication
- Maximum three incorrect PIN attempts
- Active/inactive account checking
- Validation of transaction amounts
- Prevention of self-transfer
- Prevention of transactions to inactive accounts
- Prevention of transactions when the balance is insufficient

---

## 4. Conclusion

The testing results show that the main customer, merchant and
administrator functions operate as expected.

The system also handles common invalid input and transaction
conditions without terminating unexpectedly.