from sys import argv, exit
import csv


def load_db(filename):
    """
    Read .csv file into a dictionary (DNA database)
    """
    with open(filename, "r") as infile:
        reader = csv.reader(infile)
        db_dict = {row[0]: row[1:] for row in reader}  # Key: name, value: list of STR counts
    return db_dict


def load_seq(filename):
    """
    Read .txt file as a string (DNA sequence)
    """
    with open(filename, "r") as infile:
        seq_list = infile.read()
    return seq_list


def find_reps(sub, string):
    """
    Find the largest run of consecutive repeats for an STR in the DNA sequence
    """
    max_reps = 0
    sub_length = len(sub)
    for i in range(len(string)):
        reps = 0  # Set 'reps' to 0 at the start of each iteration
        start = i
        while string[start:start+sub_length] == sub:  # On each iteration, if substring matches STR, keep checking successive substrings
            reps += 1
            start += sub_length
        max_reps = max(max_reps, reps)  # Keep track of the largest sequence of consecutive repeats
    return max_reps


def main():
    """
    Given a DNA database and a DNA sequence, look for a match in the database
    """
    # Ensure correct usage
    if len(argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        exit(1)

    # Load files into data structures
    dna_db = load_db(argv[1])
    dna_seq = load_seq(argv[2])

    # Extract counts for each STR
    counts = list()
    for seq in dna_db["name"]:
        reps = find_reps(seq, dna_seq)
        counts.append(str(reps))  # Cast int to str to enable comparison with dict value

    # Check if list of STR counts matches any of those in the database
    found = False
    for name in dna_db:
        if dna_db[name] == counts:
            found = True
            print(name)

    # Otherwise print 'No match'
    if found == False:
        print("No match")


main()