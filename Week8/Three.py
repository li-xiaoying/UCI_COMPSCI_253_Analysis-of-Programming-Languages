import sys
import numpy as np
from collections import Counter

leet_map = {'A': '4', 'E': '3', 'I': '1', 'O': '0'}

# Takes as input an array of characters
characters = np.array([' '] + list(open(sys.argv[1]).read()) + [' '])

# Normalize
characters[~np.char.isalpha(characters)] = ' '
# Normalizes to UPPERCASE
characters = np.char.upper(characters)

# Replaces the vowels with their Leet counterparts
leet = lambda c: leet_map.get(c, c)
characters = np.vectorize(leet)(characters)

# Split the words by finding the indices of delimiter
sp = np.where(characters == ' ')
# A little trick: let's double each index, and then take pairs
sp2 = np.repeat(sp, 2)
# Get the pairs as a 2D matrix, skip the first and the last
w_ranges = np.reshape(sp2[1:-1], (-1, 2))
# Remove the indexing to the spaces themselves
w_ranges = w_ranges[np.where(w_ranges[:, 1] - w_ranges[:, 0] > 2)]

# Voila! Words are in between spaces, given as pairs of indices
words = list(map(lambda r: characters[r[0]:r[1]], w_ranges))
# Let's recode the characters as strings
swords = np.array(list(map(lambda w: ''.join(w).strip(), words)))

# Counts the 5 most frequently occurring 2-grams
two_grams = zip(swords, swords[1:])
count = Counter(two_grams)

for w, c in count.most_common(5):
  print(w[0], w[1], end = '')
  print(' - ', end = '')
  print(c)