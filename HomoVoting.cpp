#include <iostream>
#include <math.h>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

// Compute the greatest common divisor of a and b
/*int gcd(int a, int b)
{
    while (b > 0)
    {
        a = b;
        b= a % b; 
    }
    return a;
}*/

// Compute the lowest common multiple of a and b
int lcm(int a, int b)
{
    return a * b / __gcd(a, b);
}

// Check a number is a prime one?
bool isPrime(int n)
{
    if (n < 2)
        return false;
    if (n == 2 || n == 3)
        return true;
    for (int i = 2; i < n / 2; ++i)
        if (n % i == 0)
            return false;
    return true;
}

// Power in Modular Arithmetic
int power(int x, int y, int p)
{
    int res = 1; // Initialize result

    x = x % p; // Update x if it is more than or equal to p

    if (x == 0)
        return 0; // In case x is divisible by p;

    while (y > 0)
    {
        // If y is odd, multiply x with result
        if (y & 1)
            res = (res * x) % p;

        // y must be even now
        y = y >> 1; // y = y/2
        x = (x * x) % p;
    }
    return res;
}

// Print a vector
void printIntVector(const vector<int> &v)
{
    for (const auto &x : v)
        cout << x << ' ';
    cout << "\n";
}

//void printintVector(const vector<int> &v)
//{
//    for (const auto &x : v)
//        cout << x << ' ';
//    cout << "\n";
//}

// Modular Inverse
int mod_inv(int a, int b)
{
    int b0 = b, t, q;
    int x0 = 0, x1 = 1;
    if (b == 1)
        return 1;
    while (a > 1)
    {
        q = a / b;
        t = b, b = a % b, a = t;
        t = x0, x0 = x1 - q * x0, x1 = t;
    }
    if (x1 < 0)
        x1 += b0;
    return x1;
}

//Paillier Cryptosystem - Partially Homomorphic Encryption

// Hàm L(u)
int L(int u, int n)
{
    return (u - 1) / n;
}

// Hàm mã hóa
int ENC(int g, int x, int r, int n)
{
    int n2 = n * n;
    int k1 = power(g, x, n2);
    int k2 = power(r, n, n2);
    return (k1 * k2) % n2;
}

// Hàm giải mã
int DEC(int c, int n, int gLambda, int gMu)
{
    int k = power(c, gLambda, n * n);
    int l = L(k, n);
    return (l * gMu) % n;
}

// Generate key

// Tạo ngẫu nhiên một số nguyên tố p trong đoạn [3,1000]
/*int randomP()
{
    int p;
    do
    {
        srand(time(NULL));
        p = rand() % (1000 - 3 + 1) + 3;
    } while (!isPrime(p));
    return p;
}

// Tạo ngẫu nhiên một số nguyên tố q trong đoạn [3,1000] và khác p
int randomQ(int p)
{

    int q;
    do
    {
        srand(time(NULL));
        int q = rand() % (1000 - 3 + 1) + 3;
    } while (!isPrime(q) || q == p);
    return q;
}

// Tạo ngẫu nhiên một số g trong đoạn [3,1000] và nguyên tố cùng nhau với n*n
int randomG(int n)
{
    int n2 = n * n;
    int g;
    do
    {
        srand(time(NULL));
        g = rand() % (1000 - 3 + 1) + 3;
    } while (gcd(g, n2) != 1);
    return g;
}*/

// Tạo ngẫu nhiên một số r trong đoạn [3,1000] và nguyên tố cùng nhau với n
int randomR(int n)
{
    int r;
    do
    {
        srand(time(NULL));
        r = rand() % (100 - 1 + 1) + 1;
    } while (__gcd(r, n) != 1);
    return r;
}
int main()
{
    //int p = randomP();
    int p = 5;

    //int q = randomQ(p);
    int q = 7;

    int n = p * q;

    int gLambda = lcm(p - 1, q - 1);

    //int g = randomG(n);
    int g = 141;

    int gl = power(g, gLambda, n * n);
    int l = L(gl, n);
    int gMu = mod_inv(l, n);

    cout << "p=" << p << "\tq=" << q << "\n";
    cout << "g=" << g << "\n";
    cout << "================\n";
    cout << "Mu:\t\t" << gMu << "\tgLambda:\t" << gLambda << "\n";
    cout << "================\n";
    cout << "Public key (n,g):\t\t\t" << n << "\t" << g << "\n";
    cout << "Private key (lambda,mu):\t\t" << gLambda << "\t" << gMu << "\n";
    cout << "================\n";

    int numCandidates;
    cout << "Số người ứng cử: ";
    cin >> numCandidates;

    int numVoters;
    cout << "Số người bầu cử: ";
    cin >> numVoters;

    vector<int> finalResult(numCandidates, 1);
    cout << "================\n";
    cout << "Bắt đầu cuộc bầu cử: \n";
    cout << "================\n";
    for (int i = 1; i <= numVoters; ++i)
    {
        cout << "Người bầu cử " << i << ": \n";
        vector<int> vote(numCandidates);
        vector<int> encVote(numCandidates);
        for (int j = 0; j < numCandidates; j++)
        {
            int x;
            cin >> x;
            vote[j] = x;
            int r = randomR(n);
            encVote[j] = ENC(g, x, r, n);
            finalResult[j] *= encVote[j];
            finalResult[j] %= n * n;
        }
        cout << "Phiếu bầu cử " << i << ": ";
        printIntVector(vote);
        cout << "Encrypted phiếu bầu cử " << i << ": ";
        printIntVector(encVote);
        vote.clear();
        encVote.clear();
    }

    cout << "Encrypted kết quả cuối cùng: ";
    printIntVector(finalResult);
    for (int i = 0; i < numCandidates; ++i)
    {
        finalResult[i] = DEC(finalResult[i], n, gLambda, gMu);
    }
    cout << "Kết quả bầu cử cuối cùng: ";
    printIntVector(finalResult);

    return 0;
}