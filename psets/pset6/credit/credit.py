from cs50 import get_int

cc = get_int("Number: ")
scc = str(cc)
if (len(scc) <= 13 and len(scc) >= 16):
    print("INVALID")
    exit()

scc = str(cc)
osum = 0
other = []
esum = 0
for i, c in enumerate(scc[::-1]):  # Taken from: https://stackoverflow.com/questions/931092/reverse-a-string-in-python; Reverse sum of every other number
    if i % 2 != 0:
        other.append(c)
    else:
        esum += int(c)

for j in range(0, len(other)):  # Multiply every one by two
    other[j] = int(other[j]) * 2

for j in range(0, len(other)):  # Sum the digits of those products
    if other[j] < 9:
        osum += other[j]
    elif other[j] >= 10:
        osum += int(str(other[j])[0]) + int(str(other[j])[1])

x = osum + esum
if x % 10 == 0:  # Check if valid
    header = int(scc[0] + scc[1])
    if (header == 34 or header == 37):
        print("AMEX")
    elif(scc[0] == "4"):
        print("VISA")
    else:
        print("MASTERCARD")
else:
    print("INVALID")