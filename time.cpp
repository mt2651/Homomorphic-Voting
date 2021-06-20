#include <gmp.h>    // gmp is included implicitly
#include <libhcs.h> // master header includes everything
#include <iostream>

using namespace std;

mpz_t encTest;

void TimeEncrypt(pcs_public_key* pk)
{
    double time_enc = 0;
    hcs_random *r = hcs_init_random(); // random r value
    for (int i = 0; i < 100; i++)
    {
        int time_start = clock();
        // cout << time_start <<endl;
        mpz_t test;
        mpz_set_ui(test,1);
        pcs_encrypt(pk, r, test, test);
        int time_end = clock();
        // hcs_free_random(r);
        time_enc += (time_end - time_start)/double(CLOCKS_PER_SEC)*1000;
    }
    // cout << time_end <<endl;
    cout << "Average PHE Encryption Time (100 round): " << time_enc/100 << " ms\n";
    //cout << "Average PHE Encryption Time (1 round): " << time_enc/2 << " mus\n";
    hcs_free_random(r);

}


void TimeAddition(pcs_public_key* pk)
{
    double time_add = 0;
    hcs_random *r = hcs_init_random(); // random r value
    mpz_t test;
    for (int i = 0; i < 100; i++)
    {
        mpz_set_ui(test,1);
        pcs_encrypt(pk, r, test, test);
        int time_start = clock();
        pcs_ee_add(pk, encTest, encTest, test);
        int time_end = clock();
        // time_enc += (time_end - time_start) % CLOCKS_PER_SEC;
        time_add += (time_end - time_start)/double(CLOCKS_PER_SEC)*1000;
    }
    cout << "Average PHE Addition Time (1 round): " << time_add/100 << " ms\n";
    //cout << "Average PHE Encryption Time (1 round): " << time_enc/2 << " mus\n";
    hcs_free_random(r);

}

void TimeDecrypt(pcs_private_key* vk)
{
    double time_dec = 0;
    mpz_t finalTest;
    for(int i = 0; i < 100; i++)
    {
        int time_start = clock();
        pcs_decrypt(vk, finalTest, encTest);
        int time_end = clock();
        time_dec += (time_end - time_start)/double(CLOCKS_PER_SEC)*1000;
    }
    // cout << "Final Test Result: ";
    // gmp_printf("%Zd\n", finalTest);
    cout << "Average PHE Decryption Time (100 round): " << time_dec/100 << " ms\n";
    //cout << "Average PHE Decryption Time (1 round): " << time_dec/2 << " mus\n";
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

    //  /*Key information*/
    // gmp_printf("p = %Zd\nq = %Zd\n", vk->p, vk->q);
    // gmp_printf("====================\n");
    // gmp_printf("Public key (n, g):\n");
    // gmp_printf("g = %Zd\nn = %Zd\n", pk->g, pk->n);
    // gmp_printf("====================\n");
    // gmp_printf("Private key (gMu, lambda):\n");
    // gmp_printf("gMu = %Zd\ngLambda = %Zd\n", vk->mu, vk->lambda);
    // gmp_printf("====================\n");

    cout << "Performance with key size " << keySize << " bits" << endl;

    //Estimate Performance
    hcs_random *r = hcs_init_random(); // random r value
    mpz_set_ui(encTest,0);
    pcs_encrypt(pk, r, encTest, encTest);
    // gmp_printf("%Zd\n", encTest);
    
    TimeEncrypt(pk);
    TimeAddition(pk);
    TimeDecrypt(vk);
    
    // Cleanup all data
    pcs_free_public_key(pk);
    pcs_free_private_key(vk);
    hcs_free_random(hr);
    hcs_free_random(r);
    return 0;
}