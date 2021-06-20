#include <gmp.h>    // gmp is included implicitly
#include <libhcs.h> // master header includes everything
#include <iostream>
#include <vector>
#include <fstream>
#include <gmpxx.h>
using namespace std;


void Vote(pcs_public_key* pk);
void Result(pcs_private_key* vk);
void InfomationKey(pcs_private_key* vk, pcs_public_key* pk);
void savePublicKey(pcs_public_key *pk);
void savePrivateKey(pcs_private_key *vk);
pcs_public_key* loadPublicKey();
pcs_private_key* loadPrivateKey();
void saveElectionResult(vector<string> result);
bool loadElectionResult(vector<string> &result);

int main(void)
{
    // initialize data structures
    pcs_public_key *pk = pcs_init_public_key(); //public key
    pcs_private_key *vk = pcs_init_private_key(); // private key

    // Generate a key pair with modulus of selected size
    // int keySize;
    // cout << "Enter size of key you want (1024, 2048, 3072 bits): ";
    // cin >> keySize; 
    // pcs_generate_key_pair(pk, vk, hr, keySize);
    // savePrivateKey(vk);
    // savePublicKey(pk);
    
    pk = loadPublicKey();
    vk = loadPrivateKey();

    int mode;

    cout << "1: Infomation key" << endl;
    cout << "2: Vote" << endl;
    cout << "3: View Result" << endl;
    cout << "0: Exit" << endl;
    cout << "Enter mode: ";
    cin >> mode;
    cout << "=====================" << endl;
    
    switch (mode)
    {
    case 1:
        InfomationKey(vk, pk);
        break;
    
    case 2:
        Vote(pk);
        break;

    case 3:
        Result(vk);
        break;

    default:
        break;
    }


    // InfomationKey(vk, pk);

    // Vote(pk);

    // Result(vk);



    // Cleanup all data
    pcs_free_public_key(pk);
    pcs_free_private_key(vk);

    return 0;
}


void Vote(pcs_public_key* pk)
{
    /*Voting Process*/
    int numCandidates = 5;
    vector<mpz_t> finalResult(numCandidates);
    vector<string> result;
    

    bool openF = loadElectionResult(result);
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

    int numVoters;
    cout << "Number of Voters: ";
    cin >> numVoters;

    // cout << "================\n";
    cout << "Rule: A voter is just allowed to vote once for only one Candidate!!\n";
    // doi lai thanh 1 la dong y, 0 la khong dong y di, con bau cho bao nhieu nguoi cung dc
    cout << "Election is ready!\n";
    cout << "================\n";
    // cout << "Enter selection ";

    vector<mpz_t> vote(numCandidates);
    vector<mpz_t> encVote(numCandidates);
    for (int i = 0; i < numVoters; i++)
    {
        cout << "\nVoters " << i+1 << ": ";
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

    saveElectionResult(new_str);

    finalResult.clear();
    result.clear();
    vote.clear();
    encVote.clear();
    new_str.clear();

}

void Result(pcs_private_key* vk)
{
    int numCandidates = 5;
    vector<mpz_t> finalResult(numCandidates);
    vector<string> result;
    bool openF = loadElectionResult(result);
    if(!openF)
    {
        cout << "File 'resultElection' not exist!" << endl;
        return;
    }
    for (int i=0; i<5; ++i)
    {   
        mpz_t a;
        char *chr = &result[i][0];
        mpz_set_str(a, chr, 10);
        mpz_set(finalResult[i], a);
        // delete chr;
        // delete a;
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

    finalResult.clear();
    result.clear();    
    // finalResult.resize(0);
    result.resize(0);
}


void InfomationKey(pcs_private_key* vk, pcs_public_key* pk)
{
    /*Key information*/
    gmp_printf("p = %Zd\nq = %Zd\n", vk->p, vk->q);
    gmp_printf("====================\n");
    gmp_printf("Public key (n, g):\n");
    gmp_printf("g = %Zd\nn = %Zd\n", pk->g, pk->n);
    gmp_printf("====================\n");
    gmp_printf("Private key (gMu, lambda):\n");
    gmp_printf("gMu = %Zd\ngLambda = %Zd\n", vk->mu, vk->lambda);
    gmp_printf("====================\n");

}

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

