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
#include <pthread.h>
#include <unistd.h>
using namespace std;

#define NUM_THREADS     5

unordered_map<string, vector<int>> unmap;
pthread_mutex_t count_mutex;

bool myfunction (int i, int j) {
    return (i==j);
}

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

void addWord(string key, int fnum, unordered_map<string,vector<int>> &unmap)
{
    pthread_mutex_lock(&count_mutex);  
        unmap[key].push_back(fnum);
    pthread_mutex_unlock(&count_mutex);
}

struct thread_data{
   int  thread_id;
   int firstFile;
   int endFile;
   string mydir;
};

void *core(void *t)
{
    string need = "abcdefghijklmnopqrstuvwxyz"; 
    ostringstream ss;
    int fname;
   int i;
   long tid;
   tid = (long)t;

   sleep(1);
   struct thread_data *data;

   data = (struct thread_data *) t;

   /*cout << "Thread ID : " << data->thread_id ;
   cout << " Message : " << data->message << endl;*/

    for(int i = data->firstFile ; i<= data->endFile ;i++)
    {
        ss.str( string() );
        ss.clear();
        ss << i;
        string word;

        string fnum = ss.str();
        string namestr = data->mydir+"/file"+fnum+".txt";
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
                            fname = atoi(fnum.c_str());
                            addWord(newword,fname,unmap);
                            newword = "";
                        }
                    }
                    if(need.find_first_of(word[len-1]) != string::npos)
                    {
                        fname = atoi(fnum.c_str());
                        addWord(newword,fname,unmap);
                    }
                    

                } else {
                    //not have special char in word.
                    //cout << word << " ";
                    fname = atoi(fnum.c_str());
                    addWord(word,fname,unmap);
                }
            }
            myfile.close();
            //cout << "\n\n\n";
        }

        else break; 
    }

   pthread_exit(NULL);
}

int main (int arc, char *argv[]) 
{
    int count_file=0;


    //--------------------------count file in directory-----------------------------------
	DIR *dir;
	struct dirent *ent;
    string fol = argv[1];

    string mydir = "/home/silverice/Documents/Assignment_OS/"+fol+"/data";
	if ((dir = opendir (mydir.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            string a = ent->d_name;
    	   if (a.substr(0,4)=="file")
    	       count_file++;
        }
        closedir (dir);
	} else {
  		perror ("");
  		return EXIT_FAILURE;
	}
    //--------------------------------------------------------------------------------------

    int cnt = 0;
    int n = count_file / NUM_THREADS;
    int rc;
    pthread_t threads[NUM_THREADS];
    struct thread_data td[NUM_THREADS];
    pthread_attr_t attr;
    void *status;

    // Initialize and set thread joinable
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for(int i=0; i < NUM_THREADS; i++ ){
        cout << "main() : creating thread, " << i << endl;

        td[i].mydir = mydir;
        td[i].thread_id = i;
        td[i].firstFile = cnt+1;

        if (i == NUM_THREADS-1)
            td[i].endFile = count_file;

        else
        {
            cnt = cnt+n;
            td[i].endFile = cnt;
        }
        
        rc = pthread_create(&threads[i], NULL, core, (void *)&td[i] );
        if (rc){
            cout << "Error:unable to create thread," << rc << endl;
            exit(-1);
        }
    }

    // free attribute and wait for the other threads
    pthread_attr_destroy(&attr);
    for(int i=0; i < NUM_THREADS; i++ ){
        rc = pthread_join(threads[i], &status);
        if (rc){
            cout << "Error:unable to join," << rc << endl;
            exit(-1);
        }
        cout << "Main: completed thread id :" << i ;
        cout << "  exiting with status :" << status << endl;
    }

    //sort.
    map<string,vector<int>> ordered(unmap.begin(), unmap.end());

    //write output to file.
    ofstream outputFile("myOutputThread");
    vector<int> v;
    outputFile << ordered.size()-1 << endl;
    for (auto& x: ordered)
    {
        v = x.second;
        sort (v.begin(), v.end());
        vector<int>::iterator it;
        it = unique (v.begin(), v.end());
        v.resize( distance(v.begin(),it) );
        
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
/*        cout << "1" << td[0].firstFile << " " << td[0].endFile << endl;
        cout << "2" << td[1].firstFile << " " << td[1].endFile << endl;
        cout << "3" << td[2].firstFile << " " << td[2].endFile << endl;
        cout << "4" << td[3].firstFile << " " << td[3].endFile << endl;
        cout << "5" << td[4].firstFile << " " << td[4].endFile << endl;*/
/*vector<int> v;
        for (auto& x: unmap)
    {
        v = x.second;
        if (x.first != "")
        {
            cout << x.first << ":" ;
            cout << v.size() << ":" ;
            cout << v[0] ;
            for (int i = 1; i < v.size(); i++)
            {
                cout << "," << v[i] ;
            }

            cout << endl;
        }
        
    }*/


    cout << "Main: program exiting." << endl;
    pthread_exit(NULL);



/*	for(int i=1;i<= count_file-2;i++) 
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
	}*/

    /*//sort.
    map<string,vector<int>> ordered(unmap.begin(), unmap.end());

    //write output to file.
    ofstream outputFile("myOutputThread");
    vector<int> v;
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
        
    }*/
    
  return 0;
}