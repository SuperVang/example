#!/usr/bin/python3
# -*- coding: UTF-8 -*-

import sys
import keyword

# 系统路径
print('Python Path : ', sys.path)

#
print("Python Keyword : ")
print(keyword.kwlist)

# this is a comment

'''
this is a block comment
'''

"""
this is also a block comment
"""

for b in keyword.kwlist:
    print(b, end='\t')
print()
if keyword.kwlist.count('or') == 1:
    print("r")
else:
    print('w')

input('please input enter...')
