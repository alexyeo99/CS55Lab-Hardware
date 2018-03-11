/*
Alex Yeo
CMSC 21 LAB#6

Notes:
1. ASCII 31 is used to denote the end of each field in the file. This is not visible in a normal text editor.
2. Records are limited to 100 
*/

#define REC_CNT 100 //Record MAX count.

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>

using namespace std;

struct recordRow{
    int recordID;
    string name;
    int qty;
    double cost;
};

struct recordTable{
    recordRow table[100];
};

void initFile();
bool fileExists();
recordTable loadTable();
void generateDefaultFile();
recordRow extractLine(string input);
void printTable(recordTable input);
void saveData(recordTable input);
recordTable newRecordFromObj(recordTable source, recordRow input);
recordTable newRecord(recordTable source, int recordID, string name, int qty, double cost);
recordRow returnNullRow(int recordID);
recordTable deleteRow(recordTable source, int recordID);
recordTable updateRow(recordTable source, int recordID, string name, int qty, double cost);
void printMenu();
recordTable interactiveAddEntry(recordTable source);
recordTable interactiveRemoveEntry(recordTable source);
recordTable interactiveUpdateEntry(recordTable source);

int main(){
    recordTable main;
    //check if file exists
    if(fileExists()){
        cout << ": FILE EXISTS!" << endl;
        main = loadTable();
    }else{
        cout << ": FILE DOES NOT EXIST!" << endl;
        //generate defaults
        main = newRecord(main, 77, "Sledge Hammer", 11, 21.50);
        main = newRecord(main, 3, "Electric Sander", 7, 58.98);
        main = newRecord(main, 17, "Hammer", 86, 11.99);
        main = newRecord(main, 24, "Jigsaw", 21, 11.00);
        main = newRecord(main, 39, "Lawn Mower", 3, 79.50);
        main = newRecord(main, 56, "Power Saw", 18, 99.99);
        main = newRecord(main, 68, "Screwdriver", 106, 6.99);
        saveData(main);
    }
    bool exit = false;
    cout << ": *This program automatically saves. Delete hardware.dat to reset to default values." << endl;
    while(!exit){
        printMenu();
        int choice;
        cin >> choice;
        if(choice>=1 && choice <=5){
            //VALID!
            //show submenu here.
            switch(choice){
	    	    case 1:
	    	        cout << endl << "--------------------------" << endl;
	    	        cout << "REC #, Name, QTY, PRICE" << endl;
	    	        cout << "--------------------------" << endl;
	    	        printTable(main);
		            break;
		        case 2:
		            cout << endl << "--------------------------" << endl;
	    	        cout << "ADD ENTRY" << endl;
	    	        cout << "--------------------------" << endl;
	    	        main = interactiveAddEntry(main);
	    	        break;
	    	    case 3:
	    	        cout << endl << "--------------------------" << endl;
	    	        cout << "REMOVE ENTRY" << endl;
	    	        cout << "--------------------------" << endl;
	    	        main = interactiveRemoveEntry(main);
	    	        break;
	    	    case 4:
	    	        cout << endl << "--------------------------" << endl;
	    	        cout << "UPDATE ENTRY" << endl;
	    	        cout << "--------------------------" << endl;
	    	        main = interactiveUpdateEntry(main);
	    	        break;
                case 5:
                    exit = true;
                    break;
            }
        }else{
            cout << ">> Please enter a valid choice!" << endl;
            sleep(1);
        }
        //exit code starts here
        saveData(main); //save before exit
         
    }
    
    //printTable(main);
    return 0;
}

void printMenu(){
    cout << endl << "-----" << endl << "MENU" << endl << "-----" << endl;
    cout << "1: List Tools" << endl;
    cout << "2: Add Entry" << endl;
    cout << "3: Remove Entry" << endl;
    cout << "4: Update Entry" << endl;
    cout << "5: Exit" << endl;
    cout << endl;
}

recordTable interactiveAddEntry(recordTable source){
    int recordID;
    string name;
    int qty;
    double cost;
    cout << "Record ID ?> ";
    cin >> recordID;
    if(!(recordID>=0 && recordID<=99)){
        cout << ": ERROR: Only recordIDs 0-99 are allowed. Resetting ..." << endl;
        return source;
    }
    cin.ignore();
    cout << "Item Name ?> ";
    getline(cin, name);
    cout << "Qty. ?> ";
    cin >> qty;
    cout << "Cost ?> ";
    cin >> cost;
    
    return newRecord(source, recordID, name, qty, cost);
}

recordTable interactiveUpdateEntry(recordTable source){
    int recordID;
    string name;
    int qty;
    double cost;
    cout << "Record ID ?> ";
    cin >> recordID;
    if(!(recordID>=0 && recordID<=99)){
        cout << ": ERROR: Only recordIDs 0-99 are allowed. Resetting ..." << endl;
        return source;
    }
    cout << endl << "Selected Record: " << endl;
    cout << "Item name: " << source.table[recordID].name << endl;
    cout << "QTY: " << source.table[recordID].qty << endl;
    cout << "Cost: " << source.table[recordID].cost << endl;
    cout << endl;
    cin.ignore();
    cout << "NEW Item Name ?> ";
    getline(cin, name);
    cout << "NEW Qty. ?> ";
    cin >> qty;
    cout << "NEW Cost ?> ";
    cin >> cost;
    
    return newRecord(source, recordID, name, qty, cost);
}

recordTable interactiveRemoveEntry(recordTable source){
    int recordID;
    cout << "Record ID ?> ";
    cin >> recordID;
    if(!(recordID>=0 && recordID<=99)){
        cout << ": ERROR: Only recordIDs 0-99 are allowed. Resetting ..." << endl;
        return source;
    }
    return deleteRow(source, recordID);
}

void initFile(){
    int x=0;
    ofstream fObj;
    fObj.open("hardware.dat");
    for(int i=0;i<REC_CNT;i++){
        fObj << " " << (char)31 << endl;
    }
    fObj.close();
}

bool fileExists(){
    if(FILE *file = fopen("hardware.dat","r")){
        fclose(file);
        return true;
    }else{
        return false;
    }
}

recordTable loadTable(){
    recordTable a;
    //1. open the file
    //2. add each row to recordRow
    //3. Add recordrow to recordtable
    //open file
    ifstream file;
    file.open("hardware.dat");
    string line;
    //iterate line by line and add data to struct
    while(getline(file,line)){
        recordRow b;
        b = extractLine(line);
        a.table[b.recordID] = b;
    }
    return a;
}

void generateDefaultFile(){
    recordTable a;
    //initialize struct
    for(int i=0;i<REC_CNT;i++){
        a.table[i].recordID = i;
        a.table[i].name = "NULL";
        a.table[i].qty = 50;
        a.table[i].cost = 10.10;
    }
    //write data to file
    saveData(a);
}

recordRow extractLine(string input){
    recordRow a;
    //use stoi()
    int elem = 0;
    string cell[4];
    for(int i=0;i<input.length();i++){
        if((int)input[i] == 31){
            elem++;
        }else{
            cell[elem] = cell[elem] + input[i];
        }
    }
    a.recordID = stoi(cell[0]);
    a.name = cell[1];
    a.qty = stoi(cell[2]);
    a.cost = stod(cell[3]);
    return a;
}

void printTable(recordTable input){
    for(int i=0;i<REC_CNT;i++){
        if(input.table[i].name != ""){
        cout << input.table[i].recordID << " "
        << input.table[i].name << " " 
        << input.table[i].qty << " "
        << input.table[i].cost << " "
        << endl;
        }
    }
}

void saveData(recordTable input){
    //write data to file
    ofstream fObj;
    fObj.open("hardware.dat");
    for(int i=0;i<REC_CNT;i++){
        if(input.table[i].name != ""){
            fObj << input.table[i].recordID << (char)31
            << input.table[i].name << (char)31
            << input.table[i].qty << (char)31
            << input.table[i].cost << (char)31
            << endl;
        }else{
            recordRow f = returnNullRow(i);
            fObj << f.recordID << (char)31
            << f.name << (char)31
            << f.qty << (char)31
            << f.cost << (char)31
            << endl;
        }
        
    }
    fObj.close();
}

recordTable newRecordFromObj(recordTable source, recordRow input){
    source.table[input.recordID] = input;
    return source;
}

recordTable newRecord(recordTable source, int recordID, string name, int qty, double cost){
    recordRow a;
    a.recordID = recordID;
    a.name = name;
    a.qty = qty;
    a.cost = cost;
    return newRecordFromObj(source, a);
}

recordRow returnNullRow(int recordID){
    recordRow a;
    a.recordID = recordID;
    a.name = "";
    a.qty = 0;
    a.cost = 0;
    return a;
}

recordTable deleteRow(recordTable source, int recordID){
    source.table[recordID] = returnNullRow(recordID);
    return source;
}

recordTable updateRow(recordTable source, int recordID, string name, int qty, double cost){
    return newRecord(source, recordID, name, qty, cost);
}
