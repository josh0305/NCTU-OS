#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <bits/stdc++.h>

#define SIZE 4096

using namespace std;

int N;

// Function to get cofactor of mat[p][q] in temp[][]. n is current 
// dimension of mat[][] 
void getCofactor(int** mat, int **temp, int p, int q, int n) ;
  
/* Recursive function for finding determinant of matrix. 
   n is current dimension of mat[][]. */
int determinantOfMatrix(int **mat, int n);

//
void childprocess(int **mat, int id, int n, int *det);

int loc(int row, int col, int num);

  
// Driver program to test above functions 
int main() 
{ 
    int shmid, shmid1;
    int *det;

    cin >> N;

    int **mat = new int *[N];
    for(int i = 0; i < N; i++)
    	mat[i] = new int[N];
    for(int i = 0; i < N; i++)
    	for(int j = 0; j < N; j++)
    		cin >> mat[i][j];


    for(int i = 1; i <= 4; i++)
    {
    	struct timeval start, end;
    	gettimeofday(&start, 0);

    	shmid = shmget(IPC_PRIVATE, SIZE*8, IPC_CREAT|0666);
    	det = (int*)shmat(shmid, NULL, 0);
    	(*det) = 0;
    	for(int j = 0; j < i; j++)
    	{
    		pid_t pid = fork();
    		//childprocess calculate determinant
    		if(pid == 0)
    		{
    			childprocess(mat, j, i, det);
    			exit(0);
    		}
    		else if(pid < 0)
    		{
    			cout << "Error for create process" << endl;
    			return -1;
    		}
    	}

    	for(int j = 0; j < i; j++)
    		wait(NULL);

    	gettimeofday(&end, 0);
    	float sec = end.tv_sec - start.tv_sec;
    	float usec = end.tv_usec -start.tv_usec;
    	cout << "Calculating determinant using " << i << " process" << endl;
    	cout << "Elapsed time: " << sec+ (usec/1000000) << "sec, determinant : " << *det << endl;

    	if(shmdt(det) == -1)
    	{
    		cout << "detach error" << endl;
    		return -1;
    	}
    }

    for(int i = 0; i < N; i++)
    	delete mat[i];
    delete mat;

    return 0; 
} 


int determinantOfMatrix(int** mat, int n) 
{ 
    int D = 0; // Initialize result 
  
    //  Base case : if matrix contains single element 
    if (n == 1) 
        return mat[0][0]; 
  
    int **temp = new int*[N]; // To store cofactors 
    for(int i = 0; i < N; i++)
    	temp[i] = new int[N];
  
    int sign = 1;  // To store sign multiplier 
  
     // Iterate for each element of first row 
    for (int f = 0; f < n; f++) 
    { 
        // Getting Cofactor of mat[0][f] 
        getCofactor(mat, temp, 0, f, n); 
        D += sign * mat[0][f] * determinantOfMatrix(temp, n - 1); 
  
        // terms are to be added with alternate sign 
        sign = -sign; 
    } 
    
    for(int i = 0; i < N; i++)
    	delete temp[i];
    delete temp;

    return D; 
} 

void getCofactor(int** mat, int** temp, int p, int q, int n) 
{ 
    int i = 0, j = 0; 
  
    // Looping for each element of the matrix 
    for (int row = 0; row < n; row++) 
    { 
        for (int col = 0; col < n; col++) 
        { 
            //  Copying into temporary matrix only those element 
            //  which are not in given row and column 
            if (row != p && col != q) 
            { 
                temp[i][j++] = mat[row][col]; 
  
                // Row is filled, so increase row index and 
                // reset col index 
                if (j == n - 1) 
                { 
                    j = 0; 
                    i++; 
                } 
            } 
        } 
    } 
} 

void childprocess(int **mat, int id, int n, int *det)
{

	int **temp = new int*[N];
    for(int i = 0; i < N; i++)
    	temp[i] = new int[N];


    for (int f = id; f < N; f+=n) 
    { 
        int sign = 1;
    	if(f%2)
    		sign = -1;
        // Getting Cofactor of mat[0][f] 
        getCofactor(mat, temp, 0, f, N); 
        (*det) += sign * mat[0][f] * determinantOfMatrix(temp, N - 1); 
    } 
    
    for(int i = 0; i < N; i++)
    {
    	delete temp[i];
    	//delete mat2d[i];
    }
    delete temp;
    //delete mat2d;

    return;
}

int loc(int row, int col, int num)
{
	return row*num+col;
}