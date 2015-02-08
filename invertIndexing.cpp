#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <dirent.h>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <vector>
using namespace std;

void convertToLower(char c, string &word)
{
    int lw=0;
    while (c)
    {
        if(isupper(c)) word[lw] = tolower(c);
        lw++;
        c = word[lw];
    }
                
}

void addWord(string key, string fnum, unordered_map<string,vector<string>> &unmap)
{
    if (unmap[key].empty() || unmap[key].back()!=fnum)
    {
        //not in map.
        unmap[key].push_back(fnum);
    }
}

int main (int arc, char *argv[]) {

	int i=0;
    int count_file=0;
	ostringstream ss;
    string need = "abcdefghijklmnopqrstuvwxyz";

    /*unordered_map<string,vector<string>> unmap;*/

    //--------------------------count file in directory-----------------------------------
	DIR *dir;
	struct dirent *ent;
    string fol = argv[1];

    unordered_map<string, vector<string>> unmap;

    string mydir = "/home/silverice/Documents/Assignment_OS/"+fol+"/data";
	if ((dir = opendir (mydir.c_str())) != NULL) {
  	while ((ent = readdir (dir)) != NULL) {
    	//cout << ent->d_name << endl;
    	count_file++;
  	}
  		closedir (dir);
	} else {
  		perror ("");
  		return EXIT_FAILURE;
	}
    //--------------------------------------------------------------------------------------

	for(i=1;i<= count_file-2;i++) 
	{
		ss.str( string() );
		ss.clear();
		ss << i;
        string word;

        string fnum = ss.str();
		string namestr = mydir+"/file"+fnum+".txt";
  		ifstream myfile (namestr.c_str());
        

  		if (myfile.is_open())
  		{
    		while ( myfile >> word )
    		{
                convertToLower(word[0],word);

                if (word.find_first_not_of(need) != string::npos)
				{
                    //at least one char in word is special char.
    				int len = word.length();
                    string newword = "";
    				for(int j=0;j<len;j++)
    				{
                        if(need.find_first_of(word[j]) != string::npos)
                        {
                            //cout << word[j];
                            newword = newword+word[j];
                        }
                        else {
                            //cout << " ";
                            //break;
                            addWord(newword,fnum,unmap);
                            newword = "";
                        }
    				}
                    if(need.find_first_of(word[len-1]) != string::npos)
                    {
                        addWord(newword,fnum,unmap);
                    }
                    

				} else {
                    //not have special char in word.
                    //cout << word << " ";
                    addWord(word,fnum,unmap);
                }
    		}
    		myfile.close();
    		//cout << "\n\n\n";
  		}

  		else break; 
	}

    //sort.
    map<string,vector<string>> ordered(unmap.begin(), unmap.end());

    //write output to file.
    ofstream outputFile("myOutputMedium");
    vector<string> v;
    outputFile << ordered.size()-1 << endl;
    for (auto& x: ordered)
    {
        v = x.second;
        if (x.first != "")
        {
            outputFile << x.first << ":" ;
            outputFile << v.size() << ":" ;
            outputFile << v[0] ;
            for (int i = 1; i < v.size(); i++)
            {
                outputFile << "," << v[i] ;
            }

            outputFile << endl;
        }
        
    }
    
  return 0;
}