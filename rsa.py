import random


class RSA:
    def __init__(self, p, q):
        self.p = p
        self.q = q
        self.n = p * q
        if self.n < 256:
            raise ValueError(
                f"Invalid n = {self.n}. n must be > 255 so all ASCII characters can be encoded.")
        self.phi = (p - 1) * (q - 1)
        self.e, self.d = self._generate_keys()

    # --- Euclidean GCD ---
    def _gcd(self, a, b):
        while b != 0:
            a, b = b, a % b
        return a

    # --- Extended Euclidean Algorithm ---
    def _extended_gcd(self, a, b):
        if b == 0:
            return a, 1, 0
        gcd_, x1, y1 = self._extended_gcd(b, a % b)
        x, y = y1, x1 - (a // b) * y1
        return gcd_, x, y

    # --- Modular Inverse ---
    def _mod_inverse(self, e, phi):
        gcd_, x, _ = self._extended_gcd(e, phi)
        if gcd_ != 1:
            raise Exception("Modular inverse does not exist.")
        return x % phi

    # --- Key Generation ---
    def _generate_keys(self):
        e = random.randrange(2, self.phi)
        while self._gcd(e, self.phi) != 1:
            e = random.randrange(2, self.phi)
        d = self._mod_inverse(e, self.phi)
        return e, d

    # --- Public / Private Keys ---
    def get_public_key(self):
        return (self.e, self.n)

    def get_private_key(self):
        return (self.d, self.n)

    # --- Encryption ---
    def encrypt(self, plaintext):
        e, n = self.get_public_key()
        return [pow(ord(ch), e, n) for ch in plaintext]

    # --- Decryption ---
    def decrypt(self, ciphertext):
        d, n = self.get_private_key()
        return ''.join(chr(pow(num, d, n)) for num in ciphertext)


# --- Helper Functions ---
def is_prime(num):
    if num <= 1:
        return False
    for i in range(2, int(num ** 0.5) + 1):
        if num % i == 0:
            return False
    return True


def get_primes():
    while True:
        try:
            p = int(input("Enter first prime number (p): "))
            q = int(input("Enter second prime number (q): "))

            # Check primality
            if not is_prime(p):
                print(f" {p} is not a prime number.")
            if not is_prime(q):
                print(f" {q} is not a prime number.")

            if not (is_prime(p) and is_prime(q)):
                print(" Please enter two valid prime numbers.\n")
                continue

            # Ensure different primes
            if p == q:
                print(" p and q must be DIFFERENT primes.\n")
                continue

            # Ensure n large enough
            n = p * q
            if n < 256:
                print(f"n = p × q = {n} is too small for text encryption.")
                print("   Choose larger primes so that n > 255.")
                print("   Example: try p ≥ 17 and q ≥ 19, which gives n = 323.\n")
                continue

            return p, q

        except ValueError:
            print("Invalid input. Please enter integer values only.\n")


# --- Main Program ---
if __name__ == "__main__":
    print("=== RSA Algorithm (OOP Implementation with Detailed Validation) ===\n")

    p, q = get_primes()
    rsa = RSA(p, q)

    print("\nValid primes accepted!")
    print("Public Key :", rsa.get_public_key())
    print("Private Key:", rsa.get_private_key())

    message = input("\nEnter a message to encrypt: ")
    encrypted = rsa.encrypt(message)
    print(" Encrypted Message:", encrypted)

    decrypted = rsa.decrypt(encrypted)
    print(" Decrypted Message:", decrypted)
