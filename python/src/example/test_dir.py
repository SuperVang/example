#!/usr/bin/python3

import sys,os

print("sys path[0] ",sys.path[0])
print("os cwd ",os.getcwd())
print("os abspath __file__ ",os.path.abspath(__file__))
print("os dirname __file__ ",os.path.dirname(__file__))
