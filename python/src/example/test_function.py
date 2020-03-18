#!/usr/bin/python3


def changeVal(a):
    a = a+10
    return


def changeLst(lst):
    for a in lst:
        a += 1
    return


def changeLst2(lst):
    lst.append(1)
    return


a = 100
changeVal(a)
print(a)

lst = [1, 2, 3]
changeLst(lst)
print(lst)

changeLst2(lst)
print(lst)


def printme(str):
    print(str)
    return


def printmeWithNum(str, num=1):
    a = 0
    while a < num:
        print(str, end=' ')
        a += 1
    else:
        print()
    return


def printmeWithTuple(str, *varTuple):
    print(str)
    print(type(varTuple))
    for x in varTuple:
        print(x, end=' | ')
    else:
        print()
    return


def printmeWithDict(str, **varDict):
    print(str)
    print(type(varDict))
    print(varDict)
    return


def printLambda(str): return print(str)


printme('hello')
printme(str='hello')
printmeWithNum('hello')
printmeWithNum('hello', 3)
printmeWithNum(num=3, str='hello')
printmeWithTuple('hello')
printmeWithTuple('hello', 12, 199)
printmeWithDict('hello', a=1, b='books', c=[1, 2])
printLambda('hello')


def fact(num):
    if num == 1:
        return num
    return num*fact(num-1)


print(fact(1))

print(fact(10))

print(fact(15))


def fact2(n):
    return fact2_iter(n, 1)


def fact2_iter(num, product):
    if num == 1:
        return product
    return fact2_iter(num-1, num*product)


print(fact2(10))

def fact3(n):
    def fact3_impl(n):
        if n==1:
            return n
        return n*fact3_impl(n-1)
    return fact3_impl(n)

print(fact3(5))
        
