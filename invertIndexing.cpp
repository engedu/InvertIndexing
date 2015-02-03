// reading a text file
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <dirent.h>
using namespace std;

int main () {
	int i=0;
	int count_file=0;
	ostringstream ss;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir ("/home/silverice/Documents/Assignment_OS/small/data")) != NULL) {
  	/* print all the files and directories within directory */
  	while ((ent = readdir (dir)) != NULL) {
    	//printf ("%s\n", ent->d_name);
    	count_file++;
  	}
  		closedir (dir);
	} else {
  		/* could not open directory */
  		perror ("");
  		return EXIT_FAILURE;
	}

	for(i=1;i<= count_file;i++)
	{
		ss.str( std::string() );
		ss.clear();
		string word;
		ss << i;
		string namestr = "/home/silverice/Documents/Assignment_OS/small/data/file"+ss.str()+".txt";

  		ifstream myfile (namestr.c_str());
  		if (myfile.is_open())
  		{
    		while ( myfile >> word )
    		{
      			/*if (word.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") != std::string::npos)
				{
    				char len = word.length();
    				for(int j=0;j<len;j++)
    				{
    					if (word[i].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") == std::string::npos)
    					{
    						cout << word[i];
    					}
    				}
				}
				else 
					cout << word << " ";*/
    		}
    		myfile.close();
    		//cout << "\n\n\n";
  		}

  		else break; 
	}
  
  return 0;
}