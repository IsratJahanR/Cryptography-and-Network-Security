#include <bits/stdc++.h>
#include <windows.h>
#include <wincrypt.h>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;

// Manually define CALG_SHA_256 if it's not defined by your SDK
#ifndef CALG_SHA_256
#define CALG_SHA_256 0x0000800c
#endif

// Function to convert byte array to hexadecimal string
string to_hex_string(BYTE* hash, int length) {
    const char hex_chars[] = "0123456789abcdef";
    string result;
    for (int i = 0; i < length; ++i) {
        result += hex_chars[(hash[i] >> 4) & 0xF];  // Extract the high nibble (first 4 bits)
        result += hex_chars[hash[i] & 0xF];   // Extract the low nibble (last 4 bits)

        cout<<result<<endl;
    }

    return result;
}


// Function to hash message using Windows Crypto API
string hash_message(const string& message, const string& algorithm) {
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    BYTE hash[32];  // SHA256 produces 32-byte hash
    DWORD hash_length = 0;
    ALG_ID alg_id;

    if (algorithm == "md5") {
        alg_id = CALG_MD5;
        hash_length = 16;  // MD5 produces 16-byte hash
    } else if (algorithm == "sha1") {
        alg_id = CALG_SHA1;
        hash_length = 20;  // SHA1 produces 20-byte hash
    } else if (algorithm == "sha256") {
        alg_id = CALG_SHA_256;
        hash_length = 32;  // SHA256 produces 32-byte hash
    } else {
        cout << "Unsupported hash algorithm. Use 'md5', 'sha1', or 'sha256'." << endl;
        return "";
    }

    // Acquire cryptographic provider
    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
        cout << "Error during CryptAcquireContext" << endl;
        return "";
    }

    // Create hash object
    if (!CryptCreateHash(hProv, alg_id, 0, 0, &hHash)) {
        cout << "Error during CryptCreateHash" << endl;
        CryptReleaseContext(hProv, 0);
        return "";
    }

    // Hash the message
    if (!CryptHashData(hHash, (BYTE*)message.c_str(), message.size(), 0)) {
        cout << "Error during CryptHashData" << endl;
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return "";
    }

    // Get the hash result
    if (!CryptGetHashParam(hHash, HP_HASHVAL, hash, &hash_length, 0)) {
        cout << "Error during CryptGetHashParam" << endl;
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return "";
    }

    // Cleanup
    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);

    // Return the hexadecimal string of the hash
    return to_hex_string(hash, hash_length);
}

int main() {
    string message = "Israt";

    string md5_hash = hash_message(message, "md5");
    string sha1_hash = hash_message(message, "sha1");
    string sha256_hash = hash_message(message, "sha256");

    if (!md5_hash.empty()) {
        cout << "MD5: " << md5_hash << endl;
    }
    if (!sha1_hash.empty()) {
        cout << "SHA-1: " << sha1_hash << endl;
    }
    if (!sha256_hash.empty()) {
        cout << "SHA-256: " << sha256_hash << endl;
    }

    return 0;
}
