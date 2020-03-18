#!/usr/bin/python3

from functools import reduce


def f(x):
    return x*x


r = map(f, [1, 2, 3, 4, 5])
print(list(r))


def add(x, y):
    return x+y


r = reduce(add, [1, 2, 3, 4, 5])
print(r)
