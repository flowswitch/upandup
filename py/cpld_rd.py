#!/usr/bin/python3
#from sys import argv, exit

#if len(argv)!=3:
#    exit("Usage: "+argv[0]+" <addr> <value>")

from up.up1024p import *

up = UP1024P()

#print("Loading firmware...")
#up.load_fx2('up256.bin', 'bin')
#up.load_fx2('../FX2/build/gpio256.ihx', 'ihex')
#print("OK")

up.get_status()

while True:
    addr = input("Enter addr:")
    if addr=='q':
        break
    addr = int(addr, 0)
    print("[%02X] = %02X" % (addr, up.cpld_read(addr)))
