#include <gmp.h>    // gmp is included implicitly
#include <libhcs.h> // master header includes everything
#include <iostream>
#include <fstream>
#include <gmpxx.h>
#include <ctime>
using namespace std;

void TimeEncrypt(mpz_t test[], mpz_t encTest[], pcs_public_key* pk)
{
    double time_enc = 0;
    int time_start = clock();
    int round = 1;
    while(round <= 10000)
    {
        for(int i = 0; i < 5; ++i)
        {
            hcs_random *hr = hcs_init_random(); // random r value
            mpz_set_ui(test[i],1);
            pcs_encrypt(pk, hr, test[i], test[i]);
            pcs_ee_add(pk, encTest[i], encTest[i], test[i]);
            hcs_free_random(hr);
        }
        round++;
    }
    int time_end = clock();
    time_enc += (time_end - time_start) % CLOCKS_PER_SEC * 1000;
    cout << "Total PHE Encryption Time (10000 rounds): " << time_enc << " ms\n";
    cout << "Average PHE Encryption Time (1 round): " << time_enc/10000 << " ms\n";
}

void TimeDecrypt(mpz_t encTest[], pcs_private_key* vk)
{
    double time_dec = 0;
    int round = 1;
    mpz_t finalTest[5];
    int time_start = clock();
    while(round <= 10000)
    {
        for (int i = 0; i < 5; ++i)
        {
            pcs_decrypt(vk, finalTest[i], encTest[i]);
        }
        round++;
    }
    int time_end = clock();
    time_dec += (time_end - time_start) % CLOCKS_PER_SEC * 1000;
    cout << "Final Test Result: ";
    for (int i = 0; i < 5; ++i)
        gmp_printf("%Zd ", finalTest[i]);
    cout << "\nTotal PHE Decryption Time (10000 rounds): " << time_dec << " ms\n";
    cout << "Average PHE Decryption Time (1 round): " << time_dec/10000 << " ms\n";
}

int main()
{
    pcs_public_key *pk = pcs_init_public_key(); //public key
    pcs_private_key *vk = pcs_init_private_key(); // private key
    hcs_random *hr = hcs_init_random(); // random r value

    // Generate a key pair with modulus of selected size
    int keySize;
    cout << "Enter size of key you want (1024, 2048, 3072 bits): ";
    cin >> keySize; 
    pcs_generate_key_pair(pk, vk, hr, keySize);

     /*Key information*/
    gmp_printf("p = %Zd\nq = %Zd\n", vk->p, vk->q);
    gmp_printf("====================\n");
    gmp_printf("Public key (n, g):\n");
    gmp_printf("g = %Zd\nn = %Zd\n", pk->g, pk->n);
    gmp_printf("====================\n");
    gmp_printf("Private key (gMu, lambda):\n");
    gmp_printf("gMu = %Zd\ngLambda = %Zd\n", vk->mu, vk->lambda);
    gmp_printf("====================\n");

    //Estimate Performance
    mpz_t test[5];
    mpz_t encTest[5];
    for(int i =0;i<5;++i)
    {
        mpz_set_ui(test[i],0);
        pcs_encrypt(pk, hr, encTest[i], test[i]);
    }
    TimeEncrypt(test, encTest,pk);
    TimeDecrypt(encTest, vk);
    // Cleanup all data
    pcs_free_public_key(pk);
    pcs_free_private_key(vk);
    hcs_free_random(hr);
    return 0;
}