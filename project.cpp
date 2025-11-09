#include<iostream>
#include<queue>
#include<stack>
#include<limits>
#include<algorithm>
#include<vector>
#include<string>
#include<sstream>
#include<fstream>

using namespace std;

struct Stock{
    string name;
    vector<int> prices;
    vector<string>trend;
};

vector<Stock> stocks;

void loadStocks(){
    ifstream myFile("stock.txt");
    if(!myFile.is_open()){
        cout<<"No Stock Data Found!\n";
        return;
    }
    stocks.clear();
    int totalStocks = 0;
    if(!(myFile >> totalStocks)){
        cout<<"Error: Could not read total stock count.\n";
        myFile.close();
        return;
    }
    myFile.ignore(numeric_limits<streamsize>::max(),'\n');
    string line;
    int lineNumber = 0;
    while(getline(myFile, line)){
        lineNumber++;
        if(line.empty()) continue;

        stringstream ss(line);
        Stock s;
        int priceCount = 0;
        ss>>s.name>>priceCount;

        if(s.name.empty() || priceCount <= 0){
            cerr<<"Warning: Skipping invalid data at line "<<lineNumber + 1<<endl;
            continue;
        }

        s.prices.clear();
        int price;
        for(int j = 0; j < priceCount; ++j){
            if(!(ss >> price)){
                if(!(myFile >> price)){
                    cerr<<"Error: Missing price data for "<<s.name<<" (expected "<<priceCount<<" prices, got "<<s.prices.size()<< ")\n";
                    break;
                }
            }
            s.prices.push_back(price);
        }
        if(!s.prices.empty()){
            stocks.push_back(s);
        }
    }
    myFile.close();
    if((int)stocks.size() != totalStocks){
        cout<<"⚠ Warning: File said "<<totalStocks<<" stocks, but loaded "<<stocks.size()<<" successfully.\n";
    }
}

void saveToFile(){
    ofstream myFile("stock.txt");
    if(!myFile.is_open()){
        cout << "Error saving file!" << endl;
        return;
    }
    myFile << stocks.size() << endl;
    for(auto &i : stocks){
        myFile << i.name << " " << i.prices.size();
        for(int p : i.prices){
            myFile << " " << p;
        }
        myFile << " " << i.trend.size();
        for(const string& t : i.trend){
            myFile << " " << t;
        }
        myFile << endl; 
    }
    myFile.close(); 
}

void addStock(){
    cout<<"\n=====================================\n";
    cout<<"          ADD NEW STOCK DATA          \n";
    cout<<"=====================================\n";

    string name;
    int numPrices;
    float price;
    cout<<"\nEnter stock/company name: ";
    cin>>name;

    for(auto &s: stocks){
        if(s.name == name){
            cout<<"Error: Stock '"<<name<<"' already exists!\n";
            return;
        }
    }
    cout<<"Enter number of past days to record prices for: ";
    cin>>numPrices;

    if(numPrices <= 0){
        cout<<"Invalid number of days!\n";
        return;
    }
    Stock newStock;
    newStock.name = name;
    cout<<"Enter "<<numPrices<<" prices separated by space: ";
    for(int i = 0; i < numPrices; i++){
        cin>>price;
        if(price<0){
            cout<<"Invalid price. Price must be positive!\n";
            return;
        }
        newStock.prices.push_back(price);
    }
    stocks.push_back(newStock);
    ofstream myFile("stock.txt");
    if(!myFile.is_open()){
        cout<<"Error: Unable to open file!\n";
        return;
    }
    myFile<<stocks.size()<<endl;

    for(auto &s : stocks){
        myFile<<s.name<<" "<<s.prices.size();
        for(float p : s.prices){
            myFile<<" "<<p;
        }
        myFile<<endl;
    }
    myFile.close();
    cout<<"\n-------------------------------------\n";
    cout<<"Stock '"<<name<<"' added successfully!\n";
    cout<<"-------------------------------------\n";
}

void displayStocks(){
    if(stocks.empty()){
        cout<<"\nNo Stocks Available To Display!\n\n";
        return;
    }
    cout<<"\n=====================================\n";
    cout<<"         LIST OF ALL STOCKS          \n";
    cout<<"=====================================\n";

    for(unsigned int i = 0; i < stocks.size(); i++){
        cout<<i+1<<". "<<stocks[i].name<<endl;
        cout<<"    Total Days Recorded: "<<stocks[i].prices.size()<<endl;
        cout<<"    Prices: ";
        if(stocks[i].prices.empty()){
            cout<<"No price data available";
        }else{
            for(int price : stocks[i].prices){
                cout<<price<<" ";
            }
        }
        cout<<"\n-------------------------------------\n";
    }
    cout<<endl;
}

void stockMenu(Stock &s){
    cout<<"\n=====================================\n";
    cout<<"        STOCK ANALYSIS MENU          \n";
    cout<<"=====================================\n";
    cout<<"Selected Stock: "<<s.name<<"\n";
    cout<<"-------------------------------------\n";

    int ch;
    do{
        cout<<"\n1.Add Daily Prices\n";
        cout<<"2.Display Prices\n";
        cout<<"3.Max/Min Price\n";
        cout<<"4.Moving Average (7-Days)\n";
        cout<<"5.Sort Prices\n";
        cout<<"6.Price Trend Analysis\n";
        cout<<"7.Search Price (by Day)\n";
        cout<<"8.Show Stack (Last 5)\n";
        cout<<"9.Back To Main Menu\n";
        cout<<"10.Show Weekly Records (Queue)\n";
        cout<<"-------------------------------------\n";
        cout<<"Enter Choice: ";
        cin>>ch;
        cout<<"-------------------------------------\n";

        switch(ch){
            case 1:{
                int days;
                cout<<"Enter number of days to add prices for: ";
                cin>>days;

                queue<int>priceQueue; 
                for(int i = 0; i < days; i++){
                    int price;
                    cout<<"Enter price of Day "<<(s.prices.size()+i+1)<<": ";
                    cin>>price;
                    priceQueue.push(price);
                }

                while(!priceQueue.empty()){
                    s.prices.push_back(priceQueue.front());
                    priceQueue.pop();
                }

                saveToFile();
                cout<<"Prices added successfully.\n";
                break;
            }

            case 2:{
                cout<<"\n-------------------------------------\n";
                cout<<" Displaying Prices for "<<s.name<<"\n";
                cout<<"-------------------------------------\n";
                if(s.prices.empty()){
                    cout<<"No prices recorded yet!\n";
                    break;
                }

                queue<int>q;
                for(int price : s.prices) q.push(price);

                cout<<"\n[Using Queue - Oldest to newest]\n";
                int day = 1;
                while(!q.empty()){
                    cout<<"Day "<<day<<": "<<q.front()<<endl;
                    day++;
                    q.pop();
                }

                stack<int> st;
                for(int price : s.prices) st.push(price);

                cout<<"\n[Using Stack - Newest to Oldest]\n";
                day = s.prices.size();
                while(!st.empty()){
                    cout<<"Day "<<day<<": "<<st.top()<<endl;
                    day--;
                    st.pop();
                }
                cout<<endl;
                break;
            }

            case 3:{
                cout<<"\n-------------------------------------\n";
                cout<<" Max/Min Price Analysis\n";
                cout<<"-------------------------------------\n";
                if(s.prices.empty()){
                    cout<<"No prices available!\n";
                    break;
                }
                stack<pair<int, int> >st;
                for(int i = 0; i < (int)s.prices.size(); i++){
                    st.push(make_pair(s.prices[i], i + 1));
                }
                int maxPrice = s.prices[0], minPrice = s.prices[0];
                int maxDay = 1, minDay = 1;

                while(!st.empty()){
                    int price = st.top().first;
                    int day = st.top().second;
                    st.pop();

                    if(price > maxPrice){
                        maxPrice = price;
                        maxDay = day;
                    }
                    if(price < minPrice){
                        minPrice = price;
                        minDay = day;
                    }
                }
                cout<<"Maximum Price: "<<maxPrice<<" (Day " <<maxDay<< ")\n";
                cout<<"Minimum Price: "<<minPrice<<" (Day " <<minDay<< ")\n";
                break;
            }

            case 4:{
                cout<<"\n-------------------------------------\n";
                cout<<" 7-Day Moving Average\n";
                cout<<"-------------------------------------\n";
                if(s.prices.size()<7){
                    cout<<"Not enough data to calculate 7-day moving average.\n";
                    break;
                }
                queue<int>window;
                double sum = 0;
                for(int i = 0; i < (int)s.prices.size(); i++){
                    window.push(s.prices[i]);
                    sum += s.prices[i];
                    if(window.size() == 7){
                        double avg = sum/7.0;
                        cout<<"Day "<<(i-6)<<" - "<<(i+1)<<" Average: "<<avg<<endl;
                        sum -= window.front();
                        window.pop();
                    }
                }
                break;
            }

            case 5:{
                cout<<"\n-------------------------------------\n";
                cout<<" Sorting Prices\n";
                cout<<"-------------------------------------\n";
                if(s.prices.empty()){
                    cout<<"No prices available!\n";
                    break;
                }
                int choice;
                cout<<"1. Ascending Order\n";
                cout<<"2. Descending Order\n";
                cout<<"Enter choice: ";
                cin>>choice;
                queue<int>priceQueue;
                for (int price : s.prices) priceQueue.push(price);
                vector<int>sortedPrices;
                while (!priceQueue.empty()) {
                    sortedPrices.push_back(priceQueue.front());
                    priceQueue.pop();
                }
                if(choice == 1){
                    sort(sortedPrices.begin(),sortedPrices.end());
                    cout<<"\nPrices sorted in Ascending Order:\n";
                }else if(choice == 2){
                    sort(sortedPrices.begin(),sortedPrices.end(), greater<int>());
                    cout<<"\nPrices sorted in Descending Order:\n";
                }else{
                    cout<<"Invalid choice.\n";
                    break;
                }
                stack<int>displayStack;
                for(int price : sortedPrices) displayStack.push(price);

                int day = sortedPrices.size();
                while(!displayStack.empty()){
                    cout<<"Day "<<day<<": "<<displayStack.top()<<endl;
                    displayStack.pop();
                    day--;
                }
                s.prices = sortedPrices;
                saveToFile();
                cout<<"Prices sorted and updated successfully!\n";
                break;
            }

            case 6:{
                cout<<"\n-------------------------------------\n";
                cout<<" Price Trend Analysis\n";
                cout<<"-------------------------------------\n";
                for(int i = 0; i < (int)s.prices.size()-1; i++){
                    if(s.prices[i] > s.prices[i+1]){
                        cout<<"trend goes on decreasing"<<endl;
                        s.trend.push_back("decreasing");
                    }
                    else if(s.prices[i] < s.prices[i+1]) s.trend.push_back("increasing");

                    else s.trend.push_back("stable");
                }
                saveToFile();
                break;
            }

            case 7:{
                cout<<"\n-------------------------------------\n";
                cout<<" Search Price by Day\n";
                cout<<"-------------------------------------\n";
                string stockName;
                int day;
                cout<<"Enter stock name to search: ";
                cin>>stockName;
                cout<<"Enter day number: ";
                cin>>day;
                bool found = false;
                for(auto &st : stocks){
                    if(st.name == stockName){
                        if(day >= 1 && day <= static_cast<int>(st.prices.size())){
                            cout<<"Price on day "<<day<<" for "<<st.name<< " is "<<st.prices[day-1]<<endl;
                            found = true;
                        }
                        else cout<<"Invalid day number.\n";
                        break;
                    }
                }
                if(!found) cout<<"Stock not found.\n";
                break;
            }

            case 8:{
                cout<<"\n-------------------------------------\n";
                cout<<" Last 5 Stocks (Stack Display)\n";
                cout<<"-------------------------------------\n";
                stack<Stock>lastFive;
                size_t start = (stocks.size() >= 5) ? (stocks.size()-5) : 0;
                for(size_t i = start; i < stocks.size(); ++i) lastFive.push(stocks[i]);
                while(!lastFive.empty()){
                    Stock st = lastFive.top();
                    cout<<st.name<<" => Prices: ";
                    for(int p : st.prices) cout<<p<<" ";
                    cout<<endl;
                    lastFive.pop();
                }
                break;
            }

            case 9:{
                cout<<"\nReturning To Main Menu...\n";
                break;
            }

            case 10:{
                cout<<"\n-------------------------------------\n";
                cout<<" Weekly Records (Queue Display)\n";
                cout<<"-------------------------------------\n";
                if(stocks.empty()){
                    cout<<"No stocks available!\n";
                    break;
                }
                queue<Stock> weeklyRecords;
                for(const auto& st : stocks){
                    weeklyRecords.push(st);
                }
                int week = 1;  
                while(!weeklyRecords.empty()){
                    Stock st = weeklyRecords.front();
                    weeklyRecords.pop();
                    cout<<"\nWeek "<<week<<" -> "<<st.name<<endl;
                    cout<<"Prices: ";
                    for(int p : st.prices) {
                        cout<<p<<" ";
                    }
                    cout<<"\nTrend: ";
                    if(!st.trend.empty()){
                        for(const string& t : st.trend){
                            cout << t << " ";
                        }
                    }
                    else cout<<"(trend not calculated)";
                    cout<<"\n---------------------------------------------\n";
                    week++;
                }
                break;
            }
            default: cout<<"Invalid Choice!\n";
            break;
        } 
    }while(ch!=9);
}

void deleteStock(){
    displayStocks();
    if(stocks.empty()){
        cout<<"No Stocks Available To Delete\n";
        return;
    }
    string name;
    cout<<"Enter Name Of The Company You Want To Delete: ";
    cin>>name;
    bool found=false;
    for(unsigned int i=0;i<stocks.size();i++){
        if(stocks[i].name==name){
            found=true;
            stocks.erase(stocks.begin()+i);
            saveToFile();
            cout<<"\n-------------------------------------\n";
            cout<<"Company Deleted Successfully\n";
            cout<<"-------------------------------------\n";
            break;
        }
    }
    if(!found) cout<<"Stock Of This Company Not Available\n";
    cout<<endl;
}

int main(){
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif

    loadStocks();

    cout<<"\n========================================\n";
    cout<<"        STOCK PRICE ANALYSIS SYSTEM        \n";
    cout<<"=========================================\n\n";

    int ch;
    do{
        cout<<"1.Add Stock\n";
        cout<<"2.Display All Stocks\n";
        cout<<"3.Select a Stock For Analysis\n";
        cout<<"4.Delete a Stock\n";
        cout<<"5.Exit..\n";
        cout<<"-----------------------------------------\n";
        cout<<"Enter Your Choice: ";
        cin>>ch;

        if(cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout<<"Invalid input! Please enter a valid number.\n";
            continue;
        }
        switch(ch){
            case 1:
                addStock();
                break;

            case 2:
                displayStocks();
                break;

            case 3:
                if(stocks.empty()){
                    cout<<"No Stocks Available!\n";
                    break;
                }
                cout<<"\nSelect Company\n";
                for(unsigned int i=0; i<stocks.size(); i++){
                    cout<<i+1<<". "<<stocks[i].name<<endl;
                }
                unsigned int id;
                cout<<"Enter ID: ";
                cin>>id;
                if(id>=1 && id<=stocks.size()){
                    stockMenu(stocks[id-1]);
                }else {
                    cout<<"Invalid Choice\n";
                }
                break;

            case 4:
                deleteStock();
                break;

            case 5:
                cout<<"\n========================================\n";
                cout<<"        Exiting the System...            \n";
                cout<<"========================================\n";
                break;

            default:
                cout<<"Invalid Choice. Re-enter\n";
                break;
        }
    }while(ch!=5);

    return 0;
}
