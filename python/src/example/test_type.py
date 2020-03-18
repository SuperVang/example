
#!/usr/bin/python3

# assignment one variable
counter = 100
miles = 1000.0
name = 'WhuBooks'

# assignment multi variable
a = b = c = 1.92
a, b, c = 1, 0.2, 'yuan'

# delete variable
del counter, miles, name

# type
a, b, c, d = 2, 0.14, True, 2+1j
print('type a(2) : ', type(a))
print('type b(0.14) : ', type(b))
print('type c(True) : ', type(c))
print('type d(2+1j) : ', type(d))

print('a(2) is int : ', isinstance(a, int))
print('b(0.14) is float : ', isinstance(b, float))
print('c(True) is bool : ', isinstance(c, bool))
print('d(2+1j) is complex : ', isinstance(d, complex))

print()

# operator
print('2+1 = ', 2+1)
print('2.1-10 = ', 2.1-10)
print('3*10.2 = ', 3*10.2)
print('18/7 = ', 18/7)
print('18//7 = ', 18//7)
print('18%7 = ', 18 % 7)
print('2**10 = ', 2**10)
print('60&13 = ', 60 & 13)
print('60|13 = ', 60 | 13)
print('60^13 = ', 60 ^ 13)
print('~60 = ', ~60)
print('60<<2 = ', 60 << 2)
print('60>>2 = ', 60 >> 2)

print()

# string
word = 'first line\nSecond line'
print(word)
multi = '''this is a
multiline string'''
print(multi)

print()

# index of string
str = 'WhuBooks'
print('str : ', str)
print('str[0:2] : ', str[0:2])
print('str[3:5] : ', str[3:5])
print('str[2:] : ', str[2:])
print('str[0:-1] : ', str[0:-1])
print('str[-3:-1] : ', str[-3:-1])
print('str[-1:-3] : ', str[-1:-3])
print('str[0:3]*2 : ', str[0:3]*2)
print('str[0:3]+str[3:5] : ', str[0:3]+str[3:5])
print('str[:] : ', str[:])
print('str[::2] : ', str[::2])
print('str[::-1] : ', str[::-1])
print('str[::-2] : ', str[::-2])

if 'Bo' in str:
    print('Bo in str')
else:
    print('Bo not in str')

print()

# list
list = ['WhuBooks', 100, 3.14, 'yuan', ['WhuBooks', 'Yuan']]
tinyList = [0, 1]
print('list : ', list)
print('list[0] : ', list[0])
print('list[1:3] : ', list[1:3])
print('list[0,1]+list[2,4] : ', list[0:1]+list[2:4])
print('list*2 : ', list*2)
print('list[:] : ', list[:])
print('list[::2] : ', list[::2])
print('list[::3] : ', list[::3])
print('list[::-1] : ', list[::-1])
print('list[::-2] : ', list[::-2])

if 100 in list:
    print('100 in list')
else:
    print('100 not in list')

list = [0, 1, 2, 3, 4, 5]
list[1] = 100
list[0:2] = [1, 2]
list[1:3] = []

print()

# tuple
tuple = ('WhuBooks', 100, 3.14, 'yuan', ['WhuBooks', 'Yuan'])
print('tuple : ', tuple)
print('tuple[0] : ', tuple[0])
print('tuple[1:3] : ', tuple[1:3])
print('tuple[0,1]+tuple[2,4] : ', tuple[0:1]+tuple[2:4])
print('tuple*2 : ', tuple*2)
print('tuple[:] : ', tuple[:])
print('tuple[::2] : ', tuple[::2])
print('tuple[::3] : ', tuple[::3])
print('tuple[::-1] : ', tuple[::-1])
print('tuple[::-2] : ', tuple[::-2])

if 100 in tuple:
    print('100 in tuple')
else:
    print('100 not in tuple')

tuple[4][0] = 'Cheng'
tuple = (1,)

print()

# set
s = {'WhuBooks', 100, 3.14, 'yuan', ('WhuBooks', 'Yuan'), 'WhuBooks'}
print('s : ', s)

if 100 in s:
    print('100 in s')
else:
    print('100 not in s')

a = set('abracadabra')
b = set('alacazam')
print('a : ', a)
print('b : ', b)
print('a-b : ', a-b)
print('a|b : ', a | b)
print('a&b : ', a & b)
print('a^b', a ^ b)

print()

# dictionary
d = {'one': 1, 2: 2}
d['code'] = 'python'
d['site'] = 'google'
print('d : ', d)
print('d[\'one\'] : ', d['one'])
print('d keys : ', d.keys())
print('d values : ', d.values())

d = dict([('code', 1), ('site', 2), ('pass', 3)])
print('d : ', d)
