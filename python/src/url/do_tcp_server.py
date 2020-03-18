#!/usr/bin/python3
# -*- coding=UTF-8 -*-
import sys
import os
import time
import socketserver


class MyTCPHadler(socketserver.BaseRequestHandler):
    """
    """

    def handle(self):
        print("Start A Connect")
        while True:
            self.data = self.request.recv(1024).strip()
            if self.data == b'':
                break
            print("%s write : %s" % (self.client_address[0], self.data))
            self.request.sendall(self.data.upper())
        print("Close A Connect")


if __name__ == '__main__':
    host, port = "127.0.0.1", 8888

    server = socketserver.TCPServer((host, port), MyTCPHadler)
    server.serve_forever()
