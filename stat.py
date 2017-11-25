
from subprocess import *
from os import listdir
from os.path import isfile, join
import time

def gen_table(cmd, iter, pres):
  lst= {}

  for i in range (iter):
    try:
      out =  Popen(["taskset","-c", "0", cmd, "100", "-o"], stdout=PIPE).communicate()[0].decode("utf-8")
      out = str(round(float(out), pres))
      if out in lst.keys() :
        lst[out] = lst[out] + 1
      else:
        lst[out] = 1
    except:
      pass

  stat = sorted(lst.items())
  outputfile = cmd.replace("/", "").replace(".", "")

  f = open("data/"+outputfile+".data", "w")
  for temps, nombre in stat:
    f.write( temps + " " + str(nombre)+"\n")

gen_table("./expliciteFIFO/processus", 1000, 2)
gen_table("./expliciteFIFO/thread", 1000, 2)
gen_table("./impliciteFIFO/processus", 1000, 2)
gen_table("./impliciteFIFO/thread", 1000, 2)
gen_table("./tempsContext/processus", 1000, 2)
gen_table("./tempsContext/thread", 1000, 2)
gen_table("./tempsExecution/processus", 1000, 1)
gen_table("./tempsExecution/thread", 1000, 1)
