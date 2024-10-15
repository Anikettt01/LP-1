#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class PageReplacementAlgorithm
{
private:
    int n;          // Length of page reference sequence
    int nf;         // Number of frames
    vector<int> in; // Input reference sequence
    vector<int> p;  // frames
    int pgfaultcnt = 0;

    void initialize()
    {
        pgfaultcnt = 0;
        p.assign(nf, 9999);
    }
    bool isHit(int data)
    {
        for (int j = 0; j < nf; j++)
        {
            if (p[j] == data)
                return true;
        }
        return false;
    }
    void dispPages()
    {
        for (int k = 0; k < nf; k++)
        {
            if (p[k] != 9999)
                cout << " " << p[k];
        }
    }

    void displayPgFaultCnt()
    {
        cout << "Total no of faults: " << pgfaultcnt << endl;
    }

    int predict(int index)
    {
        int res = -1, farthest = index;
        for (int i = 0; i < p.size(); i++) {
            // Find farthest reference for a page in frame
            int j;
            for (j = index; j < n; j++) {
                if (p[i] == in[j]) {
                    if (j > farthest) {
                        farthest = j;
                        res = i;
                    }
                    break;
                }
            }
            cout << endl << "Farthest for " << p[i] << "->" << j << endl;

            // If no future reference, return the page to be replaced
            if (j == n)
                return i;
        }

        return (res == -1) ? 0 : res;
    }

public:
    void getData()
    {
        cout << "Enter length of page reference sequence: ";
        cin >> n;
        cout << "Enter the page reference sequence: ";
        in.resize(n); // resize the vector to n

        for (int i = 0; i < n; i++)
        {
            cin >> in[i];
        }

        cout << "Enter no of frames: ";
        cin >> nf;
        p.resize(nf);

        cout << endl;
    }
    void fifo()
    {   
        initialize();
        for (int i = 0; i < n; i++)
        {
            cout << "For " << in[i] << " : ";
            if (!isHit(in[i]))
            { 
                for (int k = 0; k < nf - 1; k++)
                    p[k] = p[k + 1];

                p[nf - 1] = in[i];
                pgfaultcnt++;
                dispPages();
            }
            else
                cout << "H I T";
            cout << endl;
        }
        cout << endl << "No. of hits = " << n- pgfaultcnt << endl;
        cout << "No. of misses = " << pgfaultcnt << endl;
        cout << endl;
    }
	
	void lru()
	{
		initialize();
		for(int i = 0; i < n; i++) 
		{
			cout << "For " << in[i] << " : ";
			if(!isHit(in[i]))
			{
				for (int k = 0; k < nf - 1; k++)
                    p[k] = p[k + 1];

                p[nf - 1] = in[i];
                pgfaultcnt++;
                dispPages();
			}
			else
			{	
				int idx = -1;
				for (int j = 0; j < nf; j++)
        		{
        		    if (p[j] == in[i])
                    {
        	 	       idx = j;
        	 	       break;
        			}
        		}
				// cout << "idx : " << idx << endl;
				int temp = p[idx];
				for(int k = idx; k < nf-1; k++) 
				{
					p[k] = p[k+1];
				}
				p[nf - 1] = temp;
				cout << "H I T \t";
				dispPages();
			}
			cout << endl;
		}
		cout << endl << "No. of hits = " << n - pgfaultcnt << endl;
        cout << "No. of misses = " << pgfaultcnt << endl;
        cout << endl;
	}


 
    void optimal()
    {
        p.clear();
        int hit = 0;
        for (int i = 0; i < n; i++) {
            
            cout << "For " << in[i] << " : ";
            // Page found in a frame : HIT
            if (isHit(in[i])) {
                hit++;
                cout << "H I T" << endl;
                continue;
            }

            // if space in frames
            if (p.size() < nf)
                p.push_back(in[i]);
    
            // Find the page to be replaced.
            else {
                int j = predict(i + 1);
                cout << "Replacing : " << p[j] << endl;
                p[j] = in[i];
            }
            dispPages();
            cout << endl;
        }
        cout << endl << "No. of hits = " << hit<< endl;
        cout << "No. of misses = " << n - hit << endl;
    }
  
};

int main()
{
    PageReplacementAlgorithm prAlgo;
    bool loop = true;
    int choice;

    while (loop)
    {
        cout << "###### PAGE REPLACEMET ALGORITHM ######" << endl;
        cout << "1. Enter Data" << endl;
        cout << "2. FIFO" << endl;
        cout << "3. LRU" << endl;
        cout << "4. Optimal" << endl;
        cout << "5. Exit" << endl;

        cout << "\n Enter your choice: ";
        cin >> choice;
        cout << endl;

        switch (choice)
        {
        case 1:
            prAlgo.getData();
            break;
        case 2:
            prAlgo.fifo();
            break;
        case 3:
         	prAlgo.lru();
         	break; 
        case 4:
            prAlgo.optimal();
            break;
        case 5:
            cout << "Exiting..." << endl;
            cout << "Thank you!!" << endl;
            loop = false;
        default:
            return 0;
            break;
        }
    }
}