from sys import argv, exit
import csv


def main():
    # Check for presence of 2 command line args (First database.csv, second sequence.txt)
    if len(argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        exit(1)

    # Open csv database and load into memory
    # Collect all STRs into a list
    with open(argv[1]) as db:
        db = csv.DictReader(db)
        STRs = db.fieldnames[1:]

        # Open dna.txt file and load into memory
        f = open(argv[2], "r")
        dna = f.read().strip()

        # Compute the longest sequential run for each STR
        # Put STRs with corresponding counts into a dictionary
        person_str = {}
        for STR in STRs:
            person_str[STR] = STR_count(dna, STR)

        # Compare the STR counts against each row in the csv - identify person. Print name / Print No Match
        # For each row in the data, check if each STR count matches. If yes - print name.
        found = 0
        for row in db:
            name = row.pop("name")
            person_db = {}
            # Make new normal dict (not ordered) to compare with person_str dict.
            for i, j in row.items():
                person_db[i] = int(j)
            if person_db == person_str:
                print(name)
                found = 1
        # If no match in db
        if found != 1:
            print("No match")
    exit(0)


def STR_count(dna, STR):
    # Compute the longest sequential run for an STR
    # For each position in the sequence, compute how many times the STR repeats starting at that position.
    # For each position, keep checking successive substrings until the STR does not repeat
    ldna = len(dna) + 1
    lSTR = len(STR)
    max_STR = 0

    for i in range(ldna - lSTR):
        counter = 0
        if dna[i:i+lSTR] == STR:
            counter += 1
            for j in range(i + lSTR, ldna - lSTR, lSTR):
                if dna[j:j+lSTR] == STR:
                    counter += 1
                else:
                    break
        if counter > max_STR:
            max_STR = counter
    return(max_STR)


# Main funct - syntax to remember. Ensures main() is executed first but also allows function definition below main code
if __name__ == "__main__":
    main()