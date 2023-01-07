import numpy as np
# This program is part of the paper entitled 'The Prime Number Theorem and Primorial Numbers' written by Professor Jonatan Gomez

# Euler's constant
gamma = 0.5772156649

# e^gamma
e_gamma = 1.78107241799

# A list of numbers marked as prime numbers or not 
P = []

# Quantity of prime numbers up to 10^k (k position in the list)
# Taken from https://oeis.org/A006880 and 
# https://en.wikipedia.org/wiki/Prime_number_theorem
pi = [0, 4, 25, 168, 1229, 9592, 78498, 664579, 5761455, 50847534, 455052511, 4118054813, 37607912018, 346065536839, 3204941750802, 29844570422669, 279238341033925, 2623557157654233, 24739954287740860, 234047667276344607, 2220819602560918840, 21127269486018731928, 201467286689315906290, 1925320391606803968923, 18435599767349200867866, 176846309399143769411680]

# Erathostenes sieve (compute prime numbers up to n+1)
# Due to pure Python restrictions not suitable for n>100000
# Use the C++ version for computing prime numbers up to 10^10
def primes(n):
  global P
  P = [True] * (n+1)
  P[0] = P[1] = False
  sn = int(n ** 0.5)
  for i in range(2,sn):
    for k in range(i*i, n+1, i):
      P[k] = False
  return P

#Show prime numbers in a range
def show(start, end):
  global P
  print('PRIME NUMBERS BETWEEN', start, 'and', end)
  for i in range(start, end+1):
    if(P[i]): print(i,end=' ')
  print()

# Get the first n prime numbers
def primelist(n):
  global P
  L = []
  k = 2
  for i in range(n):
    while(not P[k]): k +=1
    L.append(k)
    k += 1
  return L

# Computes the product of numbers in a list
def listproduct(L):
  s = 1
  for k in L:
    s *= k
  return s

# Computes the sum of the natural logarithm of the numbers in a list
# i.e. the natural logarithm of the product of the numbers in a list
def listlogsum(L):
  s = 0
  for k in L:
    s += np.log(k)
  return s  

# Computes the n-th primorial number
def primorial(n):
  return listproduct(primelist(n))

# Computes the n-th totatives, see Corollary 10
def tot(n):
  l = primelist(n)
  for i in range(len(l)):
    l[i] -= 1
  return listproduct(l)
  
# Gets the n-th prime number (limited to primes in the list P, of course)
def p(n):
  global P
  k = 2
  for i in range(n):
    while(not P[k]): k +=1
    if(i<n-1):
      k += 1
  return k

# The r-th multiplicative fraction of number y
def fraction(y, r):
  return 1 + (y-1)*r
  
# Represents a number in terms of primorials, see Definition 18
# i.e., x = #(n'(x)) * s'(x)
# returns n'(x), #(n'(x)), r'(x), s'(x)
def represent(x):
  global P
  n = 0
  pn = 1
  pm = primorial(n+1)
  while(x>=pm):
    n += 1
    pn = pm
    pm = primorial(n+1)
  r = (x-pn)/(pm-pn)
  s = fraction(p(n+1),r)
  return n, pn, r, s

# Computing natural logarithm using primorials approach, see Equation 4.4
def log(x):
  n, primn, r, s = represent(x)
  log_n = listlogsum(primelist(n))
  return np.log(s) + log_n

# log* function, see see Corollary 23 item 5
def logstar(x):
  n, primn, r, s = represent(x)
  sstar = fraction(p(n),r)
  log_n1 = listlogsum(primelist(n-1))
  return np.log(sstar) + log_n1

# log<> function, see Corollary 23 item 6
def logdiamond(x):
  n, primn, r, s = represent(x)
  sstar = fraction(p(n),r)
  log_n1 = listlogsum(primelist(n-1))
  a = n + r
  return np.log(a) + np.log(sstar) + log_n1

# Prime number theorem approximation, computes natural logarithm with Equation 4.4 since numpy.log cannot deal with numbers higher than 10^20
def pnt(x):
  return x/log(x) 

# Prime number theorem approximation for powers of 10
def pnt_10(exp):
  return (10**exp)/(exp*log(10))

# Prime number theorem using log*
def pntstar(x):
  return x/logstar(x)

# Prime number theorem using log<>
def pntdiamond(x):
  return x/logdiamond(x)

# n-totatives up to x, see Equation 5.1
def totstar(x):
  n, primn, r, s = represent(x)
  return tot(n)*fraction(p(n+1),r)

def hcirc(x):
  n, primn, r, s = represent(x)
  pn = p(n)
  pn1 = p(n+1)
  y = p(n)*fraction(pn1/pn,r)
  return totstar(x)*e_gamma*log(y)

# Prime number theorem using hcirc in both x and log(x)
def pnthcirc(x):
  y = hcirc(x)
  return y/log(y)

# Prime number theorem using hcirc just in the log(x)
def pnthcirc2(x):
  y = hcirc(x)
  return x/log(y)

# Prime number theorem using hcirc just in the log(x)
def pnthcircdiamond(x):
  return hcirc(x)/logdiamond(x)

# Generates Table 2 Asymptotic behavior (latex)
def latex_pnt( options ):
  m = len(options)
  print('% Generating Asymptotic behavior table in latex')
  print('\\begin{table}[htbp]')
  print('\\centering')
  print('\\begin{tabular}{|',end='')
  for i in range(1,m):
    print('r|', end='')
  print('}')

  print('\\cline{2-',m-1,'}', sep='')
  print('\\multicolumn{1}{c}{} & \\multicolumn{',m-2,'}{|c|}{$\\pi(x)/$ } \\\\', sep='')
  print('\\hline')
   
  c = ''
  l = '|'
  for i in range(m):
    if(i!=1):
      print(c, ' \\multicolumn{1}{',l,'c|}{', options[i]['title'], '}', end=' ', sep='')
    l =''
    c = '&'
  print('\\\\')
  print('\\hline')
  for k in range(1,len(options[0]['values'])):
    c = '&'
    print(options[0]['values'][k], end=' ' )
    for i in range(2, m):
      x = round((options[1]['values'][k]/options[i]['values'][k])*1000)/1000
      print(c, x, end=' ')
    print('\\\\')  
  print('\\hline')    
  print('\\end{tabular}')
  print('\\end{table}')

def caption(str, k):
  return str.center(k,' ')
  
# Main function
def main():
  # For the purpose of PNT up to 10^25 we only require to compute prime numbers up to 75
  primes(75)
  # We show the prime numbers between 1 and 75 
  show(1,75)

  print('PRIMES NUMBER THEOREM APPROXIMATIONS')
  k = 26

  options = [ {'values':[0], 'caption':'$10^$', 'title':'$x$'}, {'values':[0], 'caption':'pi(x)', 'title':'$\\pi(x)$'}, {'values':[0], 'caption':'x/log(x)', 'title':'$\\frac{x}{\\log(x)}$', 'f':pnt}, {'values':[0], 'caption':'x/log*(x)', 'title':'$\\frac{x}{\\log^{*}(x)}$', 'f':pntstar}, {'values':[0], 'caption':'x/log<>(x)', 'title':'$\\frac{x}{\\log^{\\diamond}(x)}$', 'f':pntdiamond}, {'values':[0], 'caption':'ho(x)/log(ho(x))', 'title':'$\\frac{h^{\\circ}(x)}{\\log(h^{\\circ}(x))}$', 'f':pnthcirc}, {'values':[0],'caption':'x/log(ho(x))', 'title':'$\\frac{x}{\\log(h^{\\circ}(x))}$', 'f':pnthcirc2}, {'values':[0],'caption':'ho(x)/log<>(x)', 'title':'$\\frac{h^{\\circ}x}{\\log^{\\diamond}(x)}$', 'f':pnthcircdiamond}  
  ]
  for i in range(len(options)):
    print(caption(options[i]['caption'], k), end='')
  print()
  for i in range(1,k):
    x = 10**i
    j = str(i).rjust(3)
    options[0]['values'].append('$10^{'+str(i)+'}$')
    options[1]['values'].append(pi[i])
    for m in range(2,len(options)):
      v = options[m]['f'](x)
      if(i<5): # Small numbers do not require rounding
        options[m]['values'].append(v)
      else:
        options[m]['values'].append(round(v))
    print(j, end=' ')
    for m in range(1,len(options)):
      print(str(options[m]['values'][i]).rjust(k), end=' ')
    print()  
  latex_pnt(options)  
  
main()
