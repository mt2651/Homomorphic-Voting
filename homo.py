import random

def isPrime(n):
    for i in range(2,int(n**0.5)+1):
        if n%i==0:
            return False
    
    return True

minPrime = 1
maxPrime = 1000000

p = random.randint(minPrime, maxPrime)
while not isPrime(p):
    p = random.randint(minPrime, maxPrime)

q = random.randint(minPrime, maxPrime)
while not isPrime(q) or (p == q):
    q = random.randint(minPrime, maxPrime)

#print(q)
#print(p)

def gcd(a,b):
    if(b==0):
        return a
    else:
        return gcd(b,a%b)


def lcm(x, y):
   if x > y:
       greater = x
   else:
       greater = y

   while(True):
       if((greater % x == 0) and (greater % y == 0)):
           lcm = greater
           break
       greater += 1

   return lcm


n = p*q
n2 = n*n
lamda = lcm(p-1, q-1)
r = random.randint(1, n-1)

g = random.randint(minPrime, maxPrime)
while (gcd(g, n2) != 1):
    g = random.randint(minPrime, maxPrime)

def Enc(a, b):
    return pow(g, a)*pow(b, n)%n2

Cipher = []

def Calculator(arr)
    multi = 1
    for i in arr:
        multi *= i
    return multi

def Ly(ans, lamda, n):
    mod = pow(ans, lamda, n*n)
    return (mod-1)/n

def Lg(l, lamda, n):
    mod = pow(ans, lamda, n*n)
    return (mod-1)/n

def Dec(Ly, Lg, n):
    return lcm(pow(Ly, 1, n), pow(Lg, -1, n))%n
    #return pow(Ly, 1, n) + pow(Lg, -1, n) - 1

//359*173*486*1088*541*163*486
//1213
//1062*588*212*1202*298*209

//925*1052*464*1123*488*588





