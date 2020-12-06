import cs50

# Print mario pyramids
height = 0
while height < 1 or height > 8:
    height = cs50.get_int("Height: ")

for i in range(height):
    spaces = height - i - 1
    hashes = i + 1
    print((" " * spaces) + ("#" * hashes) + "  " + ("#" * hashes))