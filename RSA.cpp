#include <bits/stdc++.h>
using namespace std;

class RSA {
private:
    set<int> prime;
    int public_key;
    int private_key;
    int n;

    void primeGenerate()
    {
        vector<bool> seive(500, true);
        seive[0] = false;
        seive[1] = false;
        for (int i = 2; i < 500; i++) {
            for (int j = i * 2; j < 500; j += i) {
                seive[j] = false;
            }
        }
        for (int i = 0; i < seive.size(); i++) {
            if (seive[i])
                prime.insert(i);
        }
    }

    int getRandomPrime() {
        int k = rand() % prime.size();
        auto it = prime.begin();
        while (k--)
            it++;
        int ret = *it;
        prime.erase(it);
        return ret;
    }

    int gcd(int a, int b) {
        if (b == 0) return a;
        return gcd(b, a % b);
    }

public:

    void setkeys() {
        int p = getRandomPrime();
        int q = getRandomPrime();
        n = p * q;
        int fi = (p- 1) * (q - 1);

        int e = 2;
        while (1)
        {
            if (gcd(e, fi) == 1)break;
            e++;
        }
        public_key = e;
        int d = 2;
        while (1) {
            if ((d * e) % fi == 1)break;
            d++;
        }
        private_key = d;
    }
    RSA() {
        primeGenerate();
        setkeys();
    }

    long long int mod_pow(int txt,int key)
    {

        long long int ans = 1;
        while (key--)
        {
            ans *= txt;
            ans %= n;
        }
        return ans;
    }


    vector<int> encodeMessage(const string& message)
    {
        int e = public_key;
        vector<int> encoded;
        for (auto& letter : message)
            encoded.push_back(mod_pow((int)letter,e));
        return encoded;
    }

    string decodeMessage(const vector<int>& encoded) {
        int d = private_key;
        string decoded;
        for (auto& num : encoded)
            decoded += mod_pow(num,d);
        return decoded;
    }

    void displayKeys() {
        cout << "Public Key: " << public_key << "\nPrivate Key: " << private_key << "\nn: " << n << endl;
    }
};

int main() {

    RSA rsa;

    string message = "Test Message";
    cin>>message;
    cout << "Original message : " << message << endl;

    //rsa. displayKeys();

    vector<int> encodedMessage = rsa.encodeMessage(message);
    cout << "\nEncoded message  : ";
    for (auto& p : encodedMessage)cout << p<<" ";

    cout<<endl;

    cout << "Decoded message  : ";
    string decodedMessage = rsa.decodeMessage(encodedMessage);
    cout << decodedMessage << endl;

    return 0;
}
