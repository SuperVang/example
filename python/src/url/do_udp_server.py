#!/usr/bin/python3
# -*- coding=UTF-8 -*-
import sys
import os
import time
import socketserver


class MyUDPHandler(socketserver.BaseRequestHandler):
    def handle(self):
        data = self.request[0].strip()
        sock = self.request[1]
        print("%s Write : %s" % (self.client_address[0], data))
        sock.sendto(data.upper(), self.client_address)


if __name__ == '__main__':
    host, port = '127.0.0.1', 9999
    server = socketserver.UDPServer((host, port), MyUDPHandler)
    server.serve_forever()
