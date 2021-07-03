/*HOMOMORPHIC ENCRYPTION - BASED ONLINE VOTING SYSTEM*/

#include <iostream>
using namespace std;
#include <vector>

/* Library supports Paillier Algorithm */
#include <libhcs.h>

/* Support large integer computation */
#include <gmp.h> 
#include <gmpxx.h>

/* Load and Save key, result to file */
#include <fstream>


/* Key functions*/
void savePublicKey(pcs_public_key *pk);
void savePrivateKey(pcs_private_key *vk);
pcs_public_key* loadPublicKey();
pcs_private_key* loadPrivateKey();

/* Three main modules */
void InfomationKey(pcs_private_key* vk, pcs_public_key* pk);
void Vote(pcs_public_key* pk);
void Result(pcs_private_key* vk);

/* Load and Save Election Result */
void saveElectionResult(vector<string> result);
bool loadElectionResult(vector<string> &result);

int main(void)
{
    /* Initialize Paillier public key and private key */
    pcs_public_key *pk = pcs_init_public_key(); //public key
    pcs_private_key *vk = pcs_init_private_key(); // private key
    
    /* Generate a key pair with modulus of selected size */
    /* If keys dont exist, please uncomment these line to generate keys */
    
    // int keySize;
    // cout << "Enter size of key you want (1024, 2048, 3072 bits): ";
    // cin >> keySize; 
    // pcs_generate_key_pair(pk, vk, hr, keySize);
    // savePrivateKey(vk);
    // savePublicKey(pk);
    
    /* Load key to use */
    pk = loadPublicKey();
    vk = loadPrivateKey();

    
    /* Menu of modules */
    cout << "1: Key Information" << endl;
    cout << "2: Vote" << endl;
    cout << "3: View Result" << endl;
    cout << "0: Exit" << endl;
    /* Choose module */
    int mode;
    cout << "Enter mode: ";
    cin >> mode;
    cout << "=====================" << endl;
    
    switch (mode)
    {
    case 1:
        /* Parameters list of public key and private key */
        InfomationKey(vk, pk);
        break;
    
    case 2:
        /* Voting Procedure: Encrypt ballot and Computation in server */    
        Vote(pk);
        break;

    case 3:
        /* Return final result */
        Result(vk);
        break;

    default: /* Exit */
        break;
    }

    // Cleanup all data
    pcs_free_public_key(pk);
    pcs_free_private_key(vk);

    return 0;
}

/* Save public key into publicKey.key */
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

/* Save private key into privateKey.key */
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

/* Load public key from file publicKey.key */
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



/* Load private key from file privateKey.key */
pcs_private_key* loadPrivateKey()
{
    pcs_private_key *vk = pcs_init_private_key();

    ifstream ofs("privateKey.key");

    ofs >> vk->hp; // p
    // ofs << "\n";
    ofs >> vk->hq; // q
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

/* Print key information: parameters of public and private key */
void InfomationKey(pcs_private_key* vk, pcs_public_key* pk)
{
    /*Key information*/
    gmp_printf("p = %Zd\nq = %Zd\n", vk->p, vk->q);
    gmp_printf("====================\n");
    gmp_printf("Public key (g, n):\n");
    gmp_printf("g = %Zd\nn = %Zd\n", pk->g, pk->n);
    gmp_printf("====================\n");
    gmp_printf("Private key (gMu, lambda):\n");
    gmp_printf("gMu = %Zd\ngLambda = %Zd\n", vk->mu, vk->lambda);
    gmp_printf("====================\n");
}

/* Vote module: Encrypt ballot and compute in server */
void Vote(pcs_public_key* pk)
{
    /* Initialize election with 5 candidates */
    int numCandidates = 5;
    /* Vector to store election result of candidates in server */
    vector<mpz_t> finalResult(numCandidates);
    /* Store result in string format to save into file */
    vector<string> result;
    /* To check whether election result file exists  */
    bool openF = loadElectionResult(result);
    
    /* if it exists, load result in large integer(mpz_t) format*/
    if(openF)
    {
        char* chr;
        for (int i=0; i<5; ++i)
        {   
            mpz_t a;
            chr = &result[i][0];
            mpz_set_str(a, chr, 10);
            mpz_set(finalResult[i], a);
        }
        delete chr;
    }
    /* if not, create a new result file in server and initialize with encrypted 0 values */
    else 
    {
        cout << "Not exist file 'resultElection'!\nStart new voting" << endl;

        for(int i = 0; i < numCandidates; ++i)
        {
            hcs_random *hr = hcs_init_random(); // random r value
            mpz_set_ui(finalResult[i],0);
            pcs_encrypt(pk, hr, finalResult[i], finalResult[i]);
        }
    }
    
    /* Enter number of voters */
    int numVoters;
    cout << "Number of Voters: ";
    cin >> numVoters;
    
    cout << "Rule: A voter is just allowed to vote once!!\n";
    cout << "Election is ready!\n";
    cout << "================\n";
    
    /* Vector to store ballot of each voter */
    vector<mpz_t> vote(numCandidates);
    /* Vector to store encrypted ballot of each voter */
    vector<mpz_t> encVote(numCandidates);
    
    for (int i = 0; i < numVoters; i++)
    {
        cout << "\nVoter " << i+1 << ": ";
        for (int j = 0; j < numCandidates; j++)
        {
            /* For each candidate, vote x = 1(for) or vote x = 0(against)*/
            int x;
            cin >> x;
            /* Store ballot */
            mpz_set_ui(vote[j], x);
            /* random r value */
            hcs_random *r = hcs_init_random(); 
            /* Encrypt ballot of each candidate */
            pcs_encrypt(pk, r, encVote[j], vote[j]);
            /* Add process in server */
            pcs_ee_add(pk, finalResult[j], finalResult[j],encVote[j]);
            hcs_free_random(r);
        }
        
        /* Print encrypted ballot of each voter */
        cout << "\nEncrypted Vote " << i+1 << ": \n\n";
        for (int j = 0; j < numCandidates; j++)
        {
            gmp_printf("Candidate %d: %Zd\n\n",j+1, encVote[j]);
        }       
    }

    vector<string> new_str;
    for (int i=0; i<5; ++i)
    {
        string a = mpz_get_str(NULL, 10, finalResult[i]);
        new_str.push_back(a);
    }

    cout << "The election is successful, the results are saved in file 'resultElection.txt' ";
    /* Save election result at this time to file */
    saveElectionResult(new_str);

    finalResult.clear();
    result.clear();
    vote.clear();
    encVote.clear();
    new_str.clear();

}

/* View election result */
void Result(pcs_private_key* vk)
{
    int numCandidates = 5;
    vector<mpz_t> finalResult(numCandidates);
    vector<string> result;
    
    /* Load Election Result */
    bool openF = loadElectionResult(result);
    if(!openF)
    {
        cout << "File 'resultElection' not exist!" << endl;
        return;
    }
    /* if it exists, convert result of each candidate from string format to integer(mpz_t) format, then store in finalResult vector */
    for (int i=0; i< numCandidates; ++i)
    {   
        mpz_t a;
        char *chr = &result[i][0];
        mpz_set_str(a, chr, 10);
        mpz_set(finalResult[i], a);
        // delete chr;
        // delete a;
    }
    
    /* Print encrypted result atfer election */
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
        /* Decrypt ballot of each candidates using private key */
        pcs_decrypt(vk, finalResult[i], finalResult[i]);
        /* Print decrypted result */
        gmp_printf("%Zd ", finalResult[i]);
    }

    finalResult.clear();
    result.clear();    
    // finalResult.resize(0);
    result.resize(0);
}


void saveElectionResult(vector<string> result)
{
    ofstream ofs;
    ofs.open("resultElection.txt");
    // mpz_t ans;
    for (int i=0; i<result.size(); ++i)
    {
        ofs << result[i] << endl; 
    }

    ofs.close();

}

bool loadElectionResult(vector<string> &result)
{
    ifstream ifs("resultElection.txt");
    if(!ifs)
        return 0;

    string str;
    while(getline(ifs, str))
    {
        result.push_back(str);
    }
    ifs.close();

    return 1;
}

