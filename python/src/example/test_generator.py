#!/usr/bin/python3

import sys

def fibonacci(n):
    a,b,counter=0,1,0
    while True:
        if counter>n:
            return
        yield a
        a,b=b,a+b
        counter+=1

f=fibonacci(10)
print('f is : ',f)
for x in f:
    print(x,end=' ')
else:
    print()

def get():
    m = 0
    n = 2
    l = ['s',1,3]
    k = {1:1,2:2}
    p = ('2','s','t')
    while True:
        m += 1
        yield m
        yield m ,n ,l ,k ,p
        
it = get()
print(next(it)) #1
print(next(it)) #(1, 2, ['s', 1, 3], {1: 1, 2: 2}, ('2', 's', 't'))

print(next(it)) #2
print(type(next(it))) #<class 'tuple'>
