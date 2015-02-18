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

#define NUM_THREADS     3

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
   unordered_map<string, vector<int>> unmap;
};

void *core(void *t)
{
    string need = "abcdefghijklmnopqrstuvwxyz"; 
    ostringstream ss;
    int fname;
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
                            fname = atoi(fnum.c_str());
                            addWord(newword,fname,data->unmap);
                            newword = "";
                        }
                    }
                    if(need.find_first_of(word[len-1]) != string::npos)
                    {
                        fname = atoi(fnum.c_str());
                        addWord(newword,fname,data->unmap);
                    }
                    

                } else {
                    //not have special char in word.
                    //cout << word << " ";
                    fname = atoi(fnum.c_str());
                    addWord(word,fname,data->unmap);
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
    //string fol = argv[1];

    string mydir = argv[1];//"/home/silverice/Documents/Assignment_OS/"+fol+"/data";
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

    unordered_map<string, vector<int>> unmap1;
    unordered_map<string, vector<int>> unmap2;
    unordered_map<string, vector<int>> unmap3;

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

        if (i==0)
        {
            td[i].unmap = unmap1;
        }
        else if (i==1)
        {
            td[i].unmap = unmap2;
        }
        else if (i==2)
        {
            td[i].unmap = unmap3;
        }


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

        if (i==1)
        {
            for (auto& x: td[1].unmap)
            {
                td[0].unmap[x.first].insert(td[0].unmap[x.first].end(),(x.second).begin(),(x.second).end());
            }
            td[1].unmap.clear();
        }

        else if (i==2)
        {
            for (auto& x: td[2].unmap)
            {
                td[0].unmap[x.first].insert(td[0].unmap[x.first].end(),(x.second).begin(),(x.second).end());
            }
            td[2].unmap.clear();
        }
    }



    //sort.
    map<string,vector<int>> ordered((td[0].unmap).begin(), (td[0].unmap).end());

    //write output to file.
    ofstream outputFile("output");
    vector<int> v;
    vector<int>::iterator it;

    outputFile << ordered.size()-1 << endl;
    for (auto& x: ordered)
    {
        v = x.second;
        sort (v.begin(), v.end());
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
    td[0].unmap.clear();
    
    cout << "Main: program exiting." << endl;
    pthread_exit(NULL);
    
  return 0;
}