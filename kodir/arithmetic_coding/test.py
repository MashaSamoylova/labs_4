from pwn import *
import hashlib
import random
import sys

sh = process('/bin/sh')

def test():
    for i in xrange(10000):
        i = random.randint(0,300000)
        m = hashlib.md5()
        string = m.hexdigest()
        cmd = "echo '" + string + "'|./arithmetic"
        sh.sendline(cmd)
        ans = sh.recvline(timeout=0.01)
        if ans != '0\n':
            print ans
            print cmd
            return


if __name__=="__main__":
    test()
