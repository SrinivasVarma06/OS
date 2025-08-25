#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <cstring>
using namespace std;

int main(int argc, char **argv)
{
	if(argc != 6)
	{
		cout <<"usage: ./partitioner.out <path-to-file> <pattern> <search-start-position> <search-end-position> <max-chunk-size>\nprovided arguments:\n";
		for(int i = 0; i < argc; i++)
			cout << argv[i] << "\n";
		return -1;
	}
	
	char *file_to_search_in = argv[1];
	char *pattern_to_search_for = argv[2];
	int search_start_position = atoi(argv[3]);
	int search_end_position = atoi(argv[4]);
	int max_chunk_size = atoi(argv[5]);

	cout << "[" << getpid() << "] start position = " << search_start_position << " ; end position = " << search_end_position << "\n";
	int diff = search_end_position - search_start_position;
	if(diff>max_chunk_size){
		int left_start = search_start_position;
		int left_end = search_start_position + (search_end_position - search_start_position)/2;
		int right_start = search_start_position + (search_end_position - search_start_position)/2 + 1;
		int right_end = search_end_position;
		
		string left_st=to_string(left_start);
		string left_en=to_string(left_end);
		string right_st=to_string(right_start);
		string right_en=to_string(right_end);

		pid_t left_child = fork();
		
		if(left_child<0){
			cout << "Error in forking left child";
			return -1;
		}
		if(left_child==0){
			char* args[7];

			args[0] = strdup("./part2_partitioner.out");
			args[1] = file_to_search_in;
			args[2] = pattern_to_search_for;
			args[3] = strdup(left_st.c_str());
			args[4] = strdup(left_en.c_str());
			args[5] = strdup(argv[5]);
			args[6] = (char*) nullptr;

			execvp(args[0], args);
			cout<<"Error in execvp of left child";
		}

		pid_t right_child = fork();
		
		if(right_child<0){
			cout << "Error in forking right child";
			return -1;
		}

		if(right_child==0){
			char* args[7];
			args[0] = strdup("./part2_partitioner.out");
			args[1] = file_to_search_in;
			args[2] = pattern_to_search_for;
			args[3] = strdup(right_st.c_str());
			args[4] = strdup(right_en.c_str());
			args[5] = strdup(argv[5]);
			args[6] = (char*) nullptr;

			execvp(args[0], args);
			cout<<"Error in execvp of right child";
		}

		cout << "[" << getpid() << "] forked left child " << left_child << "\n";
		cout << "[" << getpid() << "] forked right child " << right_child << "\n";

		int left_status, right_status;
		pid_t left_wait=waitpid(left_child,&left_status,0);
		pid_t right_wait=waitpid(right_child,&right_status,0);

		if(left_wait<0){
			cout << "Error in waiting for left child";
			return -1;
		}

		if(right_wait<0){
			cout << "Error in waiting for right child";
			return -1;
		}

		if(WIFEXITED(left_status)){
			cout << "[" << getpid() << "] left child returned\n";
		}

		if(WIFEXITED(right_status)){
			cout << "[" << getpid() << "] right child returned\n";
		}

	}
	else{
		pid_t searcher_child = fork();
		
		if(searcher_child<0){
			cout << "Error in forking searcher child";
			return -1;
		}
		if(searcher_child==0){
			char* args[6];
			args[0] = strdup("./part2_searcher.out");
			args[1] = file_to_search_in;
			args[2] = pattern_to_search_for;
			args[3] = strdup(argv[3]);
			args[4] = strdup(argv[4]);
			args[5] = (char*) nullptr;

			execvp(args[0], args);

			cout<<"Error in execvp of searcher child";

		}else{
			cout << "[" << getpid() << "] forked searcher child " << searcher_child << "\n";
			int status;

			pid_t searcher_wait=waitpid(searcher_child,&status,0);
			if(searcher_wait<0){
				cout << "Error in waiting for searcher child";
				return -1;
			}
			if(WIFEXITED(status))
				cout << "[" << getpid() << "] searcher child returned \n";
			
		}
	}
	
	//TODO
	//cout << "[" << my_pid << "] start position = " << search_start_position << " ; end position = " << search_end_position << "\n";
	//cout << "[" << my_pid << "] forked left child " << my_children[0] << "\n";
	//cout << "[" << my_pid << "] forked right child " << my_children[1] << "\n";
	//cout << "[" << my_pid << "] left child returned\n";
	//cout << "[" << my_pid << "] right child returned\n";
	//cout << "[" << my_pid << "] left child returned\n";
	//cout << "[" << my_pid << "] right child returned\n";*/
	//cout << "[" << my_pid << "] forked searcher child " << searcher_pid << "\n";
	//cout << "[" << my_pid << "] searcher child returned \n";
	//cout << "[" << my_pid << "] received SIGTERM\n"; //applicable for Part III of the assignment

	return 0;
}

