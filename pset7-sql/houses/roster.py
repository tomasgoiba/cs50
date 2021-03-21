from sys import argv, exit
import csv
import cs50

# Ensure correct usage
if len(argv) != 2:
    print("Usage: python roster.py Gryffindor")
    exit(1)

# Set up database connection
db = cs50.SQL("sqlite:///students.db")

# Query students from a given house from SQL database
result = db.execute(
    "SELECT first, middle, last, birth FROM students WHERE house = ? ORDER BY last, first;",
    argv[1]
)

# Print out results
for row in result:
    if row["middle"] == None:
        print(f"{row['first']} {row['last']}, born {row['birth']}")
    else:
        print(f"{row['first']} {row['middle']} {row['last']}, born {row['birth']}")
