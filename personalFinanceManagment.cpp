#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream> // For file handling

using namespace std;

struct Transaction {
    string type; // "Income" or "Expense"
    string category;
    double amount;
};

// Function to save data to a file
void saveToFile(const vector<Transaction>& transactions) {
    ofstream file("transactions.txt");
    if (file.is_open()) {
        for (const auto& t : transactions) {
            file << t.type << "," << t.category << "," << t.amount << "\n";
        }
        file.close();
        cout << "-> Data saved to file successfully!\n";
    } else {
        cout << "-> Error opening file for saving.\n";
    }
}

// Function to load data from a file
void loadFromFile(vector<Transaction>& transactions, double& totalIncome, double& totalExpense) {
    ifstream file("transactions.txt");
    if (file.is_open()) {
        string type, category;
        double amount;
        while (getline(file, type, ',') && getline(file, category, ',') && file >> amount) {
            file.ignore(); // ignore the newline character
            transactions.push_back({type, category, amount});
            if (type == "Income") totalIncome += amount;
            else if (type == "Expense") totalExpense += amount;
        }
        file.close();
    }
}

int main() {
    vector<Transaction> transactions;
    double totalIncome = 0.0;
    double totalExpense = 0.0;
    int choice;

    // Load existing data when program starts
    loadFromFile(transactions, totalIncome, totalExpense);

    cout << "=========================================\n";
    cout << "   WELCOME TO PERSONAL FINANCE TRACKER   \n";
    cout << "=========================================\n";

    while (true) {
        cout << "\n--- MENU ---\n";
        cout << "1. Add Income\n";
        cout << "2. Add Expense\n";
        cout << "3. View All Transactions\n";
        cout << "4. View Financial Summary\n";
        cout << "5. Delete a Transaction\n";
        cout << "6. Search by Category\n";
        cout << "7. Save Data\n";
        cout << "8. Exit\n";
        cout << "Enter your choice (1-8): ";
        cin >> choice;

        if (choice == 1 || choice == 2) {
            Transaction t;
            t.type = (choice == 1) ? "Income" : "Expense";
            
            cout << "Enter " << t.type << " category: ";
            cin >> t.category;
            cout << "Enter amount: ";
            cin >> t.amount;
            
            transactions.push_back(t);
            if (choice == 1) totalIncome += t.amount;
            else totalExpense += t.amount;
            
            cout << "-> " << t.type << " added successfully!\n";
        } 
        else if (choice == 3) {
            cout << "\n--- ALL TRANSACTIONS ---\n";
            if (transactions.empty()) {
                cout << "No transactions recorded yet.\n";
            } else {
                cout << fixed << setprecision(2);
                for (size_t i = 0; i < transactions.size(); i++) {
                    cout << i + 1 << ". [" << transactions[i].type << "] " 
                         << transactions[i].category << ": $" << transactions[i].amount << "\n";
                }
            }
        }
        else if (choice == 4) {
            cout << "\n--- FINANCIAL SUMMARY ---\n";
            cout << fixed << setprecision(2);
            cout << "Total Income:  $" << totalIncome << "\n";
            cout << "Total Expense: $" << totalExpense << "\n";
            cout << "Current Balance: $" << (totalIncome - totalExpense) << "\n";
        } 
        else if (choice == 5) {
            if (transactions.empty()) {
                cout << "No transactions to delete.\n";
                continue;
            }
            cout << "Enter the transaction number to delete (1-" << transactions.size() << "): ";
            int index;
            cin >> index;
            
            if (index > 0 && index <= transactions.size()) {
                // Adjust totals before deleting
                if (transactions[index - 1].type == "Income") totalIncome -= transactions[index - 1].amount;
                else totalExpense -= transactions[index - 1].amount;
                
                transactions.erase(transactions.begin() + index - 1);
                cout << "-> Transaction deleted successfully!\n";
            } else {
                cout << "-> Invalid transaction number.\n";
            }
        }
        else if (choice == 6) {
            cout << "Enter category to search: ";
            string searchCategory;
            cin >> searchCategory;
            
            bool found = false;
            cout << "\n--- SEARCH RESULTS ---\n";
            for (const auto& t : transactions) {
                if (t.category == searchCategory) {
                    cout << "[" << t.type << "] " << t.category << ": $" << t.amount << "\n";
                    found = true;
                }
            }
            if (!found) cout << "No transactions found for that category.\n";
        }
        else if (choice == 7) {
            saveToFile(transactions);
        }
        else if (choice == 8) {
            cout << "Exiting... Have a great day!\n";
            break;
        } 
        else {
            cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}