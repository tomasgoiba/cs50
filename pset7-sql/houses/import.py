from sys import argv, exit
import csv
import cs50

# Ensure correct usage
if len(argv) != 2:
    print("Usage: python import.py data.csv")
    exit(1)

# Set up database connection
db = cs50.SQL("sqlite:///students.db")

# Read data from CSV file into a SQL database
with open(argv[1], "r") as infile:
    reader = csv.DictReader(infile)
    for row in reader:  # Parse students' names
        full = row["name"].split()
        if len(full) == 3:
            first = full[0]
            middle = full[1]
            last = full[2]
        else:
            first = full[0]
            middle = None
            last = full[1]

        db.execute(
            "INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)",
            first, middle, last, row["house"], row["birth"]
        )