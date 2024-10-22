#include <iostream>
#include<vector>
#include<set>

using namespace std;
#define ll long long int
// Utility function for modular exponentiation
set<int> prime;
ll public_key,private_key;

    void primeGenerate()
    {
        vector<int> sieve(500, 1);
        sieve[0] = 0;
        sieve[1] = 0;
        for (int i = 2; i * i < 500; i++)
        {
            if (sieve[i])
            {
                for (int j = i * i; j < 500; j += i)
                {
                    sieve[j] = 0;
                }
            }
        }
        for (int i = 0; i < sieve.size(); i++)
        {
            if (sieve[i]) prime.insert(i);
        }
    }
    int getRandomPrime()
    {
        auto it = prime.begin();
        advance(it, rand() % prime.size());
        int rand_prime = *it;
        prime.erase(it);
        return rand_prime;
    }
ll modExp(ll base, ll exp, ll mod) {
    ll result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

// GCD function for coprimeness check
ll gcd(ll a, ll b) {
    while (b != 0) {
        ll temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Function to find modular inverse using the extended Euclidean algorithm
ll mod_inverse(ll a, ll mod) {
    if(gcd(a, mod) > 1)return -1;

    for (ll x = 1; x < mod; x++)
        if (((a % mod) * (x % mod)) % mod == 1)return x;
}





vector<ll> blind_message(ll m,ll e, ll n){

    ll r1 = rand() % n;
    ll r2 = rand() % n;
    ll a1 = getRandomPrime();
    ll a2 = getRandomPrime();

    while(gcd(r1, n) != 1 or gcd(r2, n) != 1){
        r2++;
    }

    cout<<"r1 = "<<r1<<endl;
    cout<<"r2 = "<<r2<<endl;

    ll alpha1 = (modExp(r1,e,n)*modExp(m,a1,n))%n;
    ll alpha2 = (modExp(r2,e,n)*modExp(m,a2,n))%n;

    vector<ll>ans={alpha1,alpha2,r1,r2,a1,a2};

    return ans;
}


vector<ll> sign_blinded_message(ll alpha1,ll alpha2, ll d, ll n){
    //cout<<"alpha1 = "<<alpha1<<" and  alpha2 = "<<alpha2<<endl;
    ll b1 = getRandomPrime();
    ll b2 = getRandomPrime();

    ll t1 = modExp(alpha1, b1 * d, n);
    ll t2 = modExp(alpha2, b2 * d, n);

    vector<ll>ans={t1,t2,b1,b2};
    return ans;

}


ll unblind_message(vector<ll>blinded_msg, vector<ll>blinding_factors,ll e, ll n){

    ll t1=blinded_msg[0];
    ll t2=blinded_msg[1];
    ll b1=blinded_msg[2];
    ll b2=blinded_msg[3];

    ll r1=blinding_factors[0];
    ll r2=blinding_factors[1];
    ll a1=blinding_factors[2];
    ll a2=blinding_factors[3];

    ll d = gcd(a1 * b1, a2 * b2);
    ll w = (a2 * b2 / d) % (a1 * b1);
    ll t = (a1 * b1 / d) % (a2 * b2);

    w=5;

    t=(1-a1*b1*w)/(a2*b2);

   // cout<<a1*b1*w+a2*b2*t<<endl;
    /// Unblinding the signatures

    ll inv_r1 = mod_inverse(r1,n);
    ll inv_r2 = mod_inverse(r2,n);


    ll s1 = (t1 * inv_r1) % n;
    ll s2 = (t2 * inv_r2) % n;

    ll final_signature = (modExp(s1,w,n) +modExp(s2,t,n)) % n;
    return final_signature;

}


bool verify_signature(ll message, ll signature, ll public_key, ll n){
    ll e = public_key;
    return modExp(signature, e, n) == message;
}


int main() {

    primeGenerate();

    ll p = getRandomPrime();
    ll q = getRandomPrime();


    ll n = p * q;  // RSA modulus
    ll phi = (p - 1) * (q - 1);

    int e = 2;
        while (1)
        {
            if (gcd(e, phi) == 1)break;
            e++;
        }
        public_key = e;
        int d = 2;
        while (1)
        {
            if ((d * e) % phi == 1)break;
            d++;
        }
        private_key = d;
     cout<<"p = "<<p<<endl;
     cout<<"q = "<<q<<endl;
     cout<<"e = "<<e<<endl;
     cout<<"d = "<<d<<endl;
     cout<<"n = "<<n<<endl;


    // Message to be signed
    cout<<"Enter the message :";
    long long message;
    cin>>message;

    vector<ll>blindmsg=blind_message(message,e,n);

    ll alpha1=blindmsg[0];
    ll alpha2=blindmsg[1];
    cout<<"alpha1 = "<<alpha1<<" and  alpha2 = "<<alpha2<<endl;

    vector<ll>signedMsg=sign_blinded_message(alpha1,alpha2,d,n);
    ll t1=signedMsg[0];
    ll t2=signedMsg[1];

    cout<<"t1 = "<<t1<<" t2 = "<<t2<<endl;

    vector<ll>blinding_factor={blindmsg[2],blindmsg[3],blindmsg[4],blindmsg[5]};
    ll signature=unblind_message(signedMsg, blinding_factor,e,n);

    cout<<"signature = "<<signature<<endl;

    if(verify_signature(message,signature,e,n))cout<<"inValid signature"<<endl;
    else cout<<"valid signature"<<endl;
}
