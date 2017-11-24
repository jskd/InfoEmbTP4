
from subprocess import *
from os import listdir
from os.path import isfile, join
import time



lst= {}

for i in range (10000):
  out =  Popen(["taskset","-c", "0", "./expliciteFIFO/processus", "1000", "-o"], stdout=PIPE).communicate()[0].decode("utf-8")
  out = str(round(float(out), 3)) + "ms"
  if out in lst.keys() :
    lst[out] = lst[out] + 1
  else:
    lst[out] = 1

stat = sorted(lst.items())

print(stat)
