from sys import argv, exit
from cs50 import SQL


def main():
    # Check for presence of 1 command line arg (csv file to import)
    if len(argv) != 2:
        print("Usage: python roster.py housename")
        exit(1)
    roster(argv[1])


def roster(housename):
    # connect to existing db
    db = SQL("sqlite:///students.db")
    students = db.execute("SELECT first, middle, last, house, birth FROM students WHERE house = ? ORDER BY last, first", housename)
    for student in students:
        if student['middle'] == None:
            print(student['first'], student['last'] + ",", "born", student['birth'])
        else:
            print(student['first'], student['middle'], student['last'] + ",", "born", student['birth'])


# Main funct - syntax to remember. Ensures main() is executed first but also allows function definition below main code
if __name__ == "__main__":
    main()