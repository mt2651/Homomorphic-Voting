#include <gmp.h>    // gmp is included implicitly
#include <libhcs.h> // master header includes everything
#include <iostream>
#include <vector>
#include <fstream>
#include <gmpxx.h>
#include <ctime>
using namespace std;

void TimeEncrypt(pcs_public_key* pk)
{
    double time_enc = 0;
    int time_start;
    int time_enc;
    for (int i = 0; i < 10000; i++)
    {
        time_start = clock();
        

    }
}

void TimeDecrypt(pcs_private_key* vk)
{

}

int main()
{
    pcs_public_key *pk = pcs_init_public_key(); //public key
    pcs_private_key *vk = pcs_init_private_key(); // private key
    hcs_random *hr = hcs_init_random(); // random r value

    pcs_generate_key_pair(pk, vk, hr, 1024);

}