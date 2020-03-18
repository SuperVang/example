#!/usr/bin/python3
# -*- coding=UTF-8 -*-
import sys
import os
import time
import socket

if __name__ == '__main__':
    host, port = '127.0.0.1', 9999
    data = "Hello udp"

    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:

        for i in range(5):
            line = "Turn %d : %s\n" % (i, data)
            sock.sendto(bytes(line, 'utf-8'),(host,port))
            received = str(sock.recv(1024), 'utf-8')

            print("Received : "+received)
