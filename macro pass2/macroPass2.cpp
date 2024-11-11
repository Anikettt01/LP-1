#include <bits/stdc++.h>
using namespace std;

vector<string> readLines(const string& filename) {
    ifstream file(filename);
    vector<string> lines;
    string line;

    while (getline(file, line)) {
        lines.push_back(line);
    }
    return lines;
}

vector<string> split(const string& str, char delimiter) {
    vector<string> tokens;
    string token;
    stringstream tokenStream(str);

    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

class Process {
public:
    static string getMacroDef(const string& name, const vector<string>& MNT) {
        for (const auto& entry : MNT) {
            if (split(entry, ' ')[0] == name) {
                return entry; 
            }
        }
        return "";
    }

    static void macroMDT(const string& macroMNT, const vector<string>& MDT, const vector<string>& aptab) {
        int mdtIndex = stoi(split(macroMNT, ' ')[3]) - 1;  
        cout << "Expanded Code:" << endl;

        for (size_t i = mdtIndex; i < MDT.size(); ++i) {
            if (MDT[i] == "MEND") {
                break;  
            } else {
                vector<string> mdtLine = split(MDT[i], ' ');
                string instruction = mdtLine[0];
                string expandedLine = instruction;

                for (size_t j = 1; j < mdtLine.size(); ++j) {
                    string param = mdtLine[j];
                    if (param.substr(0, 3) == "(P,") {
                        int paramIndex = stoi(param.substr(3, param.size() - 4)) - 1;  
                        expandedLine += " " + aptab[paramIndex];  
                    } else {
                        expandedLine += " " + param; 
                    }
                }
                cout << expandedLine << endl;  
            }
        }
        cout << "_________________________________" << endl;
    }

    static vector<string> prepareAPTAB(const string& macroMNT, const vector<string>& KPDTAB, const string& macroCall) {
        vector<string> mntValues = split(macroMNT, ' ');
        int numPP = stoi(mntValues[1]);  
        int numKP = stoi(mntValues[2]); 
        int kpdTabStart = stoi(mntValues[4]) - 1; 

        vector<string> aptab;
        vector<string> callParams = split(split(macroCall, ' ')[1], ','); 

        for (const auto& param : callParams) {
            if (param.find('=') == string::npos) {
                aptab.push_back(param); 
            }
        }

        for (size_t i = kpdTabStart; i < kpdTabStart + numKP; ++i) {
            vector<string> entry = split(KPDTAB[i], ' ');
            aptab.push_back(entry[1]); 
        }

        for (const auto& param : callParams) {
            if (param.find('=') != string::npos) {
                vector<string> kvPair = split(param, '=');
                aptab.push_back(kvPair[1]); 
            }
        }
        return aptab; 
    }
};

int main() {
    vector<string> macroCalls = readLines("macro_call.txt");
    vector<string> MNT = readLines("MNT2.txt");
    vector<string> MDT = readLines("MDT2.txt");
    vector<string> KPDTAB = readLines("KPDTAB2.txt");

    for (auto& macroCall : macroCalls) {
        string macroName = split(macroCall, ' ')[0];  
        string macroEntry = Process::getMacroDef(macroName, MNT);  
        vector<string> aptab = Process::prepareAPTAB(macroEntry, KPDTAB, macroCall);
        Process::macroMDT(macroEntry, MDT, aptab);
    }

    return 0;
}

