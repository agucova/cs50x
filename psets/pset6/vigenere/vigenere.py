from cs50 import get_string
from sys import argv

if len(argv) != 2:
    print("Usage: python vigenere.py k")
    exit(1)

key = argv[1]
if not key.isalpha():
    print("Usage: python vigenere.py k")
    exit(1)
plaintext = get_string("plaintext: ")

keyi = []
for c in key:  # Create a transposition array (list) from the key
    keyi.append(ord(c.lower()) - 97)

ciphertext = ""
lkey = len(key)

r = 0  # Index in key

for d in plaintext:
    if d.isalpha():  # If it-s alpha
        if d.isupper():
            ad = ord(d) - 65  # Where is d in the abecedary
        else:
            ad = ord(d) - 97
        if (ad + keyi[r] <= 25):  # Does it not wrap?
            ciphertext += chr(ord(d) + keyi[r])
        else:
            excd = (ad + keyi[r]) - 26  # How much does it exceed from 26
            if d.isupper():
                ciphertext += chr(65 + excd)
            else:
                ciphertext += chr(97 + excd)
        r += 1
    else:  # If not alpha
        ciphertext += d
    if (r == lkey):
        r = 0
print("ciphertext:", ciphertext)
print(keyi)