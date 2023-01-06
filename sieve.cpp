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
  * Runs the Sieve of Eratosthenes only considering odd numbers
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
  std::cout << "3. Print primes\n";
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
        ask_interval("PRIMES", start, end);
        print(start, end); 
      break;
    }
    m = menu();
  }
  delete[] PRIMES;
}
