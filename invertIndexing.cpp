#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <dirent.h>
using namespace std;

void convertToLower(char c, string &word)
{
    //-----------------convert string to lowercase-------------------
    int lw=0;
    while (c)
    {
        if(isupper(c)) word[lw] = tolower(c);
        lw++;
        c = word[lw];
    }
    //-----------------------------------------------------------------
                
}

int main (int arc, char *argv[]) {

	int i=0;
	int count_file=0;
	ostringstream ss;

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
		ss.str( std::string() );
		ss.clear();
		string word;
		ss << i;
		string namestr = mydir+"/file"+ss.str()+".txt";
        string need = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  		ifstream myfile (namestr.c_str());
  		if (myfile.is_open())
  		{
    		while ( myfile >> word )
    		{
                convertToLower(word[0],word);
                if (word.find_first_not_of(need) != std::string::npos)
				{
                    //at least one char in word is special char.
    				char len = word.length();
    				for(int j=0;j<len;j++)
    				{
                        if(need.find_first_of(word[j]) != std::string::npos)
                        {
                            //cout << word[j];
                        }
                        else {
                            //cout << " ";
                            break;
                        }

    				}
				} else {
                    //not have special char in word.
                    //cout << word << " ";
                }
    		}
    		myfile.close();
    		//cout << "\n\n\n";
  		}

  		else break; 
	}
  
  return 0;
}