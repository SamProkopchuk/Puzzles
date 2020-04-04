"""
This solution is based upon code from the following series:
https://www.coursera.org/specializations/discrete-mathematics

Creates an nxn board of n queens where none are attacking each other.
Apparently it's always possible to construct such a board for n>=4
"""


def can_be_extended_to_solution(perm):
    i = len(perm) - 1
    for j in range(i):
        if i - j == abs(perm[i] - perm[j]):
            return False
    return True


def extend(perm, n):
    if len(perm) == n:
        for row in range(n):
            for col in range(n):
                if col == perm[row]:
                    print("Q|", end="")
                else:
                    print("_|", end="")
            print()
        exit()

    for k in range(n):
        if k not in perm:
            perm.append(k)

            if can_be_extended_to_solution(perm):
                extend(perm, n)

            perm.pop()


extend(perm=[], n=int(input("size of board: ")))
