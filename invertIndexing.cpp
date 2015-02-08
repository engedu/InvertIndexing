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
using namespace std;

void convertToLower(char c, string &word)
{
    int lw=0;
    while (c)
    {
        word[lw] = tolower(c);
        lw++;
        c = word[lw];
    }
                
}

void addWord(string key, string fnum, unordered_map<string,string> &unmap)
{
    string str;
    /*unordered_map<string,string>::const_iterator got = unmap.find (key);
    
    if ( got == unmap.end() )
    {
        //not in map.
        unmap.emplace(key,fnum+",");
    }
    else {
        //already in map.
        str = got->second;
        if (str.find(fnum)==std::string::npos)
        {
            str+=fnum+",";
            unmap.at(key) = str;
        }
    }*/

    if (unmap[key]=="")
    {
        //not in map.
        //unmap.emplace(key,fnum+",");
        unmap[key] = fnum+",";
    }
    else {
        //already in map.
        str = unmap[key];
        if (str.find(fnum)==std::string::npos)
        {
            str+=fnum+",";
            //unmap.at(key) = str;
            unmap[key] = str;
        }
    }
}

int main (int arc, char *argv[]) {

	int i=0;
	int count_file=0;
	ostringstream ss;
    string need = "abcdefghijklmnopqrstuvwxyz";

    unordered_map<string,string> unmap;

    //--------------------------count file in directory-----------------------------------
	DIR *dir;
	struct dirent *ent;
    string fol = argv[1];
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
    map<string,string> ordered(unmap.begin(), unmap.end());

    //write output to file.
    ofstream outputFile("myOutputMedium");
    string ans;
    size_t n;
    outputFile << ordered.size()-1 << endl;
    for (auto& x: ordered)
    {
        if (x.first != "")
        {
            ans = x.second;
            n = count(ans.begin(), ans.end(), ',');
            outputFile << x.first << ":" ;
            outputFile << n << ":" << ans.substr(0,ans.size()-1);

            outputFile << endl;
        }
        
    }
    
  return 0;

  //cut first line in file
}