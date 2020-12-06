from sys import argv, exit
import csv
from cs50 import SQL


def main():
    # Check for presence of 1 command line arg (csv file to import)
    if len(argv) != 2:
        print("Usage: python import.py data.csv")
        exit(1)
    import_file(argv[1])


def import_file(file):
    # connect to existing db
    db = SQL("sqlite:///students.db")
    # Open csv file
    with open(file) as csvfile:
        reader = csv.DictReader(csvfile)

        # Parse csv and insert into db
        for row in reader:
            name = row['name']
            name = parse_name(name)
            if name['middle'] == 'NULL':
                db.execute("INSERT INTO students (first, last, house, birth) VALUES (?, ?, ?, ?)",
                           name['first'], name['last'], row['house'], row['birth'])
            else:
                db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)",
                           name['first'], name['middle'], name['last'], row['house'], row['birth'])


def parse_name(name):
    name = name.split()
    if len(name) == 2:
        return {
            "first": name[0],
            "middle": "NULL",
            "last": name[1]
        }
    else:
        return {
            "first": name[0],
            "middle": name[1],
            "last": name[2]
        }


# Main funct - syntax to remember. Ensures main() is executed first but also allows function definition below main code
if __name__ == "__main__":
    main()