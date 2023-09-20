import numpy as np
# This program is part of the paper entitled 'The Prime Number Theorem and Primorial Numbers' written by Professor Jonatan Gomez

# Euler's constant
gamma = 0.5772156649

# e^gamma
e_gamma = 1.78107241799

# A list of prime numbers up to 200
P = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199]

# Generates a list of Pi-k values
def P_k(k):
  return [P[i]-k for i in range(len(P))]

# A list of Pi-1 values
P_1 = P_k(1)

# A list of Pi-2 values
P_2 = P_k(2)
P_2[0] = 1

# Quantity of prime numbers up to 10^k (k position in the list)
# Taken from https://oeis.org/A006880 and 
# https://en.wikipedia.org/wiki/Prime_number_theorem
pi = [0, 4, 25, 168, 1229, 9592, 78498, 664579, 5761455, 50847534, 455052511, 4118054813, 37607912018, 346065536839, 3204941750802, 29844570422669, 279238341033925, 2623557157654233, 24739954287740860, 234047667276344607, 2220819602560918840, 21127269486018731928, 201467286689315906290, 1925320391606803968923, 18435599767349200867866, 176846309399143769411680]

# Difference between li function and pi function
li_pi = [0, 2.2, 5.1, 10, 17, 38, 130, 339, 754, 1701, 3104, 11588, 38263, 108971, 314890, 1052619, 3214632, 7956589, 21949555, 99877775, 222744644, 597394254, 1934355208, 7250186216, 17146907278, 55160980939
]

# li function table 10^m
li = [li_pi[i]+pi[i] for i in range(len(pi))]

# Twin primes table
pi_2 = [0, 2, 8, 35, 205, 1224, 8169, 58980, 440312, 3424506, 27412679, 224376048, 1870585220, 15834664872, 135780321665, 1177209242304, 10304195697298]

# Computes the product of the first n [0..n-1] numbers in a list
def listproduct(L, n):
  s = 1
  for i in range(n):
    s *= L[i]
  return s

# Computes the sum of the natural logarithm of the first n [0..n-1] numbers in a list
# i.e. the natural logarithm of the product of the first n numbers in a list
def listlogsum(L, n):
  s = 0
  for i in range(n):
    s += np.log(L[i])
  return s  

# Computes the n-th primorial number
def hash(n):
  return listproduct(P, n)

  
# The r-th multiplicative fraction of number y
def fraction(y, r):
  return 1 + (y-1)*r
  
# Represents a number in terms of primorials, see Definition 18
# i.e., x = #(n_x) * s_x
# returns n_x, #_{n_x}, \lambda_x, s_x
def represent(x):
  global P
  n = 1
  pr_n = 2 # Primorial n #_n
  pr_n1 = pr_n*P[1] # Primorial n+1 #_{n+1} 
  while(pr_n1<=x):
    pr_n = pr_n1
    n += 1
    pr_n1 *= P[n]
  lambda_x = (x-pr_n)/(pr_n1-pr_n)
  s_x = fraction(P[n], lambda_x)
  return n, pr_n, lambda_x, s_x

# Computing natural logarithm using primorials approach, see Equation 4.4. We use this version of log(x) function since numpy.log cannot deal with numbers higher than 10^20
def log(x):
  n_x, pr_nx, lambda_x, s_x = represent(x)
  log_n = listlogsum(P, n_x)
  return np.log(s_x) + log_n

# log(x) function approximation using Theorem ?
def log_pr_q(x, q):
  n_x, pr_nx, lambda_x, s_x = represent(x)
  log_n = listlogsum(P, n_x)
  return log_n + q(x)

# Piecewise linear approximation of log(x), see Equation 4.4
def log_hash(x):
  n_x, pr_nx, lambda_x, s_x = represent(x)
  log_n = listlogsum(P, n_x)
  return log_n + lambda_x*log(P[n_x])

###################### PNT ###################

# Prime number theorem approximation using an approximation of natural logarithm 
def pi_circ(log_circ):
  return lambda x: x/log_circ(x) 

def pi_hash(x):
  n_x, pr_nx, lambda_x, s_x = represent(x)
  pi_star = pi_circ(log)
  return (1-lambda_x)*pi_star(pr_nx)+lambda_x*pi_star(pr_nx*P[n_x])

def pi_diamond(x):
  pi_star = pi_circ(log)
  n_x, pr_nx, lambda_x, s_x = represent(x)
  hash = 1
  s = 0
  for i in range(n_x):
    hash *= P[i]
    # (n_x/(i+1))*
    s += pi_star((1+lambda_x*(P[i+1]-1))*hash)
  return s

PREC = 100000
PREC_CHARS = 7

def PNT(n, pi_circ):
  global PREC
  r = []
  x = 10
  for k in range(1,n+1):
    r.append(round(PREC*pi[k]/pi_circ(x))/PREC)
    x *= 10
  return r

###################### Totatives ###################
def phi_hash(x):
  n_x, pr_nx, lambda_x, s_x = represent(x)
  phi_n = listproduct(P_1, n_x)
  s = 1 + lambda_x*(P[n_x]-2)
  return phi_n*s

def T_hash(x):
  n_x, pr_nx, lambda_x, s_x = represent(x)
  T_n = listproduct(P_2, n_x)
  s = 1 + lambda_x*(P[n_x]-3)
  return T_n*s

def pi_2_circ(pi_approx):
  return lambda x: T_hash(x)*(pi_approx(x)/phi_hash(x))**2

def pi_2_HL(x):
  C2 = 0.6601618158468
  return 2*C2*x/(log(x)**2)

def pi_2_HL_li(x):
  C2 = 0.6601618158468
  return 2*C2*li_x(x)/log(x)

def twin(n, pi_2_approx):
  global PREC
  r = []
  x = 10
  for k in range(1,n+1):
    r.append(round(PREC*pi_2[k]/pi_2_approx(x))/PREC)
    x *= 10
  return r

def li_x(x):
  n = 1
  y = 10
  while(y<x):
    n +=1
    y *= 10
  return li[n]

def pi_x(x):
  n = 1
  y = 10
  while(y<x):
    n +=1
    y *= 10
  return pi[n]


################### latex #################
# Generates Table 2 Asymptotic behavior (latex)
def latex( label, f_caption, col_title, col_values ):
  global PREC_CHARS
  m = len(col_title)
  print('%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%')
  print('% Generating Asymptotic behavior table of', f_caption)
  print('\\begin{table}[htbp]')
  print('\\centering')
  print('\\begin{tabular}{|r|',end='')
  for i in range(m):
    print('r|', end='')
  print('}')

  print('\\cline{2-',(m+1),'}', sep='')
  print('\\multicolumn{1}{c}{} & \\multicolumn{',m,'}{|c|}{$',f_caption,'/$ } \\\\', sep='')
  print('\\hline')
   
  print(' \\multicolumn{1}{|c|}{$x$}', end=' ')
  for i in range(m):
    print(' & \\multicolumn{1}{c|}{$', col_title[i], '$}', end=' ', sep='')
  print('\\\\')
  print('\\hline')
  
  n = len(col_values[0])
  for k in range(n):
    print('$10^{', (k+1) , '}$ ', sep='', end=' ' )
    for i in range(m):
      v = str(col_values[i][k])
      while(len(v)<PREC_CHARS):
        v += '0'
      print(' & ', v, end=' ')
    print('\\\\')  
  print('\\hline')    
  print('\\end{tabular}')
  print('\\label{tab:pnt',label,'}', sep='')
  print('\\caption{Asymptotic behavior of $', f_caption, '$ compare to' , sep='', end='')
  for i in range(m):
    print(', $', col_title[i], '$', end='', sep='')
  print('. Results are obtained with the Python program (primorialsprimes.py) freely available at Professor Jonatan Gomez github repository \\cite{GomezPrimesGit}.}')  
  print('\\end{table}')
  
# Main function
def main():
  # primes 
  star = PNT(25,pi_circ(log))
  star_log_hash = PNT(25,pi_circ(log_hash))
  star_hash = PNT(25, pi_hash)
  diamond = PNT(25,pi_diamond)

  latex('pi', '\\pi(x)', ['\\pi^{\\star}(x)', '\\frac{x}{\\log_{\\#}(x)}', '\\pi^{\\star}_{\\#}(x)', '\\pi^{\\diamond}(x)'], [star, star_log_hash, star_hash, diamond])

  # Twin primes
  pi_2x = twin(16, pi_2_circ(pi_x))
  star_2 = twin(16, pi_2_circ(pi_circ(log)))
  li_2 = twin(16, pi_2_circ(li_x))
  lh = twin(16, pi_2_HL)
  lhli = twin(16, pi_2_HL_li)

  latex('pi_2', '\\pi_2(x)', ['E^{(T)}[x]', 'E^{(T)}_{\star}[x]', 'E^{(T)}_{li}[x]', 'E^{(T)}_{HL}[x]', 'E^{(T)}_{HLli}[x]'], [pi_2x, star_2, li_2, lh, lhli])
  
main()
