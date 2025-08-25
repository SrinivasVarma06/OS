#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <signal.h>

using namespace std;

int main(int argc, char **argv)
{
    if (argc != 5)
    {
        cout << "usage: ./partitioner.out <path-to-file> <pattern> <search-start-position> <search-end-position>\nprovided arguments:\n";
        for (int i = 0; i < argc; i++)
            cout << argv[i] << "\n";
        return -1;
    }

    char *file_to_search_in = argv[1];
    char *pattern_to_search_for = argv[2];
    int search_start_position = atoi(argv[3]);
    int search_end_position = atoi(argv[4]);

    string str(pattern_to_search_for);
    ifstream file(file_to_search_in);
    string text = "";
    int pid = -1, idx = -1;
    bool flag = false;

    int len=strlen(pattern_to_search_for);
    file.seekg(search_start_position);
    
    char c;
    string sample = "";
    int cnt=search_start_position;
    
    while (file.get(c) && cnt<search_end_position)
    {
        sample += c;
        cnt++;
        if (sample.size() == len)
        {
            if (sample == str)
            {
                pid = (int)getpid();
                idx = cnt - len;
                flag = true;
                break;
            }
            sample.erase(0, 1);
        }
    }
    file.close();

    if(pid!=-1 && idx != -1){
        cout <<"["<<getpid()<<"]"<< " found at position " << idx << endl;
    }else{
        cout <<"["<<getpid()<<"]"<< " didn't find\n";
    }
    return 0;
}
