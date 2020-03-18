#!/usr/bin/python3
# -*- coding=UTF-8 -*-
import sys
import os
import time
import socket

if __name__ == '__main__':
    host, port = '127.0.0.1', 8888
    data = "Hello TCP"

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:

        sock.connect((host, port))
        for i in range(5):
            line = "Turn %d : %s\n" % (i, data)
            sock.sendall(bytes(line, 'utf-8'))
            received = str(sock.recv(1024), 'utf-8')

            print("Received : "+received)
