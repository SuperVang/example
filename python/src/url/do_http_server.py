#!/usr/bin/python3
# -*- coding=UTF-8 -*-
import sys
import os
import time
import socketserver
import json
import http.server
import urllib.parse
import cgi


class MyRequestHandler(http.server.BaseHTTPRequestHandler):
    def do_GET(self):
        if '?' not in self.path:
            return
        query = urllib.parse.unquote(self.path.split('?', 1)[1])
        params = urllib.parse.parse_qs(query)
        print(params)
        data = dict()
        for key, val in params.items():
            data[key] = "Server-" + val[0]

        self.send_response(200)
        self.send_header('Content-type',
                         'application/json; charset=utf-8')
        self.end_headers()
        self.wfile.write(json.dumps(data).encode())

    def do_POST(self):
        form = cgi.FieldStorage(fp=self.rfile, headers=self.headers,
                                environ={'REQUEST_METHOD': 'POST',
                                         'CONTENT_TYPE': self.headers['Content-Type'], })
        for field in form.keys():
            print(field, form[field].value)

        self.send_response(200)
        self.send_header('Content-Type',
                         'application/json; charset=utf-8')
        self.end_headers()
        self.wfile.write(json.dumps({'hello': 'world'}).encode(encoding='utf-8'))


if __name__ == '__main__':
    host, port = '127.0.0.1', 8888
    server = http.server.HTTPServer((host, port), MyRequestHandler)
    print('Start Http Service...')
    server.serve_forever()
