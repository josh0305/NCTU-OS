#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <bits/stdc++.h>

using namespace std;
int N;

int loc(int row, int col, int num)
{
	return row*num+col;
}

int main()
{
	pid_t pid;

    int shmid, shmid1;
    int *det, *mat;

    shmid = shmget(IPC_PRIVATE, 1024*8, IPC_CREAT|0666);
    det = (int *)shmat(shmid, NULL, 0);
    *det = 0;


    /*shmid1 = shmget(IPC_PRIVATE, 1024*8, IPC_CREAT|0666);
    mat = (int *)shmat(shmid1, NULL, 0);


    for(int row = 0; row < N; row++)
    {
    	for(int col = 0; col < N; col++)
    	{
    		cin >> mat[loc(row, col, N)];
    	}
    }

	pid = fork();
	if(pid == 0)
	{
		
		exit(0);
	}
	else
	{
		wait(NULL);
	}*/

	for(int i = 0; i < 3; i++)
	{
		pid = fork();
		if(pid == 0)
		{
			(*det)++;
			exit(0);
		}
	}
	wait(NULL);
	cout << *det << endl;
	return 0;
}