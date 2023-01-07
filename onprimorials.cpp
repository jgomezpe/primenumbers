#include <iostream>
#include <math.h>

/**
  * Determines if tracing messagess are printed or not
  */
bool TRACE = true; // Set to false if you do not want to trace algorithm

/**
  * Determines the number of bits used for representing an integer
  */  
int bits(){
  uint i=1;
  int k=0;
  while(i>0){
    i <<= 1;
    k++;
  }
  return k;  
}
int BITS = bits();

/** Determines the numbers of bits to shift a number for 
  * determining the position of a number's mark on the array of bits
  */ 
int shift(){
  int bits = BITS;
  int k=-1;
  while(bits>0){
    bits >>= 1;
    k++;
  }
  return k + 1;
}
int SHIFT = shift();

/** 
  * Mask for getting the bit inside the number that represents the number
  */
long MASK = (BITS<<1)-1;

/**
  * Position in the array of the bit representing the odd number n
  */
long pos(long n){ return (n >> SHIFT); }

/**
  * Bit representing the odd number n.
  */
long bit(long n){ return (n & MASK)>>1; }

/**
  * Prime numbers computed up to N
  */ 
long N;

/**
  * Array with odd numbers marked as prime == 0 or not prime == 1
  */ 
int* PRIMES=NULL;

/**
  * Get the value (0 : is prime or 1: is not prime) of the bit representing odd number n.
  */
int get(long n){
  int p = pos(n);
  int b = bit(n);
  return (PRIMES[p] & (1<<b))>>(b);
}

/**
  * Initializes array (up to n) of values as possible primes (all bit set to 0)
  */
void init(long n){
  if(PRIMES != NULL ) delete[] PRIMES;
  int m = (int)(n>>SHIFT) + 1;
  PRIMES = new int[m]; 
  for(int i=0; i<m; i++) PRIMES[i] = 0;
  N = n;
}

/**
  * Mark the number as not prime (set bit to 1)
  */
void mark(long n){
  int p = pos(n);
  int b = bit(n);
  PRIMES[p] |= (1<<b);
}

/**
  * Mark as not prime all multiples of number p starting at p*p
  */
void mark_multiples(long n, long p){
  long p2 = p<<1;
  for(long i=p*p; i<=n; i+=p2) mark(i);
}

/**
  * Runs Erathostenes sieve only considering odd numbers
  */
void sieve(long n){
  init(n);
  int m = (int)sqrt(n);
  mark(1);
  if(TRACE) std::cout << "Marks multiples of:\n"; 
  for(long i=3; i<=m; i+=2)
    if(get(i)==0){
      if(TRACE) std::cout << i << ".."; 
      mark_multiples(n, i);
      if(TRACE) std::cout << "..done\n"; 
    } 
}

/**
  * Determines if a number is prime or not 
  */
bool isprime(long n){ return (n==2 || (n%2!=0 && get(n)==0)); }

/**
  * Counts the number of prime number in a givn interval
  */
long primes(long start, long end){
  long count = start<=2?1:0;
  if(start % 2 == 0) start++;
  for(long i=start; i<=end; i+=2)
    if(get(i)==0) count++;
  return count;
}
/**
  * Print prime numbers in a given interval
  */
void print(long start, long end){
  if(start<=2){
    std::cout << "2 ";
    start = 3;
  }  
  for(long i=start; i<=end; i+=2)
    if(get(i)==0)
      std::cout << i << " ";
  std::cout << "\n";
}

/**
  * Counts the number of twin couples in a given interval
  */
long primes_couple(long start, long end, long sep){
  long count=0;
  if(start % 2 == 0) start++;
  for(long i=start; i<=end-sep; i+=2)
    if(get(i)==0 && get(i+sep)==0) count++;
  return count;
}

/**
  * Counts the number of twin prime couples in a given interval
  */
long twin_primes(long start, long end){
  return primes_couple(start,end,2);
}

/**
  * Counts the number of cousin prime couples in a given interval
  */
long cousin_primes(long start, long end){
  return primes_couple(start,end,4);
}

/**
  * Counts the number of sexy prime couples in a given interval
  */
long sexy_primes(long start, long end){
  return primes_couple(start,end,6);
}

/////////// Functions on paper /////////
/**
 * Gets the nth prime number
 */
long p(long n){
  if(n==1) return 2;
  n-=2;
  for( long i=1; i<N; i+=2 )
    if(get(i)==0){
      if(n==0) return i;
      else n--;
    }
  return -1;
}

/**
  * Computes the nth primorial number
  */
long primorial(long n){
  if(n==0) return 1;
  long prim = 2;
  int k=1;
  long i=3;
  while(k<n){
    if(get(i)==0){
      k++;
      prim *= i;
    }
    i+=2;
  }
  return prim;
}

/**
 * Computes the nth totative number
 */
long tot(long n){
  long t = 1;
  int k=1;
  long i=3;
  while(k<n){
    if(get(i)==0){
      k++;
      t *= (i-1);
    }
    i+=2;
  }
  return t;  
}

/**
 * Computes the number of n-twin totatives
 */
long twin(long n){
  if(n<=2) return n-1;
  return (p(n)-2)*twin(n-1);
}

/**
 * Computes the number of n-cousin totatives
 */
long cousin(long n){
  if(n<3) return 0;
  if(n==3) return n;
  return (p(n)-2)*cousin(n-1);
}

/**
 * Computes the number of n-sexy cuadruplet totatives
 */
long quad(long n){
  if(n<3) return 0;
  if(n==3) return 1;
  if(n==4) return 6;
  return (p(n)-4)*quad(n-1);
}

/**
 * Computes the number of isolated n-sexy triplet totatives
 */
long itriple(long n){
  if(n<3) return 0;
  if(n==3) return 1;
  if(n==4) return 4;
  return (p(n)-3)*itriple(n-1) + 2*quad(n-1);
}

/**
 * Computes the number of n-sexy triplet totatives
 */
long triple(long n){
  return itriple(n) + 2*quad(n);
}

/**
 * Computes the number of isolated n-sexy totative couples
 */
long isexy(long n){
  if(n<4) return 0;
  if(n==4) return 4;
  return (p(n)-2)*isexy(n-1) + 2*itriple(n-1) + 2*quad(n-1);
}

/**
 * Computes the number of n-sexy totative couples
 */
long sexy(long n){
  if(n<3) return 0;
  if(n==3) return 5;
  if(n==4) return 30;
  return (p(n)-2)*sexy(n-1);
}

void latex_table(int m, char* label_a, char* label_b, long* prim, long* a, long* b){
  int cols = 5;
  std::cout << "% Generating table in latex\n";
  std::cout << "\\begin{table}[htbp]\n";
  std::cout << "\\centering\n";
  std::cout << "\\begin{tabular}{|";
  for(int i=0; i<cols; i++) std::cout << "r|";
  std::cout << "}\n";
  std::cout << "\\hline\n";

  std::cout << "$n$ & $\\#(n)$ & $" << label_a << "$ & $" << label_b << "$ & " << "$\\frac{" << label_a << "}{" << label_b << "}$\\\\\n";
  std::cout << "\\hline\n";
  for(int n=3; n<m; n++){
    std::cout << n << " & " << prim[n] << " & " << a[n] << " & " << b[n] << " & " << (a[n]/(double)b[n]) << "\\\\\n"; 
    std::cout << "\\hline\n";

  }
  std::cout << "\\end{tabular}\n";
  std::cout << "\\end{table}\n";
}

void totclasses(){
  int m = 1;
  while(primorial(m) <= N && m<11){ m++; }
  long* prim = new long[m];
  long* pi = new long[m];
  long* t = new long[m];
  long* twp = new long[m];  
  long* twt = new long[m];
  long* sxp = new long[m];
  long* sxt = new long[m];
  long* csp = new long[m];
  long* cst = new long[m];

  for(int n=3; n<m; n++){
    prim[n] = primorial(n);
    pi[n] = primes(1,prim[n]+1);
    t[n] = tot(n);
    twp[n] = twin_primes(1, prim[n]+1);
    twt[n] = twin(n);
    sxp[n] = sexy_primes(1, prim[n]+1);
    sxt[n] = sexy(n);
    csp[n] = cousin_primes(1, prim[n]+1);
    cst[n] = cousin(n);
    std::cout << "**************************\n";
    std::cout << "Primorial set Z_" << n << "=[1,#(" << n << ")+1]\n";
    std::cout << "#(" << n << "):" << prim[n] << '\n';
    std::cout << "Totatives: tot(" << n << ")=" << t[n] << '\n';
    std::cout << "Primes: pi(" << (prim[n]+1) << ")=" << pi[n] << '\n';
    std::cout << "tot/pi = " << (t[n]/(double)pi[n]) << '\n';
    std::cout << "Twin totatives: twin(" << n << ")=" << twt[n] << '\n';
    std::cout << "Twin Primes: twin_primes(" << (prim[n]+1) << ")=" << twp[n] << '\n';
    std::cout << "twin/twin_primes = " << (twt[n]/(double)twp[n]) << '\n';
    std::cout << "Cousin totatives: cousin(" << n << ")=" << cst[n] << '\n';
    std::cout << "Cousin Primes: cousin_primes(" << (prim[n]+1) << ")=" << csp[n] << '\n';
    std::cout << "cousin/cousin_primes = " << (cst[n]/(double)csp[n]) << '\n';
    std::cout << "Sexy totatives: sexy(" << n << ")=" << sxt[n] << '\n';
    std::cout << "Sexy Primes: sexy_primes(" << (prim[n]+1) << ")=" << sxp[n] << '\n';
    std::cout << "sexy/sexy_primes = " << (sxt[n]/(double)sxp[n]) << '\n';
  }

  latex_table(m, "tot(n)", "\\pi(\\#(n)+1)", prim, t, pi);
  latex_table(m, "twin(n)", "twin_{*}(\\#(n)+1)", prim, twt, twp);
  latex_table(m, "cousin(n)", "cousin_{*}(\\#(n)+1)", prim, cst, csp);
  latex_table(m, "sexy(n)", "sexy_{*}(\\#(n)+1)", prim, sxt, sxp);

  delete[] prim;
  delete[] pi;
  delete[] t;
  delete[] twt;
  delete[] twp;
  delete[] cst;
  delete[] csp;
  delete[] sxt;
  delete[] sxp;
}

// @TODO Add test for interval's goldbach conjecture
/**
 * Computes the nth primorial interval return it as [start,end] interval
 */
void primorial_interval(long n, long& start, long& end){
  start = primorial(n-1) + 1;
  end = primorial(n) + 1;
}

/**
 * Test the Goldbach interval conjecture for the (n+1)th primorial interval
 */
bool goldbach_interval( long n ){
  long start, end;
  long start1, end1;
  primorial_interval(n,start,end);
  primorial_interval(n+1,start1,end1);
  bool flag = true;
  long k=start1+1;
  while(flag && k<end1){
    long p = start;
    while(p<=end && (get(p)!=0 || get(k-p)!=0)) p+=2;
    flag = (p<=end);
    if(flag){
      if(TRACE && n<6) std::cout << k << "=" << p << "+" << (k-p) << '\n';
      k+=2;
    }else std::cout << "Fails for " << k << '\n';
  }
  return flag;
}

bool check(long k, long p, int gap){
  long q = k-p;
  return get(p)!=0 || get(q)!=0 || (get(p-gap)!=0 && get(p+gap)!=0 && get(q-gap)!=0 && get(q+gap)!=0);
}

/**
 * Test the Goldbach-couple conjecture for the (n+1)th primorial set
 */
bool goldbach_couple( long n, int gap, char* couple ){
  long start = 3;
  long end = primorial(n)+1;
  bool flag = true;
  long k=6;
  while(flag && k<=end){
    long p = start;
    while(p<=end && check(k,p,gap)) p+=2;
    flag = (p<=end);
    if(flag){
      if(TRACE && n<6){
        long q = k-p;
        std::cout << k << "=" << p << "+" << q << ' ' << couple << ':';
        if(get(q+gap)==0) std::cout << (q+gap) << '\n';
        else std::cout << (q-gap) << '\n';
      }
      k+=2;
    }else std::cout << "Fails for " << k << '\n';
  }
  return flag;
}

void ask_interval(char* txt, long& start, long& end){
  std::cout << txt <<" IN INTERVAL [start,end]\nstart?";
  std::cin >> start;
  std::cout << "end?";
  std::cin >> end;  
}

void print_interval( char* txt, long start, long end){
  std::cout << txt << " in [" << start << "," << end << "]=";
}

int menu(){
  std::cout << "************************\n";
  std::cout << "1. Check primality\n";
  std::cout << "2. Count primes\n";
  std::cout << "3. Count twin primes\n";
  std::cout << "4. Count cousin primes\n";
  std::cout << "5. Count sexy primes\n";
  std::cout << "6. Compute primorial\n";
  std::cout << "7. Compute totative\n";
  std::cout << "8. Compute n-totative classes\n";
  std::cout << "9. Print primes\n";
  std::cout << "10. Goldbach interval conjecture test\n";
  std::cout << "11. Goldbach-Couple conjecture test\n";
  std::cout << "0. Exit\n";
  int c;
  std::cin >> c;
  return c;
}

int main() {
  std::cout << "Program developed by Professor Jonatan Gomez\n";
  std::cout << "Trace process (1=yes, 0=false)?";
  std::cin >> TRACE;
  long n;
  std::cout << "Computing prime numbers up to (max 1e10 = 10000000000)?";
  std::cin >> n;
  sieve(n+1);

  bool flag;
  long c, k;
  long start, end;
  int m = menu();
  while(m!=0){
    switch(m){
      case 1:
        std::cout << "NUMBER TO CHECK (max " << n << ")?";
        std::cin >> start;
        std::cout << isprime(start) << '\n';
      break; 
      case 2:
        ask_interval("COUNT PRIMES", start, end);
        c = primes(start, end);
        print_interval("Counting primes ", start, end);
        std::cout << c << '\n';
      break; 
      case 3: 
        ask_interval("TWIN PRIMES", start, end);
        c = twin_primes(start,end);
        print_interval("Counting twin primes ", start, end);
        std::cout << c << '\n';
      break;
      case 4: 
        ask_interval("COUSIN PRIMES", start, end);
        c = cousin_primes(start,end);
        print_interval("Counting cousin primes ", start, end);
        std::cout << c << '\n';
      break;
      case 5: 
        ask_interval("SEXY PRIMES", start, end);
        c = sexy_primes(start,end);
        print_interval("Counting sexy primes ", start, end);
        std::cout << c << '\n';
      break;
      case 6: 
        std::cout << "COMPUTES PRIMORIAL NUMBER\n";
        std::cout << "n?";
        std::cin >> c;
        k = primorial(c); 
        std::cout << "#(" << c  << ")=" << k << "\n";
      break;
      case 7: 
        std::cout << "COMPUTES TOTATIVE NUMBER\n";
        std::cout << "n?";
        std::cin >> c;
        k = tot(c); 
        std::cout << "tot(" << c  << ")=" << k << "\n";
      break;
      case 8:
        totclasses();
      break;
      case 9: 
        ask_interval("PRIMES", start, end);
        print(start, end); 
      break;
      case 10:
        std::cout << "PRIMORIAL INTERVAL TO CHECK (max 9)?";
        std::cin >> start;
        flag = goldbach_interval(start);
        std::cout << "Goldback interval test result for " << start << ':' << flag << '\n';
      break;
      case 11:
        std::cout << "PRIMORIAL SET TO CHECK (max 9 web version or 10 in a desktop C++ version)?";
        std::cin >> start;
        flag = goldbach_couple(start, 2, "twin");
        std::cout << "Goldback-twin test result for primorial set " << start << ':' << flag << '\n';
        flag = goldbach_couple(start, 4, "cousin");
        std::cout << "Goldback-cousin test result for primorial set " << start << ':' << flag << '\n';
        flag = goldbach_couple(start, 6, "sexy");
        std::cout << "Goldback-sexy test result for primorial set " << start << ':' << flag << '\n';
      break;
    }
    m = menu();
  }
  delete[] PRIMES;
}
