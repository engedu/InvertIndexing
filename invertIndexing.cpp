// reading a text file
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <dirent.h>
using namespace std;

int main (int arc, char *argv[]) {
	int i=0;
	int count_file=0;
	ostringstream ss;
	DIR *dir;
	struct dirent *ent;
    string fol = argv[1];
    string mydir = "/home/silverice/Documents/Assignment_OS/"+fol+"/data";
	if ((dir = opendir (mydir.c_str())) != NULL) {
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
      			if (word.find_first_not_of(need) != std::string::npos)
				{
    				char len = word.length();
    				for(int j=0;j<len;j++)
    				{
                        if(need.find_first_of(word[i]) != std::string::npos)
                            cout << word[i];

    				}
				}
				else 
					cout << word << " ";
    		}
    		myfile.close();
    		//cout << "\n\n\n";
  		}

  		else break; 
	}
  
  return 0;
}