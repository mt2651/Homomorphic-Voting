#include <gmp.h>    // gmp is included implicitly
#include <libhcs.h> // master header includes everything
#include <iostream>
#include <vector>
#include <fstream>
#include <gmpxx.h>
using namespace std;

void savePublicKey(pcs_public_key *pk)
{
    ofstream ofs("publicKey.key");

    ofs << pk->g;
    ofs << "\n";
    ofs << pk->n2;
    ofs << "\n";
    ofs << pk->n;

    ofs.close();
}  

void savePrivateKey(pcs_private_key *vk)
{
    ofstream ofs("privateKey.key");

    ofs << vk->hp;
    ofs << "\n";
    ofs << vk->hq;
    ofs << "\n";
    ofs << vk->lambda;
    ofs << "\n";
    ofs << vk->mu;
    ofs << "\n";
    ofs << vk->n2;
    ofs << "\n";
    ofs << vk->n;
    ofs << "\n";
    ofs << vk->p2;
    ofs << "\n";
    ofs << vk->p;
    ofs << "\n";
    ofs << vk->q2;
    ofs << "\n";
    ofs << vk->q;

    ofs.close();
}

pcs_public_key* loadPublicKey()
{
    pcs_public_key *pk = pcs_init_public_key();
    ifstream ofs("publicKey.key");

    ofs >> pk->g;
    // ofs << "\n";
    ofs >> pk->n2;
    // ofs << "\n";
    ofs >> pk->n;

    ofs.close();

    return pk;
}

pcs_private_key* loadPrivateKey()
{
    pcs_private_key *vk = pcs_init_private_key();

    ifstream ofs("privateKey.key");

    ofs >> vk->hp;
    // ofs << "\n";
    ofs >> vk->hq;
    // ofs << "\n";
    ofs >> vk->lambda;
    // ofs << "\n";
    ofs >> vk->mu;
    // ofs << "\n";
    ofs >> vk->n2;
    // ofs << "\n";
    ofs >> vk->n;
    // ofs << "\n";
    ofs >> vk->p2;
    // ofs << "\n";
    ofs >> vk->p;
    // ofs << "\n";
    ofs >> vk->q2;
    // ofs << "\n";
    ofs >> vk->q;

    ofs.close();
    return vk;
}


int main(void)
{
    // initialize data structures
    pcs_public_key *pk = pcs_init_public_key(); //public key
    pcs_private_key *vk = pcs_init_private_key(); // private key
    hcs_random *hr = hcs_init_random(); // random r value

    // Generate a key pair with modulus of selected size
    int keySize;
    cout << "Enter size of key you want (1024, 2048, 3072 bits): ";
    cin >> keySize; 
    
    // pcs_generate_key_pair(pk, vk, hr, keySize);
    // savePrivateKey(vk);
    // savePublicKey(pk);
    
    pk = loadPublicKey();
    vk = loadPrivateKey();

    // mpz_t p, q, g, n, mu, lambda;

    /*Key information*/
    gmp_printf("p = %Zd\nq = %Zd\n", vk->p, vk->q);
    gmp_printf("====================\n");
    gmp_printf("Public key (n, g):\n");
    gmp_printf("g = %Zd\nn = %Zd\n", pk->g, pk->n);
    gmp_printf("====================\n");
    gmp_printf("Private key (gMu, lambda):\n");
    gmp_printf("gMu = %Zd\ngLambda = %Zd\n", vk->mu, vk->lambda);
    gmp_printf("====================\n");

    // cout << pk->g;

    ofstream pub("publicKey.txt");
    pub << pk->g;
    pub << "\n";
    pub << pk->n;
    pub.close();

    /*Voting Process*/
    int numCandidates;
    cout << "Number of Candidates: ";
    cin >> numCandidates;

    int numVoters;
    cout << "Number of Voters: ";
    cin >> numVoters;

    vector<mpz_t> finalResult(numCandidates);
    for(int i = 0; i < numCandidates; ++i)
    {
        mpz_set_ui(finalResult[i],0);
        pcs_encrypt(pk, hr, finalResult[i], finalResult[i]);
    }
    cout << "================\n";
    cout << "Rule: A voter is just allowed to vote once for only one Candidate!!\n";
    cout << "Election is ready!\n";
    cout << "================\n";
    
    for (int i = 0; i < numVoters; ++i)
    {
        cout << "\nVoters " << i+1 << ": ";
        vector<mpz_t> vote(numCandidates);
        vector<mpz_t> encVote(numCandidates);
        for (int j = 0; j < numCandidates; j++)
        {
            int x;
            cin >> x;
            mpz_set_ui(vote[j], x);
            hcs_random *r = hcs_init_random(); // random r value
            pcs_encrypt(pk, r, encVote[j], vote[j]);
            pcs_ee_add(pk, finalResult[j], finalResult[j],encVote[j]);
            hcs_free_random(r);
        }
        /*cout << "\nVoter " << i+1 << ": ";
        for (int j = 0; j < numCandidates; j++)
        {
            gmp_printf("%Zd ", vote[j]);
        }*/
        cout << "\nEncrypted Vote " << i+1 << ": \n\n";
        for (int j = 0; j < numCandidates; j++)
        {
            gmp_printf("Candidate %d: %Zd\n\n",j+1, encVote[j]);
        }
        cout << "\n============================\n\n";
    }
    
    cout << "Encrypted final result:\n";
    for (int i = 0; i < numCandidates; ++i)
    {
        cout << "\nCandidate " << i+1 << ": ";
        gmp_printf("%Zd\n", finalResult[i]);
    }

    cout << "\n============================\n\n";
    cout << "Final result: ";
    for (int i = 0; i < numCandidates; ++i)
    {
        pcs_decrypt(vk, finalResult[i], finalResult[i]);
        gmp_printf("%Zd ", finalResult[i]);
    }

    // Cleanup all data
    pcs_free_public_key(pk);
    pcs_free_private_key(vk);
    hcs_free_random(hr);

    return 0;
}