
from subprocess import *
from os import listdir
from os.path import isfile, join
import time





def gen_table(cmd, iter):
  lst= {}

  for i in range (iter):
    out =  Popen(["taskset","-c", "0", cmd, "1000", "-o"], stdout=PIPE).communicate()[0].decode("utf-8")
    out = str(round(float(out), 3))
    if out in lst.keys() :
      lst[out] = lst[out] + 1
    else:
      lst[out] = 1

  stat = sorted(lst.items())
  outputfile = cmd.replace("/", "").replace(".", "")

  f = open("data/"+outputfile+".data", "w")
  for temps, nombre in stat:
    f.write( temps + " " + str(nombre)+"\n")


gen_table("./expliciteFIFO/processus", 10000)
gen_table("./expliciteFIFO/thread", 10000)
gen_table("./implicteFIFO/processus", 10000)
gen_table("./implicteFIFO/thread", 10000)
