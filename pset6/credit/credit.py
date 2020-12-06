import cs50

cc = cs50.get_string("Number: ")

# Check cards validity
# Multiply every other digit by 2, starting with the number’s second-to-last digit, and then add those products’ !!digits!! together.
product = 0

# Sum the digits that weren’t multiplied by 2.
total_sum = 0

ticker = 0
for i in reversed(cc):
    if ticker == 0:
        total_sum += int(i)
        ticker = 1
    else:
        for j in str(int(i) * 2):
            product += int(j)
        ticker = 0

if (product + total_sum) % 10 != 0:
    print("INVALID")
elif cc[0:1] == "4":
    print("VISA")
elif cc[0:2] in ["34", "37"]:
    print("AMEX")
elif cc[0:2] in ["51", "52", "53", "54", "55"]:
    print("MASTERCARD")
else:
    quit
    