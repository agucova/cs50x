from cs50 import get_int
while True:
    height = get_int("Height: ")
    if height > 0 and height <= 8:
break
for i in range(height):
    s = i + 1
    k = height - s  # Whitespace counter
    for r in range(k):  # First half whitespace
        print(" ", end="")
    for m in range(s):  # First half hashes
        print("#", end="")
    print("  ", end="")  # Spacing
    for n in range(s):  # Second half hashes (no whitespace needed)
        print("#", end="")
    print("")  # Newline