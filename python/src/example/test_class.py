#!/usr/bin/python3


class Student(object):
    pass


a = Student()
print(a)
print(Student)
a.name = 'books'
print(a.name)

del Student


class Student(object):
    father = 'cai'

    def __init__(self, name, score):
        self.name = name
        self.score = score

    def print(self):
        print(self.name, self.score)


a = Student('books', 20)
print(a.name, a.score)

a.print()

print(dir(a))

print(hasattr(a, 'name'))
print(hasattr(a, 'x'))

print(hasattr(a, 'print'))
fn = getattr(a, 'print')
print(fn)
fn()

a.print = 'aaa'
print(hasattr(a, 'print'))
fn = getattr(a, 'print')
print(fn)

print(a.father)


class Person(object):
    country='China'

    def __init__(self):
        self.name='Cai'
    
    def set_country(self,coun):
        Person.country=coun

    def print_country(self):
        print(self.country)

p1=Person()
p2=Person()
p1.print_country()
p2.print_country()

p1.set_country('Canada')
p1.print_country()
p2.print_country()
