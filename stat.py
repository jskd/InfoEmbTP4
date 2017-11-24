
from subprocess import *
from os import listdir
from os.path import isfile, join
import time


out =  Popen(["taskset","-c", "0", "./expliciteFIFO/processus", "10000", "-o"], stdout=PIPE).communicate()[0].decode("utf-8")

print(out)
