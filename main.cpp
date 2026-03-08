#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>
#include <cctype>
#include <sstream>
#include <algorithm>

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
        while (file >> quoted(temp.name) >> temp.price >> temp.quantity) {
            stock.push_back(temp);
        }
        file.close();
    }
}

void saveToFile(const vector<Item>& stock) {
    ofstream file(DB_FILE);
    if (file.is_open()) {
        for (const auto& item : stock) {
            file << quoted(item.name) << " " << item.price << " " << item.quantity << "\n";
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
// Functions Blind Blackspace
// ---------------------------------------------------------

string trim(const string& s) {
    size_t first = s.find_first_not_of(' ');
    if (string::npos == first) return "";
    size_t last = s.find_last_not_of(' ');
    return s.substr(first, (last - first + 1));
}

// ---------------------------------------------------------
// Functions Show
// ---------------------------------------------------------

void showStock(const vector<Item>& stock) {

    int total_quantity = 0;
    double total_price = 0;

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
        total_price += item.price * item.quantity;
    }
    // Footer ตาราง (Total)
    cout << "---------------------------------------------------------------------------------------\n";
    cout << left << setw(30) << "Total" << " | \t" 
         << setw(20) << total_price << " | \t" 
         << total_quantity << "\n";
}

// ---------------------------------------------------------
// Functions New Stocks
// ---------------------------------------------------------

void newStocks(vector<Item>& stock) {
    cout << "\nNew stocks : \n";
    cout << "(Format: [\"Name\"] [Price] [Qty]. Use \"\" for names with spaces. Type 'DONE' to finish)\n";

    while (true) {
        string line;
        getline(cin, line);
        
        if (line.empty()) continue;
        if (toLowerCase(line) == "done") break;

        stringstream ss(line);
        string name;
        double price;
        int qty;

        ss >> quoted(name);
        name = trim(name);

        if (!(ss >> price >> qty)) {
            cout << "Invalid input. (Did you forget \"\" for names with spaces?) or Not Format: [\"Name\"] [Price] [Qty].\n";
            continue;
        }

        string extra;
        if (ss >> extra) {
            cout << "Invalid input. (Too many values)\n";
            continue;
        }

        bool isDuplicate = false;
        for (const auto& item : stock) {
            if (toLowerCase(item.name) == toLowerCase(name)) {
                isDuplicate = true;
                break;
            }
        }

        if (isDuplicate) {
            cout << "------  Fail New " << name << " because it is already in stocks ------\n";
        } else {
            if (price < 0.0) price = 0.0;
            if (qty < 0) qty = 0; 
            stock.push_back({name, price, qty});
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
        string line;
        getline(cin, line);
        
        if (line.empty()) continue;
        if (toLowerCase(line) == "done") break;

        stringstream ss(line);
        string name;
        double price_change;
        int qty_change;

        ss >> quoted(name);
        name = trim(name); 

        if (!(ss >> price_change >> qty_change)) {
            cout << "Invalid input. (Did you forget \"\" for names with spaces?) or Not Format: [\"Name\"] [PriceDiff] [QtyDiff].\n";
            continue;
        }

        string extra;
        if (ss >> extra) {
            cout << "Invalid input. (Too many values)\n";
            continue;
        }

        bool found = false;
        
        for (auto& item : stock) {
            if (toLowerCase(item.name) == toLowerCase(name)) {
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
        string line;
        getline(cin, line);
        
        if (line.empty()) continue;
        if (toLowerCase(line) == "done") break;

        stringstream ss(line);
        string delete_term;

        ss >> quoted(delete_term);
        delete_term = trim(delete_term);

        // เช็คว่ามีค่าขยะติดมาไหม (ลืม "")
        string extra;
        if (ss >> extra) {
            cout << "Invalid input. (Did you forget \"\" for names with spaces?)\n";
            continue;
        }

        bool found = false;
        
        for (auto it = stock.begin(); it != stock.end(); ++it) {
            if (toLowerCase(it->name) == toLowerCase(delete_term)) {
                stock.erase(it);
                found = true;
                cout << "------  Delete " << delete_term << " Successful ------\n";
                break; 
            }
        }

        if (!found) {
            cout << "------  Fail Delete " << delete_term << " (Not found in stocks) ------\n";
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

    while (true) {
        string line;
        getline(cin, line);
        
        if (line.empty()) continue;

        stringstream ss(line);
        string search_term;

        ss >> quoted(search_term);
        search_term = trim(search_term);

        if (toLowerCase(search_term) == "done") break;

        bool found = false;

        for (const auto& item : stock) {
            if (toLowerCase(item.name) == toLowerCase(search_term)) {
                cout << "------- Found : "<< item.name << " " 
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
                isRunning = false;
                break;
            default:
                cout << "Invalid Input. Please try again.\n";
                break;
        } 
    }
    return 0;
} // g++ main.cpp -o stock.exe
