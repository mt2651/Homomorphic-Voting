#include <gmp.h>    // gmp is included implicitly
#include <libhcs.h> // master header includes everything
#include <iostream>

using namespace std;

mpz_t encTest;

/* Time performance in 10000 rounds */
void TimeEncrypt(pcs_public_key* pk);
void TimeDecrypt(pcs_private_key* vk);
void TimeAddition(pcs_public_key* pk);

int main()
{
    pcs_public_key *pk = pcs_init_public_key(); //public key
    pcs_private_key *vk = pcs_init_private_key(); // private key
    hcs_random *hr = hcs_init_random(); // random r value

    // Generate a key pair with modulus of selected size
    int keySize;
    cout << "Enter size of key you want measure: ";
    cin >> keySize; 
    pcs_generate_key_pair(pk, vk, hr, keySize);

    cout << "Performance with key size " << keySize << " bits" << endl;

    //Estimate Performance
    hcs_random *r = hcs_init_random(); // random r value
    mpz_set_ui(encTest,0);
    pcs_encrypt(pk, r, encTest, encTest);
    
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


void TimeEncrypt(pcs_public_key* pk)
{
    double time_enc = 0;
    hcs_random *r = hcs_init_random(); // random r value
    for (int i = 0; i < 10000; i++)
    {
        int time_start = clock();
        mpz_t test;
        mpz_set_ui(test,1);
        pcs_encrypt(pk, r, test, test);
        int time_end = clock();
        time_enc += (time_end - time_start)/double(CLOCKS_PER_SEC)*1000;
    }
    cout << "Average PHE Encryption Time: " << time_enc/10000 << " ms\n";
    hcs_free_random(r);

}


void TimeAddition(pcs_public_key* pk)
{
    double time_add = 0;
    hcs_random *r = hcs_init_random(); // random r value
    mpz_t test;
    for (int i = 0; i < 10000; i++)
    {
        mpz_set_ui(test,1);
        pcs_encrypt(pk, r, test, test);
        int time_start = clock();
        pcs_ee_add(pk, encTest, encTest, test);
        int time_end = clock();
        time_add += (time_end - time_start)/double(CLOCKS_PER_SEC)*1000;
    }
    cout << "Average PHE Addition Time: " << time_add/10000 << " ms\n";
    hcs_free_random(r);

}

void TimeDecrypt(pcs_private_key* vk)
{
    double time_dec = 0;
    mpz_t finalTest;
    for(int i = 0; i < 10000; i++)
    {
        int time_start = clock();
        pcs_decrypt(vk, finalTest, encTest);
        int time_end = clock();
        time_dec += (time_end - time_start)/double(CLOCKS_PER_SEC)*1000;
    }

    cout << "Average PHE Decryption Time: " << time_dec/10000 << " ms\n";
}
