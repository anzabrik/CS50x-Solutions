from cs50 import get_string

text = get_string("Text: ")

letter_count = 0
sentence_count = 0

# Counting letters, sentences, and words
for letter in text:
    if letter.isalpha():
        letter_count += 1
    elif (letter == "!") or (letter == ".") or (letter == "?"):
        sentence_count += 1

words_list = text.split()
word_count = len(words_list)

# Counting L and S
L = letter_count / word_count * 100
S = sentence_count / word_count * 100
index = 0.0588 * L - 0.296 * S - 15.8
rounded_index = round(index)

# Printing results
if rounded_index >= 16:
    print("Grade 16+")
elif rounded_index < 1:
    print("Before Grade 1")
else:
    print("Grade ", rounded_index)