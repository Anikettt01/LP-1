#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include <iterator>
using namespace std;
int main()
{
    ifstream inp("input.txt");
    ofstream mnt("mnt.txt");
    ofstream mdt("mdt.txt");
    ofstream kpdt("kpdt.txt");
    ofstream pnt("pnt.txt");
    ofstream inr("intermediate_code.txt");
    if (!inp.is_open() || !mnt.is_open() || !mdt.is_open() || !kpdt.is_open() || !pnt.is_open() ||
        !inr.is_open())
    {
        cerr << "Error in opening file." << endl;
        return 1;
    }
    map<string, int> pntab;
    string in_line;
    string name;
    int kpdtp = 0, mdtp = 1, flag = 0, pp = 0, kp = 0, paramno = 0;
    while (getline(inp, in_line))
    {
        istringstream iss(in_line);
        vector<string> parts((istream_iterator<string>(iss)), istream_iterator<string>());
        if (parts.empty())
            continue;
        if (parts[0] == "MACRO" || parts[0] == "macro")
        {
            flag = 1;
            if (!getline(inp, in_line))
                break;
            istringstream iss2(in_line);
            vector<string> parts2((istream_iterator<string>(iss2)), istream_iterator<string>());
            name = parts2[0];
            kp = pp = 0;
            paramno = 0;
            if (parts2.size() <= 1)
            {
                mnt << parts2[0] << "\t" << pp << "\t" << kp << "\t" << mdtp << "\t" << (kp == 0 ? kpdtp : (kpdtp + 1)) << "\n";
                continue;
            }
            for (size_t i = 1; i < parts2.size(); i++)
            {
                string param = parts2[i];
                param.erase(remove_if(param.begin(), param.end(), [](char c)
                                      { return c == '&' ||
                                               c == ','; }),
                            param.end());
                if (param.find('=') != string::npos)
                {
                    kp++;
                    size_t pos = param.find('=');
                    string keyword_pp = param.substr(0, pos);
                    string value = param.substr(pos + 1);
                    pntab[keyword_pp] = paramno++;
                    kpdt << keyword_pp << "\t" << value << "\n";
                }
                else
                {
                    pntab[param] = paramno++;
                    ++pp;
                }
            }
            mnt << parts2[0] << "\t" << pp << "\t" << kp << "\t" << mdtp << "\t" << (kp == 0 ? kpdtp : (kpdtp + 1)) << "\n";
            kpdtp += kp;
            kp = 0;
            pp = 0;
        }
        else if (parts[0] == "MEND" || parts[0] == "mend")
        {
            mdt << in_line << "\n";
            flag = 0;
            ++mdtp;
            pnt << name << ":\t";
            for (const auto &pair : pntab)
            {
                pnt << pair.first << "\t";
            }
            pnt << "\n";
            pntab.clear();
        }
        else if (flag == 1)
        {
            for (const auto &part : parts)
            {
                if (part.find('&') != string::npos)
                {
                    string param = part;
                    param.erase(remove_if(param.begin(), param.end(), [](char c)
                                          { return c == '&' ||
                                                   c == ','; }),
                                param.end());
                    if (pntab.find(param) != pntab.end())
                    {
                        mdt << "(P," << pntab[param] << ")\t";
                    }
                }
                else
                {
                    mdt << part << "\t";
                }
            }
            mdt << "\n";
            ++mdtp;
        }
        else
        {
            inr << in_line << "\n";
        }
    }
    inp.close();
    mnt.close();
    mdt.close();
    kpdt.close();
    pnt.close();
    inr.close();
    return 0;
}