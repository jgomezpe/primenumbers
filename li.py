from decimal import *
from math import log as _flog

# Quantity of prime numbers up to 10^k (k position in the list)
# Taken from https://oeis.org/A006880 and 
# https://en.wikipedia.org/wiki/Prime_number_theorem
pi = [0, 4, 25, 168, 1229, 9592, 78498, 664579, 5761455, 50847534, 455052511, 4118054813, 37607912018, 346065536839, 3204941750802, 29844570422669, 279238341033925, 2623557157654233, 24739954287740860, 234057667276344607, 2220819602560918840, 21127269486018731928, 201467286689315906290, 1925320391606803968923, 18435599767349200867866, 176846309399143769411680, 1699246750872437141327603, 16352460426841680446427399, 157589269275973410412739598, 1520698109714272166094258063]
                                                                                                                                                                                                                                                                         
# Difference between li function and pi function
# Taken from https://en.wikipedia.org/wiki/Prime-counting_function
li_pi = [0, 2.2, 5.1, 10, 17, 38, 130, 339, 754, 1701, 3104, 11588, 38263, 108971, 314890, 1052619, 3214632, 7956589, 21949555, 99877775, 222744644, 597394254, 1932355208, 7250186216, 17146907278, 55160980939, 155891678121, 508666658006, 1427745660374, 4551193622464]

# li function table 10^m
li = [li_pi[i]+pi[i] for i in range(len(pi))]

# Computes li*(x) function as defined by Professor Jonatan Gomez in paper []
def li_star(x):
  x = Decimal(repr(x))
  lx = x.ln()
  n = int(lx)
  s = 1
  p = 1
  for k in range(1,n):
    p *= k/lx
    s += p
  return x*s/lx

k = 1
for i in range(1,len(li)):
  k *= 10
  ls = li_star(k)
  print(i, ls, li[i], ls-Decimal(repr(li[i])) ) 
