#!/usr/bin/python3
# -*- coding=UTF-8 -*-

from urllib import request, parse

query_args = {'name': 'caibinbin', 'age': 19}
encode_args = parse.urlencode(query_args).encode('utf-8')

response=request.urlopen('http://localhost:8888', encode_args)
print("Http Response Info ", response.info())
data = response.read().decode('utf-8')
print("Http Response Data ", data)
