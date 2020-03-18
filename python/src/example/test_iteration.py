#!/usr/bin/python3

import sys

lst=[1,2,3,4]
itr=iter(lst)

print(itr)
print(next(itr))

itr=iter(lst)
for x in itr:
    print(x,end=' ')
else:
    print()


itr=iter(lst)
while True:
    try:
        print(next(itr),end=' ')
    except StopIteration:
        print()
        break


class MyNum:
    def __iter__(self):
        self.a=1
        return self
    
    def __next__(self):
        if self.a<=20:
            x=self.a
            self.a+=1
            return x
        else:
            raise StopIteration

myNum=MyNum()
myIter=iter(myNum)

print(myIter)
print(next(myIter))

myIter=iter(myNum)
for x in myIter:
    print(x,end=' ')
else:
    print()

