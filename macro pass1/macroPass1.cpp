#include<bits/stdc++.h>
using namespace std;

class process {
public:
    map<int, map<string, string>> MNT;
    map<int, string> MDT;
    map<int, map<string, string>> KPDTAB;
    vector<string> PNTAB;

    int mntCounter = 1;
    int kpdtabCounter = 0;
    int mdtpCounter = 1;
    bool gotHead = false;

    void main(const vector<string>& inputFile) {
        for (auto i : inputFile) {
            if (i == "MACRO") gotHead = true;
            else if (i == "MEND") processMend();
            else if (gotHead) {
                gotHead = false;
                processMNT(i);
            }
            else processMDT(i);
        }
    }

    void processMNT(string& ins) {
        stringstream ss(ins);
        string name, params;
        ss >> name >> params;

        vector<string> parameters = split(params, ',');
        int currentKPDTIndex = kpdtabCounter + 1;
        int keyword = 0;
        int positional = 0;
        for (auto param : parameters) {
            if (param.find('=') != string::npos) {
                keyword++;
                PNTAB.push_back(param);
                processKPDTAB(param);
            }
            else {
                positional++;
                PNTAB.push_back(param);
            }
        }
        string kpdtp = "-";
        if (keyword != 0) {
            kpdtp = to_string(currentKPDTIndex);
        }

        MNT[mntCounter] = {
            {"Name", name},
            {"#PP", to_string(positional)},
            {"#KP", to_string(keyword)},
            {"MDTP", to_string(mdtpCounter)},
            {"KPDTP", kpdtp} // Corrected key name
        };
        mntCounter++;
    }

    void processKPDTAB(string& paramter) {
        auto pos = paramter.find('=');
        string name = paramter.substr(0, pos);
        string value = (pos + 1 < paramter.size()) ? paramter.substr(pos + 1) : "no default value";

        KPDTAB[kpdtabCounter++] = {
            {"Name", name},
            {"Value", value},
        };
    }

    void processMDT(string& word) {
        stringstream ss(word);
        string instructions, paramString;
        ss >> instructions >> paramString;

        vector<string> parameters = split(paramString, ',');
        string ins_line = instructions + " ";
        vector<string> parameterReference;

        for (string param : parameters) {
            if (param[0] == '=') {
                parameterReference.push_back(param); 
            } else {
                auto it = find(PNTAB.begin(), PNTAB.end(), param);
                int index = (it != PNTAB.end()) ? distance(PNTAB.begin(), it) + 1 : -1;
                if (index > 3) index -= 3;
                parameterReference.push_back("(P," + to_string(index) + ")");
            }
        }
        ins_line += join(parameterReference, " ");
        MDT[mdtpCounter++] = ins_line;
    }


    void processMend() {
        MDT[mdtpCounter++] = "MEND";
    }

    void showResult() const {
        cout << "Sr.No\tName\t#PP\t#KP\tMDTP\tKPDTP\n";
        for (auto it = MNT.begin(); it != MNT.end(); ++it) {
            int sr_no = it->first;
            auto& entry = it->second;

            cout << sr_no << "\t" << entry.at("Name") << "\t" << entry.at("#PP") << "\t"
                << entry.at("#KP") << "\t" << entry.at("MDTP") << "\t" << entry.at("KPDTP") << "\n";
        }

        cout << "\nMDT\nSr.No\tInstructions\n";
        for (auto it = MDT.begin(); it != MDT.end(); it++) {
            int sr_no = it->first;
            auto& entry = it->second;
            cout << sr_no << "\t" << entry << "\n";
        }

        cout << "\nKPDTAB\nSr.No\tName\tValue\n";
        for (auto it = KPDTAB.begin(); it != KPDTAB.end(); it++) {
            int sr_no = it->first;
            auto entry = it->second;
            cout << sr_no << "\t" << entry.at("Name") << "\t" << entry.at("Value") << "\n";
        }
    }

private:
    static vector<string> split(string& str, char delimiter) {
        vector<string> tokens;
        stringstream ss(str);
        string token;
        while (getline(ss, token, delimiter)) {
            tokens.push_back(token); // Fixed issue: Removed token.push_back(delimiter)
        }
        return tokens;
    }

    static string join(const vector<string>& vec, const string& delimiter) {
        ostringstream oss;
        for (size_t i = 0; i < vec.size(); i++) {
            if (i != 0) oss << delimiter;
            oss << vec[i];
        }
        return oss.str();
    }
};

int main() {
    ifstream file("input.txt");
    string line;
    vector<string> inputFile;
    while (getline(file, line)) {
        inputFile.push_back(line);
    }
    file.close();

    process obj;
    obj.main(inputFile);
    obj.showResult();

    return 0;
}
