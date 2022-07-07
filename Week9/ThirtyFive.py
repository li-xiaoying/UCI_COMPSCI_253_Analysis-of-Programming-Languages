from keras.models import Sequential
from keras.layers import Dense
import numpy as np
import sys
import string

leet_map = {'a': '@',
            'b': '8',
            'c': '<',
            'e': '3',
            'f': '7',
            'g': '6',
            'h': '4',
            'i': '!',
            'j': '9',
            'l': '1',
            'o': '0',
            's': '$',
            't': '+',
            'x': '%',
            'z': '5'}

characters = string.printable
char_indices = dict((c, i) for i, c in enumerate(characters))
indices_char = dict((i, c) for i, c in enumerate(characters))

INPUT_VOCAB_SIZE = len(characters)


# Exercise 35.3: Another function
def leet(c):
    c = c.lower()
    if leet_map.get(c):
        return leet_map[c]
    else:
        return c


def encode_one_hot(line):
    x = np.zeros((len(line), INPUT_VOCAB_SIZE))
    for i, c in enumerate(line):
        if c in characters:
            index = char_indices[c]
        else:
            index = char_indices[' ']
        x[i][index] = 1
    return x


def decode_one_hot(x):
    s = []
    for onehot in x:
        one_index = np.argmax(onehot)
        s.append(indices_char[one_index])
    return ''.join(s)


def normalization_layer_set_weights(n_layer):
    wb = []
    w = np.zeros((INPUT_VOCAB_SIZE, INPUT_VOCAB_SIZE), dtype=np.float32)
    b = np.zeros((INPUT_VOCAB_SIZE), dtype=np.float32)
    # Let lower case letters go through
    for c in string.ascii_lowercase:
        i = char_indices[c]
        il = char_indices[leet(c)]
        w[i, il] = 1
    # Map capitals to lower case
    for c in string.ascii_uppercase:
        i = char_indices[c]
        il = char_indices[leet(c)]
        w[i, il] = 1
    # Map all non-letters to space
    sp_idx = char_indices[' ']
    for c in [c for c in list(string.printable) if c not in list(string.ascii_letters)]:
        i = char_indices[c]
        w[i, sp_idx] = 1

    wb.append(w)
    wb.append(b)
    n_layer.set_weights(wb)
    return n_layer


def build_model():
    # Normalize characters using a dense layer
    model = Sequential()
    dense_layer = Dense(INPUT_VOCAB_SIZE,
                        input_shape=(INPUT_VOCAB_SIZE,),
                        activation='softmax')
    model.add(dense_layer)
    return model


model = build_model()
model.summary()
normalization_layer_set_weights(model.layers[0])

with open(sys.argv[1]) as f:
    for line in f:
        if line.isspace(): continue
        batch = encode_one_hot(line)
        preds = model.predict(batch)
        normal = decode_one_hot(preds)
        print(normal)
