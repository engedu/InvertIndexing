#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <dirent.h>
#include <unordered_map>
#include <map>
#include <algorithm>
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

void addWord(string key, string fnum, unordered_map<string,string/*list<string>*/> &unmap)
{
    string str;
    unordered_map<string,string/*list<string>*/>::const_iterator got = unmap.find (key);
    
    if ( got == unmap.end() )
    {
        //not in map.
        //lt.push_back(fnum);
        unmap.emplace(key,fnum+","/*lt*/);
    }
    else {
        //already in map.
        str = got->second;
        if(str.find_first_of(fnum) == string::npos)
        {
            str+=fnum+",";
            unmap.at(key) = str;
        }

        /*lt.push_back(fnum);
        lt.unique();*/
        
    }
}

int main (int arc, char *argv[]) {

	int i=0;
	int count_file=0;
	ostringstream ss;
    string need = "abcdefghijklmnopqrstuvwxyz";

    unordered_map<string,string/*list<string>*/> unmap;

    //--------------------------count file in directory-----------------------------------
	DIR *dir;
	struct dirent *ent;
    string fol = argv[1];
    string mydir = "/home/silverice/Documents/Assignment_OS/"+fol+"/data";
	if ((dir = opendir (mydir.c_str())) != NULL) {
  	while ((ent = readdir (dir)) != NULL) {
    	//printf ("%s\n", ent->d_name);
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
		string word;
		ss << i;
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
                            break;
                        }
    				}
                    addWord(newword,fnum,unmap);

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
    map<string,string> ordered(unmap.begin(), unmap.end());

    //test value in map.
    //already right format!!!
    //To do : sort!
    string ans;
    size_t n;
    cout << ordered.size() << endl;
    for (auto& x: ordered)
    {
        ans = x.second;
        n = std::count(ans.begin(), ans.end(), ',');
        cout << x.first << ":" ;
        //lst = x.second;
        cout << n << ":" << ans.substr(0,ans.size()-1)/*lst.size()*/ ;
        /*for (list<string>::iterator it = lst.begin(); it != lst.end(); it++)
        {
            if(it == lst.begin())
                cout << *it ;
            else
                cout << "," << *it;
        }*/

        cout << endl;
    }
    
  return 0;
}