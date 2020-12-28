'''
An algorithm to find all possible words given a boggle board.
Note: <=2-letter wont be removed. This can be easily changed if wanted.

Usage example (Given default URL/file path):

<INPUT>:
a b c d
e f g h
i j k l
m n o qu

<OUTPUT>:
ink
ab
[...]
knife
fino
Total: 26
'''
import numpy as np
import pandas as pd

from itertools import product
from typing import List

# Board assumed to be square
BOARD_SIZE = 4
# May also use a file path:
URL = 'https://raw.githubusercontent.com/raun/Scrabble/master/words.txt'


def allprefixes(s: pd.Series):
    '''Returns the set of all prefixes of each word in s.'''
    res = set([''])
    while not s.empty:
        res |= set(s)
        s = s.apply(lambda s: s[:-1])
        s = s[s != '']
    return res


def clamp2range(val: int, stop: int = BOARD_SIZE):
    return max(0, min(BOARD_SIZE - 1, val))


def wordsfromidx(
        bboard: List[List[str]],
        row: int,
        col: int,
        pfxs: set,
        words: set,
        partial=None,
        beenat=None):
    res = set()
    if beenat is None:
        beenat = [(row, col)]
    if partial is None:
        partial = bboard[row][col]
    if partial in pfxs:
        if partial in words:
            res.add(partial)
        dests = [
            d for d in set([
                (clamp2range(row + dr), clamp2range(col + dc))
                for dr, dc in product([-1, 0, 1], [-1, 0, 1])])
            if d not in beenat]

        for rdst, cdst in dests:
            res |= wordsfromidx(bboard, rdst, cdst, pfxs, words,
                                partial + bboard[rdst][cdst], beenat + [(rdst, cdst)])
    return res


def main():
    df = pd.read_csv(URL, names=['words']).astype(str)
    df['words'] = df['words'].apply(str.lower)
    pfxs = allprefixes(df['words'])
    words = set(df['words'])

    bboard = [input().strip().lower().split() for _ in range(BOARD_SIZE)]
    foundwords = set()
    for r, c in product(range(BOARD_SIZE), range(BOARD_SIZE)):
        foundwords |= wordsfromidx(bboard, r, c, pfxs, words)
    print(*foundwords, sep='\n')
    print(f'Total: {len(foundwords)}')


if __name__ == '__main__':
    main()
