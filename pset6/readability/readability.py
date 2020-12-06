import cs50

text = cs50.get_string("Text: ")

# L = count avg number of letters per 100 words
# S = count avg number of sentences per 100 words
# Coleman-Liau index = 0.0588 * L - 0.296 * S - 15.8
# Print out Grade X, min if index < 1: "Before Grade 1", max if index > 16 "Grade 16+"

letters = 0
spaces = 0
sentences = 0

for i in text:
    # count letters
    if i.isalpha():
        letters += 1
    # count spaces
    elif i.isspace():
        spaces += 1
    # count sentences
    elif i in [".", "!", "?"]:
        sentences += 1

words = spaces + 1
L = letters * 100 / words
S = sentences * 100 / words
clindex = 0.0588 * L - 0.296 * S - 15.8

if (clindex > 15):
    print("Grade 16+")
elif (clindex < 1):
    print("Before Grade 1")
else:
    print("Grade {}".format(round(clindex)))