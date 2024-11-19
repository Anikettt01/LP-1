// first fit, best fit, worst fit, next fit implementation
#include<bits/stdc++.h>
using namespace std;

class memoryManagement{
    public:
        // vector having pair to store status and size of memory block
        vector<pair<string,int>>mainMemory;
        int nextLocation;
        // constructor
        // take input of number of blocks and sizes of each block and add in vector
        memoryManagement(){
            int num,size;
            cout<<"Enter number of blocks: ";
            cin>>num;
            for(int i=0;i<num;i++){
                cout<<"Enter size of block "<<i+1<<": ";
                cin>>size;
                mainMemory.push_back(make_pair("Free",size));
            }
            nextLocation = -1;
        }

        // firstFit
        // take input for process name and process size simply check if block is free and its size is greater than or equal to processize insert it
        void firstFit(){
            string processName;
            int processSize;
            cout<<"Enter process name: ";
            cin>>processName;
            cout<<"Enter process size: ";
            cin>>processSize;
            bool allocated = false;

            for(size_t i=0;i<mainMemory.size();i++){
                if(mainMemory[i].first == "Free" && mainMemory[i].second >= processSize){
                    mainMemory[i].first = processName;
                      allocated = true;
                    break;
                }
            }
            if (!allocated) {
                cout << "Not enough memory to allocate " << processName << endl;
            }
        }

        // take input for process name and size keep two variables to keep track on bestSize and bestIndex
        // run a loop on memory vector and check if block is free and size is available then check for if its less than bestsize then simply
        // change bestsize and bestlocation according to this block then if betslocation != -1 then add processname to that black else add 
        // no memory block available 
        void bestFit(){
            string processName;
            int processSize;
            cout<<"Enter process name: ";
            cin>>processName;
            cout<<"Enter process size: ";
            cin>>processSize;

            int bestSize = INT_MAX;
            int bestLocation = -1;
             bool allocated = false;
            for(size_t i=0;i<mainMemory.size();i++){
                if(mainMemory[i].first=="Free" && mainMemory[i].second >= processSize){
                    if(mainMemory[i].second < bestSize){
                        bestSize = mainMemory[i].second;
                        bestLocation=i;
                    }
                }
            }
            if(bestLocation != -1) {
                mainMemory[bestLocation].first = processName;
                allocated = true;
            }
            if (!allocated) {
                cout << "Memory not available for " << processName << endl;
            }
        }

        // same as bestfit just there we are checking for less memory size here check for more memory size
        void worstFit(){
            string processName;
            int processSize;
            cout<<"Enter process name: ";
            cin>>processName;
            cout<<"Enter process size: ";
            cin>>processSize;
            bool allocated = false;
            
            int worstSize=0;
            int worstLocation = -1;
            for(size_t i=0;i<mainMemory.size();i++){
                if(mainMemory[i].first=="Free" && mainMemory[i].second >= processSize){
                    if(mainMemory[i].second > worstSize){
                        worstSize = mainMemory[i].second;
                        worstLocation=i;
                    }
                }
            }
            if (worstLocation != -1) {
                mainMemory[worstLocation].first = processName;
                allocated = true;
            }

            if (!allocated) {
                cout << "Memory not available for " << processName << endl;
            }
        }

        // take input for processname and processsize 
        // // find next location
        // if block is free and have greater size than processSize add it and update nexrlocation if memoryblock not present then not enough memory
        void nextFit() {
            string processName;
            int processSize;
            cout << "Enter process name: ";
            cin >> processName;
            cout << "Enter process size: ";
            cin >> processSize;

            int i = (nextLocation + 1) % mainMemory.size();
            bool allocated = false;
            while (i != nextLocation) {
                if (mainMemory[i].first == "Free" && mainMemory[i].second >= processSize) {
                    mainMemory[i].first = processName;
                    nextLocation = i;
                    allocated = true;
                    return;
                }
                i = (i + 1) % mainMemory.size();
            }

            if (!allocated) {
                cout << "Not enough memory for " << processName << endl;
            }
        }

        void viewMemory(){
            int i=0;
            for(auto it:mainMemory){
                cout<<i++<<" "<<it.first<<" "<<it.second<<endl;
            }
        }  
};

int main() {
    memoryManagement obj;

    while (true) {
        cout << "\nSelect allocation method:" << endl;
        cout << "1. First Fit" << endl;
        cout << "2. Best Fit" << endl;
        cout << "3. Worst Fit" << endl;
        cout << "4. Next Fit" << endl;
        cout << "5. View Memory" << endl;
        cout << "6. Exit" << endl;

        int choice;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "First Fit Allocation:" << endl;
                obj.firstFit();
                break;
            case 2:
                cout << "Best Fit Allocation:" << endl;
                obj.bestFit();
                break;
            case 3:
                cout << "Worst Fit Allocation:" << endl;
                obj.worstFit();
                break;
            case 4:
                cout << "Next Fit Allocation:" << endl;
                obj.nextFit();
                break;
            case 5:
                cout << "Current Memory Status:" << endl;
                obj.viewMemory();
                break;
            case 6:
                cout << "Exiting..." << endl;
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}