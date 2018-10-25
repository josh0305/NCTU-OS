#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <iostream>

using namespace std;

void ChildProcess();
void ParentProcess(int , int);

int main()
{
	int pid = fork();
	if(pid == 0)
		ChildProcess();
	else
	{
		ParentProcess(1, pid);
		wait(NULL);
		ParentProcess(2, pid);
	}	
	
	return 0;
}

void ChildProcess()
{
	cout << "This is Child process PID = " << getpid() << ", My parent's PID = " << getppid() << endl;
}

void ParentProcess(int type, int cpid)
{
	switch(type)
	{
		case 1:
			cout << "This is Parent process PID = " << getpid() << ", waiting for my child" << endl;
			break;
		case 2:
			cout << "This is Parent process, catch my child, PID = " << cpid << endl;
			break;
		default:
			break;
	}
}