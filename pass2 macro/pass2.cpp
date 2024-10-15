#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class MNTEntry {
public:
    MNTEntry() : name(""), pp(0), kp(0), mdtp(0), kpdtp(0) {}
    MNTEntry(const string& name, int pp, int kp, int mdtp, int kpdtp)
        : name(name), pp(pp), kp(kp), mdtp(mdtp), kpdtp(kpdtp) {}

    int getPp() const { return pp; }
    int getKp() const { return kp; }
    int getMdtp() const { return mdtp; }
    int getKpdtp() const { return kpdtp; }

private:
    string name;
    int pp, kp, mdtp, kpdtp;
};

int main() {
    ifstream irb("intermediate.txt");
    if (!irb.is_open()) {
        cerr << "Error opening intermediate.txt" << endl;
        return 1;
    }

    ifstream mdtb("mdt.txt");
    if (!mdtb.is_open()) {
        cerr << "Error opening mdt.txt" << endl;
        return 1;
    }

    ifstream kpdtb("kpdt.txt");
    if (!kpdtb.is_open()) {
        cerr << "Error opening kpdt.txt" << endl;
        return 1;
    }

    ifstream mntb("mnt.txt");
    if (!mntb.is_open()) {
        cerr << "Error opening mnt.txt" << endl;
        return 1;
    }

    ofstream fr("pass2.txt");
    if (!fr.is_open()) {
        cerr << "Error opening pass2.txt" << endl;
        return 1;
    }

    unordered_map<string, MNTEntry> mnt;
    unordered_map<int, string> aptab;
    unordered_map<string, int> aptabInverse;

    vector<string> mdt;
    vector<string> kpdt;

    string line;

    // Read MDT file
    while (getline(mdtb, line)) {
        mdt.push_back(line);
    }

    // Read KPDT file
    while (getline(kpdtb, line)) {
        kpdt.push_back(line);
    }

    // Read MNT file
    while (getline(mntb, line)) {
        istringstream iss(line);
        string parts[5];
        for (int i = 0; i < 5; ++i) {
            iss >> parts[i];
        }
        mnt[parts[0]] = MNTEntry(parts[0], stoi(parts[1]), stoi(parts[2]), stoi(parts[3]), stoi(parts[4]));
    }

    // Process IRB file
    while (getline(irb, line)) {
        istringstream iss(line);
        vector<string> parts;
        string part;
        while (iss >> part) {
            parts.push_back(part);
        }

        if (mnt.find(parts[0]) != mnt.end()) {
            const MNTEntry& entry = mnt[parts[0]];
            int pp = entry.getPp();
            int kp = entry.getKp();
            int mdtp = entry.getMdtp();
            int kpdtp = entry.getKpdtp();
            int paramNo = 1;

            // Process Parameters
            for (int i = 0; i < pp; ++i) {
                if (paramNo < parts.size()) {
                    parts[paramNo] = parts[paramNo].substr(0, parts[paramNo].find(','));
                    aptab[paramNo] = parts[paramNo];
                    aptabInverse[parts[paramNo]] = paramNo;
                    paramNo++;
                }
            }

            // Process KPDT
            int j = kpdtp - 1;
            for (int i = 0; i < kp; ++i) {
                if (j >= 0 && j < kpdt.size()) {
                    istringstream kpss(kpdt[j]);
                    vector<string> kpParts;
                    string kpPart;
                    while (getline(kpss, kpPart, '\t')) {
                        kpParts.push_back(kpPart);
                    }
                    if (kpParts.size() > 1) {
                        aptab[paramNo] = kpParts[1];
                        aptabInverse[kpParts[0]] = paramNo;
                        j++;
                        paramNo++;
                    }
                }
            }

            // Process rest of the parameters
            for (int i = pp + 1; i < parts.size(); ++i) {
                parts[i] = parts[i].substr(0, parts[i].find(','));
                size_t eqPos = parts[i].find('=');
                if (eqPos != string::npos) {
                    string name = parts[i].substr(0, eqPos);
                    string value = parts[i].substr(eqPos + 1);
                    name.erase(remove(name.begin(), name.end(), '&'), name.end());
                    if (aptabInverse.find(name) != aptabInverse.end()) {
                        aptab[aptabInverse[name]] = value;
                    }
                }
            }

            // Process MDT
            int i = mdtp - 1;
            while (i < mdt.size() && mdt[i] != "MEND") {
                istringstream mdiss(mdt[i]);
                vector<string> splits;
                string split;
                while (mdiss >> split) {
                    splits.push_back(split);
                }

                fr << "+";
                for (const string& s : splits) {
                    if (s.find("(P,") != string::npos) {
                        string numStr;
                        for (char c : s) {
                            if (isdigit(c)) numStr += c;
                        }
                        int num = stoi(numStr);
                        if (aptab.find(num) != aptab.end()) {
                            fr << aptab[num] << "\t";
                        }
                    } else {
                        fr << s << "\t";
                    }
                }
                fr << "\n";
                i++;
            }

            aptab.clear();
            aptabInverse.clear();
        } else {
            fr << line << "\n";
        }
    }

    // Close files
    fr.close();
    mntb.close();
    mdtb.close();
    kpdtb.close();
    irb.close();

    cout << "Pass 2 processing done!" << endl;

    return 0;
}
