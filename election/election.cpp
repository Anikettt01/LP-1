#include<bits/stdc++.h>
#include<unistd.h>
using namespace std;

class candidates{
    public:
        int id;
        bool active;

        candidates(){
            id = 0;
            active = true;
        }

        void addCandidates(){
            cout<<"Enter the id of new Candidates: ";
            cin>>id;
        }
};

class election{
    public:
    vector<candidates>v;
    int currIndex;

    void getCandidates(){
        candidates c;
        c.addCandidates();
        v.push_back(c);
    }

    // first find max id and its index simply print current co-ordinate then it will fail which will be detected by some index then
    // deactivate currIndex and then print election initialized
    void addFailure(){
        int max=0;
        // max logic
        for(int i=0;i<v.size();i++){
            if(v[i].id > max && v[i].active){
                max = v[i].id;
                currIndex = i;
            }
        }
        cout<<"Current co-ordinate is: "<<v[currIndex].id<<endl;
        sleep(2);
        cout<<"Current co-ordinate failed";
        cout<<"Detected by: "<<v[(currIndex-1+v.size())%v.size()].id<<endl;
        v[currIndex].active = false;
        sleep(2);
        cout<<"Election Initialized"<<endl;
    }

    // Ring algorithm
    // first store the current in old  then create new i.e previous node run a while loop until new and old becomes same increment by one 
    // and simply print that message pass from and print current new id after the loop again find the maximum simply copy paste from addFailure
    void ringAlgorithm(){
        int old = currIndex;
        int new1 = (old-1+v.size())%v.size();
        while(new1!=old){
            int next = (new1+1)%v.size();
            if(v[next].active){
                sleep(2);
                cout<<"message passed from "<<v[new1].id<<"to "<<v[next].id<<endl;
            }
            new1 = next;
        }
        int max=0;
        // max logic
        for(int i=0;i<v.size();i++){
            if(v[i].id > max && v[i].active){
                max = v[i].id;
                currIndex = i;
            }
        }
        cout<<"New co-ordinate is: "<<v[currIndex].id<<endl;
    }

    // find max elements same from addFailure print current elements then find next element to that kind of same to addFailture
    // keep one flag false add a loop if [i] element is active and its id is greater than current then pass the message to new id
    // make flag true after the loop if flag is false then print current id
    void bullyAlgorithm(){
        int max=0;
        for(int i=0;i<v.size();i++){
            if(v[i].active && v[i].id > max){
                currIndex = i;
                max = v[i].id;
            }
        }
        cout<<"Current co-ordinate is: "<<v[currIndex].id<<endl;
        sleep(2);
        cout<<"Current co-ordinate failed"<<endl;

        int new1 = (currIndex-1+v.size())%v.size();
        cout<<"Detected by "<<v[new1].id<<endl;
        v[currIndex].active = false;
        sleep(2);
        cout<<"Election initialized"<<endl;

        bool foundCordinate = false;
        for(int i=new1 ; i != currIndex ; i = (i+1)%v.size()){
            if(v[i].active && v[i].id > v[currIndex].id){
                currIndex = i;
                foundCordinate = true;
                break;
            }
        }
        if(!foundCordinate){
            cout<<"co-ordinate is: "<<v[currIndex].id<<endl;
        }
    }
};

int main(){
    election e;
    bool loop = true;

    while(loop){
        int n;
        cout<<"enter the number of candidates: ";
        cin>>n;
        for(int i=0;i<n;i++){
            e.getCandidates();
        }
        cout<<"choose one election algorithm"<<endl;
        cout<<"1.Ring algorithm"<<endl;
        cout<<"2.Bully algorithm"<<endl;
        cout<<"3.exit"<<endl;

        int choice;
        cin>>choice;
        
        switch(choice){
            case 1:
            e.addFailure();
            e.ringAlgorithm();
            break;

            case 2:
            e.addFailure();
            e.bullyAlgorithm();
            break;

            case 3:
            loop  = false;
            break;

            default:
            cout<<"Invalid choice"<<endl;
        }
    }
    return 0;
}