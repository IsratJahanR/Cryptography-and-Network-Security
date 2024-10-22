#include<bits/stdc++.h>

using namespace std;

#define ull unsigned long long


ull gcd(ull a, ull b)
{
    if (b == 0) return a;
    return gcd(b, a % b);
}


class Sign
{
private:
    ull private_key;
    set<int> prime;

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
    ull mod_pow(ull base, ull exp, ull mod)
    {
        ull result = 1;
        base = base % mod;
        while (exp > 0)
        {
            if (exp % 2 == 1)
            {
                result = (result * base) % mod;
            }
            exp = exp >> 1;
            base = (base * base) % mod;
        }
        return result;
    }
public:

    ull n, public_key;
    Sign()
    {
        primeGenerate();
        generate_rsa_keys();
    }
    void generate_rsa_keys()
    {
        int p = getRandomPrime();
        int q = getRandomPrime();

        n = p * q; // Ensure proper type
        ull phi = (p - 1) * (q - 1);
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
    }
    ull sign_blinded_message(ull blinded_message)
    {
        return mod_pow(blinded_message, private_key, n);
    }
    bool verify_signature(ull signature, ull message)
    {
        ull verified_message = mod_pow(signature, public_key, n);

        cout<<"varifying message = "<<verified_message<<endl;

        return verified_message == message;
    }
    void display_keys()
    {
        cout << "Public key (n, e): (" << n << ", " << public_key << ")" << endl;
        cout << "Private key (d): " << private_key << endl;
    }


};

class Vote
{
    private:
    ull mod_pow(ull base, ull exp, ull mod)
    {
        ull result = 1;
        base = base % mod;
        while (exp > 0)
        {
            if (exp % 2 == 1)
            {
                result = (result * base) % mod;
            }
            exp = exp >> 1;
            base = (base * base) % mod;
        }
        return result;
    }
    ull mod_inverse(ull a, ull mod)
    {
        if(gcd(a, mod) > 1)return -1;

        for (int x = 1; x < mod; x++)
            if (((a % mod) * (x % mod)) % mod == 1)return x;
    }
public:
    ull message;
    ull r;

    Vote(ull msg){
        message=msg;
    }
    ull blind_message(ull e, ull n)
    {
        r = rand() % n;
        while (gcd(r, n) != 1 || r == 0)
        {
            r = rand() % n;
        }
        return (message * mod_pow(r, e, n)) % n;
    }

    ull unblind_message(ull signed_blinded_message, ull n)
    {
        ull r_inv = mod_inverse(r, n);
        return (signed_blinded_message * r_inv) % n;
    }


};


int main()
{


    /// 1. Initializing Phase
    Sign signer;
    signer.display_keys();


    ull message;
    cout << "Enter a message (as an integer): ";
    cin >> message;

    Vote voter(message);

    /// 2. Blinding Phase
    ull blinded_message = voter.blind_message(signer.public_key, signer.n);
    cout << "Blinding factor r: " << voter.r << endl;
    cout << "Blinded message: " << blinded_message << endl;

    /// 3. Signing Phase
    ull signed_blinded_message = signer.sign_blinded_message(blinded_message);
    cout << "Signed blinded message: " << signed_blinded_message << endl;

    /// 4. Unblinding Phase
    ull signature = voter.unblind_message(signed_blinded_message, signer.n);
    cout << "Unblinded signature: " << signature << endl;

    /// 5. Verifying Phase
    if (signer.verify_signature(signature, message))
    {
        cout << "Signature is valid." << endl;
    }
    else
    {
        cout << "Signature is invalid." << endl;
    }

    return 0;
}
