#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>
#include <cctype>
#include <sstream>

using namespace std;

struct Item {
    string name;
    double price;
    int quantity;
};

const string DB_FILE = "stock_data.txt";

// ---------------------------------------------------------
// Functions File I/O
// ---------------------------------------------------------

void loadFromFile(vector<Item>& stock) {
    stock.clear();
    ifstream file(DB_FILE);
    if (file.is_open()) {
        Item temp;
        // โหลดข้อมูลเก่าขึ้นมาตอนเปิดโปรแกรม
        while (file >> temp.name >> temp.price >> temp.quantity) {
            stock.push_back(temp);
        }
        file.close();
    }
}

void saveToFile(const vector<Item>& stock) {
    ofstream file(DB_FILE);
    if (file.is_open()) {
        for (const auto& item : stock) {
            file << item.name << " " << item.price << " " << item.quantity << "\n";
        }
        file.close();
    }
}

// ---------------------------------------------------------
// Functions Convert String
// ---------------------------------------------------------

string toLowerCase(string str) {
    for (int i = 0; i < str.length(); i++) {
        str[i] = tolower(str[i]);
    }
    return str;
}

// ---------------------------------------------------------
// Functions Show
// ---------------------------------------------------------

void showStock(const vector<Item>& stock) {

    int total_quantity = 0;
    double total_pirce = 0;

    cout << R"(
      ___           ___           ___           ___           ___           ___     
     /\  \         /\  \         /\  \         /\  \         /\__\         /\  \    
    /::\  \        \:\  \       /::\  \       /::\  \       /:/  /        /::\  \   
   /:/\ \  \        \:\  \     /:/\:\  \     /:/\:\  \     /:/__/        /:/\ \  \  
  _\:\~\ \  \       /::\  \   /:/  \:\  \   /:/  \:\  \   /::\__\____   _\:\~\ \  \ 
 /\ \:\ \ \__\     /:/\:\__\ /:/__/ \:\__\ /:/__/ \:\__\ /:/\:::::\__\ /\ \:\ \ \__\
 \:\ \:\ \/__/    /:/  \/__/ \:\  \ /:/  / \:\  \  \/__/ \/_|:|~~|~    \:\ \:\ \/__/
  \:\ \:\__\     /:/  /       \:\  /:/  /   \:\  \          |:|  |      \:\ \:\__\  
   \:\/:/  /     \/__/         \:\/:/  /     \:\  \         |:|  |       \:\/:/  /  
    \::/  /                     \::/  /       \:\__\        |:|  |        \::/  /   
     \/__/                       \/__/         \/__/         \|__|         \/__/    
    )" << '\n';
    cout << "=======================================================================================\n"; 

    // Header ตาราง
    cout << left << setw(30) << "NAME" << " | \t" 
         << setw(20) << "PRICE" << " | \t" 
         << "QUANTITY\n";
        
    // รายการสินค้าทั้งหมด
    for (const auto& item : stock) {
        cout << left << setw(30) << item.name << " | \t" 
             << fixed << setprecision(1) << setw(20) << item.price << " | \t" 
             << item.quantity << "\n";
        
        total_quantity += item.quantity;
        total_pirce += item.price * item.quantity;
    }
    // Footer ตาราง (Total)
    cout << "---------------------------------------------------------------------------------------\n";
    cout << left << setw(30) << "Total" << " | \t" 
         << setw(20) << total_pirce << " | \t" 
         << total_quantity << "\n";
}

// ---------------------------------------------------------
// Functions New Stocks
// ---------------------------------------------------------

void newStocks(vector<Item>& stock) {
    cout << "\nNew stocks : \n";
    cout << "(Format: [\"Name\"] [Price] [Qty]. Use \"\" for names with spaces. Type 'DONE' to finish)\n";

    cin.ignore(10000, '\n'); 
    
    while (true) {
        string line;
        getline(cin, line);
        
        if (line.empty()) continue;

        stringstream ss(line);
        string name;

        ss >> quoted(name);
        
        if (toLowerCase(name) == "done") break;

        double price;
        int qty;

        if (!(ss >> price >> qty)) {
            cout << "Invalid input. (Did you forget \"\" for names with spaces?)\n";
            continue;
        }

        string extra;
        if (ss >> extra) {
            cout << "Invalid input. (Too many values)\n";
            continue;
        }

        string input_name_lower = toLowerCase(name); 
        bool isDuplicate = false;
        for (const auto& item : stock) {
            if (toLowerCase(item.name) == input_name_lower) {
                isDuplicate = true;
                break;
            }
        }

        if (isDuplicate) {
            cout << "------  Fail New " << name << " because it is already in stocks ------\n";
        } else {
            if (price < 0.0) price = 0.0;
            if (qty < 0) qty = 0; 
            stock.push_back({input_name_lower, price, qty});
            cout << "------  New " << name << " Successful  ------\n";
        }
    }
    saveToFile(stock); 
}

// ---------------------------------------------------------
// Functions Update Stocks
// ---------------------------------------------------------

void updateStocks(vector<Item>& stock) {
    cout << "\nUpdate stocks : \n";
    cout << "(Format: [\"Name\"] [PriceDiff] [QtyDiff]. Use \"\" for names with spaces. Type 'DONE' to finish)\n";
    
    while (true) {
        string name;
        cin >> quoted(name);
        
        if (toLowerCase(name) == "done") {
            break;
        }

        double price_change;
        int qty_change;
        cin >> price_change >> qty_change;

        string input_name_lower = toLowerCase(name); 
        bool found = false;
        
        for (auto& item : stock) {
            if (toLowerCase(item.name) == input_name_lower) {
                found = true;
                item.price += price_change;
                item.quantity += qty_change;
                
                if (item.price < 0.0) item.price = 0.0;
                if (item.quantity < 0) item.quantity = 0;
                
                cout << "------  Update " << name << " Successful ------\n";
                break;
            }
        }

        if (!found) {
            cout << "------  Fail Update " << name << " (Not found in stocks) ------\n";
        }
    }
    saveToFile(stock);
}

// ---------------------------------------------------------
// Functions Delete Stocks
// ---------------------------------------------------------

void deleteStocks(vector<Item>& stock) {
    cout << "\nDelete stocks : \n";
    cout << "(Format: [\"Name\"]. Use \"\" for names with spaces. Type 'DONE' to finish)\n";
    
    while (true) {
        string name;
        cin >> quoted(name);
        
        if (toLowerCase(name) == "done") {
            break;
        }

        string input_name_lower = toLowerCase(name); 
        bool found = false;
        
        for (auto it = stock.begin(); it != stock.end(); ++it) {
            if (toLowerCase(it->name) == input_name_lower) {
                stock.erase(it);
                found = true;
                cout << "------  Delete " << name << " Successful ------\n";
                break; 
            }
        }

        if (!found) {
            cout << "------  Fail Delete " << name << " (Not found in stocks) ------\n";
        }
    }
    saveToFile(stock);
}

// ---------------------------------------------------------
// Functions Search
// ---------------------------------------------------------

void searchStocks(const vector<Item>& stock) {
    cout << "\nSearch stocks : \n";
    cout << "(Enter name to search. Use \"\" for names with spaces. Type 'DONE' to finish)\n";
    
    cin.ignore(10000, '\n');

    while (true) {
        string line;
        getline(cin, line);
        
        if (line.empty()) continue;

        stringstream ss(line);
        string search_term;
        
        ss >> quoted(search_term);
        
        if (toLowerCase(search_term) == "done") break;

        string extra;
        if (ss >> extra) {
            cout << "Invalid input.\n";
            continue;
        }

        string search_lower = toLowerCase(search_term);
        bool found = false;

        for (const auto& item : stock) {
            if (toLowerCase(item.name) == search_lower) {
                cout << "------- Found :"<< item.name << " " 
                     << fixed << setprecision(1) << item.price << " " 
                     << item.quantity << " -------\n";
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "Not Found : " << search_term << "\n";
        }
    }
}

// ---------------------------------------------------------
// Functions Export (CSV)
// ---------------------------------------------------------

void exportCSV(const vector<Item>& stock) {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    
    string filename = "Excel/stock_report_" 
                    + to_string(1900 + ltm->tm_year) 
                    + to_string(1 + ltm->tm_mon) 
                    + to_string(ltm->tm_mday) + "_" 
                    + to_string(ltm->tm_hour) 
                    + to_string(ltm->tm_min) 
                    + to_string(ltm->tm_sec) + ".csv";

    ofstream file(filename);
    if (file.is_open()) {
        file << "Name,price,quantity\n";
        for (const auto& item : stock) {
            file << item.name << "," 
                 << item.price << "," 
                 << item.quantity << "\n";
        }
        file.close();
        cout << "\n[Success] Exported to Excel file: " << filename << "\n";
    } else {
        cout << "\n[Error] Cannot create export file. Please check if 'Export' folder exists.\n";
    }
}

// ---------------------------------------------------------
// Main()
// ---------------------------------------------------------

int main() {
    vector<Item> stock;
    loadFromFile(stock); 
    
    char choice;
    bool isRunning = true;

    while (isRunning) {
        showStock(stock); 
        
        cout << "\nChoose Function : New (N) , Update (U) , Delete (D) , Show (S) , Find (F) , Export (E) , Quit (Q) : ";
        cin >> choice;
        choice = toupper(choice);

        switch (choice) {
            case 'N':
                newStocks(stock);
                break;
            case 'U':
                updateStocks(stock);
                break;
            case 'D':
                deleteStocks(stock);
                break;
            case 'S':
                cout << "\nRefreshing display...\n";
                break;
            case 'F':
                searchStocks(stock);
                break;
            case 'E':
                exportCSV(stock);
                break;
            case 'Q':
                cout << "Exiting program...\n";
                isRunning = false;
                break;
            default:
                cout << "Invalid Input. Please try again.\n";
                break;
        }
    }
    return 0;
}
