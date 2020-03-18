#!/usr/bin/python3

if True:
    msg='hello'
print(msg)

def fun1():
    global num
    num+=1
    return

num=0
print(num)
fun1()
print(num)

def fun2():
    a=10
    def funChild():
        nonlocal a
        a*=2
        return
    funChild()
    return a

print(fun2())

for i in range(0,10):
    t=i
print(t)

