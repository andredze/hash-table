import re

filename = input()

with open(filename, 'r', encoding='utf-8') as f:
    text = f.read()

words = re.findall(r'[^\W\d_]+', text)

with open(filename, 'w', encoding='utf-8') as f:
    text = f.write('\n'.join(words))
