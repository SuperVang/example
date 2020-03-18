#!/usr/bin/python3

from enum import Enum,unique

Month = Enum('Month', ('Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'))

for name,number in Month.__members__.items():
    print(name,'->',number,'->',number.value)

@unique
class Weekday(Enum):
    Sun = 0 # Sun的value被设定为0
    Mon = 1
    Tue = 2
    Wed = 3
    Thu = 4
    Fri = 5
    Sat = 6
    NNN='N'

for name,number in Weekday.__members__.items():
    print(name,'->',number,'->',number.value)

print(Month['Mar'])
print(Month.Jan)
print(Weekday(1))

e=Weekday('N')
print(e)