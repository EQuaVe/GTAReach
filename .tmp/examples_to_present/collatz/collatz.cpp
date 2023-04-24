// CPP program to print Collatz sequence
#include <bits/stdc++.h>
using namespace std;
 
void printCollatz(int n)
{
    // We simply follow steps
    // while we do not reach 1
    while (n != 1)
    {
        cout << n << " ";
 
        // If n is odd
        if (n & 1)
            n = 3*n + 1;
 
        // If even
        else
            n = n/2;
    }
 
    // Print 1 at the end
    cout << n;
}
 
// Driver code
int main(int argc, char** argv)
{
    int n;
   if(argc<2)
    cin >> n;
   else
    n=atoi(argv[1]);
    printCollatz(n);
    cout << endl;
    return 0;
}

