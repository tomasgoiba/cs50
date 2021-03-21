from cs50 import get_string


# Propmt user for text fragment
text = get_string("Text: ")

# Calculate number of letters, words and sentences in fragment
letters = sum(c.isalpha() for c in text)  # Returns sum of alphabetic characters in fragment
words = len(text.split())  # Splits fragment by whitespace and returns a list, then calculates its length
sentences = text.count(".") + text.count("?") + text.count("!")  # Counts instances of '.', '!' and '?', and returns total

# Calculate Coleman-Liau index for text fragment
letter_average = letters * 100 / words
sentence_average = sentences * 100 / words
index = round(letter_average * 0.0588 - sentence_average * 0.296 - 15.8)

# Print out grade level for text, according to Coleman-Liau formula
if index >= 16:
    print("Grade 16+")
elif index < 1:
    print("Before Grade 1")
else:
    print(f"Grade {index}")