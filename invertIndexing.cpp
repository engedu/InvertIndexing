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

#define NUM_THREADS     2

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
        unmap[key].push_back(fnum);
    }
    
}

struct thread_data{
    int  thread_id;
    int firstFile;
    int endFile;
    string mydir;
    unordered_map<string, vector<string>> unmap;
};

void *core(void *t)
{
    string need = "abcdefghijklmnopqrstuvwxyz"; 
    ostringstream ss;
    long tid;
    tid = (long)t;

    sleep(1);
    struct thread_data *data;

    data = (struct thread_data *) t;

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
                            addWord(newword,fnum,data->unmap);
                            newword = "";
                        }
                    }
                    if(need.find_first_of(word[len-1]) != string::npos)
                    {
                        addWord(newword,fnum,data->unmap);
                    }
                    

                } else {
                    //not have special char in word.
                    //cout << word << " ";
                    addWord(word,fnum,data->unmap);
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

    string mydir = argv[1];
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

    unordered_map<string, vector<string>> unmap1;
    unordered_map<string, vector<string>> unmap2;

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

        td[i].mydir = mydir;
        td[i].thread_id = i;
        td[i].firstFile = cnt+1;

        //------------------give unmap--------------------------
        if (i==0)
        {
            td[i].unmap = unmap1;
        }
        else if (i==1)
        {
            td[i].unmap = unmap2;
        }
        //-------------------------------------------------------

        //--------------give endfile & increase cnt--------------
        if (i == NUM_THREADS-1)
            td[i].endFile = count_file;

        else
        {
            cnt = cnt+n;
            td[i].endFile = cnt;
        }
        //-------------------------------------------------------
        
        rc = pthread_create(&threads[i], NULL, core, (void *)&td[i] );
        if (rc){
            cout << "Error can't thread," << rc << endl;
            exit(-1);
        }
    }


    pthread_attr_destroy(&attr);

    for(int i=0; i < NUM_THREADS; i++ ){
        rc = pthread_join(threads[i], &status);
        if (rc){
            cout << "Error can't join," << rc << endl;
            exit(-1);
        }

    }

    //put data in thread 1 to thread 0
    for (auto& x: td[1].unmap)
        td[0].unmap[x.first].insert(td[0].unmap[x.first].end(),(x.second).begin(),(x.second).end());
    
    td[1].unmap.clear();

    //sort.
    map<string,vector<string>> ordered((td[0].unmap).begin(), (td[0].unmap).end());
    td[0].unmap.clear();

    //write output to file.
    ofstream outputFile("output");
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
    ordered.clear();
    
    pthread_exit(NULL);
    
  return 0;
}