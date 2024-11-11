#include <bits/stdc++.h>
using namespace std;

class Candidates {
public:
    bool active;
    int id;

    Candidates(int pid) : id(pid), active(true) {}

    bool isActive() {
        return active;
    }
};

class BullyElection {
    int currIndex;
public:
    vector<Candidates> v;

    BullyElection(vector<Candidates>& candidatesList) : v(candidatesList) {}

    void addFailure(int failureId) {
        if (failureId < v.size()) {
            v[failureId].active = false;
            cout << "Candidate " << failureId << " has failed." << endl;
        }
    }

    void startElection(int initiatorId) {
        cout << "Election initiated by candidate " << initiatorId << endl;
        for (int i = initiatorId + 1; i < v.size(); i++) {
            if (v[i].active) {
                cout << "Candidate " << i << " receives an election message from " << initiatorId << endl;
            }
        }
        coordinateElection(initiatorId);
    }

    void coordinateElection(int initiatorId) {
        for (int i = v.size() - 1; i >= initiatorId; i--) {
            if (v[i].isActive()) {
                cout << "Candidate " << i << " becomes coordinator" << endl;
                currIndex = i;
                announceResult();
                return;
            }
        }
    }

    void announceResult() {
        for (Candidates c : v) {
            if (c.isActive() && c.id != currIndex) {
                cout << "Candidate " << c.id << " acknowledges candidate " << currIndex << " as coordinator" << endl;
            }
        }
    }
};

class RingElection {
    int currIndex;
public:
    vector<Candidates> v;

    RingElection(vector<Candidates>& candidatesList) : v(candidatesList) {}

    void startElection(int initiatorId) {
        cout << "Election initiated by candidate " << initiatorId << endl;
        currIndex = initiatorId;

        int i = (initiatorId + 1) % v.size();
        while (i != initiatorId) {
            if (v[i].isActive()) {
                cout << "Candidate " << i << " receives an election message from " << initiatorId << endl;
                currIndex = max(currIndex, i);
            }
            i = (i + 1) % v.size();
        }
        announceCoordinator();
    }

    void announceCoordinator() {
        cout << "Candidate " << currIndex << " becomes coordinator" << endl;
        for (Candidates c : v) {
            if (c.isActive() && c.id != currIndex) {
                cout << "Candidate " << c.id << " acknowledges candidate " << currIndex << " as coordinator" << endl;
            }
        }
    }
};

int main() {
    int numCandidates;
    cout << "Enter the number of candidates: ";
    cin >> numCandidates;

    vector<Candidates> candidatesList;
    for (int i = 0; i < numCandidates; i++) {
        candidatesList.push_back(Candidates(i));
    }

    cout << "Number of candidates: " << candidatesList.size() << endl;

    BullyElection bullyElection(candidatesList);

    int failureId;
    cout << "Enter the ID of the candidate to fail for the Bully Election: ";
    cin >> failureId;
    bullyElection.addFailure(failureId);

    int initiatorId;
    cout << "Enter the ID of the candidate to initiate the Bully election: ";
    cin >> initiatorId;
    bullyElection.startElection(initiatorId);

    cout << "\n------- Ring Election -------\n";
    RingElection ringElection(candidatesList);

    cout << "Enter the ID of the candidate to fail for the Ring Election: ";
    cin >> failureId;
    if (failureId < ringElection.v.size()) {
        ringElection.v[failureId].active = false;
        cout << "Candidate " << failureId << " has failed." << endl;
    }

    cout << "Enter the ID of the candidate to initiate the Ring election: ";
    cin >> initiatorId;
    ringElection.startElection(initiatorId);

    return 0;
}
