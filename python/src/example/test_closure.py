#!/usr/bin/python3


def lazy_sum(args):
    def sum():
        ax = 0
        for n in args:
            ax = ax+n
        return ax
    return sum


f1 = lazy_sum([1, 3, 5, 7, 9])
print(f1)
print(f1())
f2 = lazy_sum([1, 3, 5, 7, 9])
print(f2())
print(f1 == f2)

del f1, f2


def count():
    fs = []
    for i in range(1, 4):
        def f():
            return i*i
        fs.append(f)
    return fs


f = count()
print(f)
print(f[0](), f[1](), f[2]())


def build(x, y):
    return lambda: x*x+y*y


f = build(3, 4)
print(f)
print(f())

print(build.__name__)
print(f.__name__)
