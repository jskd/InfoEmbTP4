
from subprocess import *
from os import listdir
from os.path import isfile, join
import time



lst= {}

for i in range (500):
  out =  Popen(["taskset","-c", "0", "./expliciteFIFO/processus", "1000", "-o"], stdout=PIPE).communicate()[0].decode("utf-8")
  out = round(float(out), 2)
  lst[ str(out) ] += 1

lst[out].sort()

print(lst[out])
