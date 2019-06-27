from cs50 import get_string
from sys import argv

if len(argv) != 2:
    print("Usage: python bleep.py dictionary")
    exit(1)
# Enabled buffering, not completely sure if it increases or decreases performance.
cwords = set([line.strip() for line in open(argv[1], 'r', 1)])  # List comprehension
message = get_string("What message would you like to censor?\n")
for word in message.split(sep=" "):
    if word.lower() in cwords:
        for i in range(0, len(word)):
            print("*", end="")
        print(" ", end="")
    else:
        print(word, end=" ")
print("")