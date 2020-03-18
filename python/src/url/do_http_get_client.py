#!/usr/bin/python3
# -*- coding=UTF-8 -*-

from urllib import request

response = request.urlopen('http://localhost:8888/?x=1&y=1')
print("Http Response Info ", response.info())
data = response.read().decode('utf-8')
print("Http Response Data ", data)
