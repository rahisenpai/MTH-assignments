'''
Write a program in Python to solve the homogenous system Ax=0 and write the general solution in parametric vector form.
Your program should accept as input the size of the matrix, i.e. the number of rows and the number of columns of A, and also the entries of A.
The input should ideally be read from a text file, but if you haven't learnt how to do this, you may hard-code your input, as long as you are able to explain to your TA how to change the input to your program.
'''

from sympy import Matrix

#take input
f = open("programming assignment\input.txt")
d=f.readlines()
r,c=int(d[0]),int(d[1])
l1=[]
for i in range(2,len(d)):
    x=[]
    y=d[i].split()
    for j in y:
        x.append(float(j))
    x.append(0)
    l1.append(x)

#get rref
m=Matrix(l1)
m1=m.rref()
l=[]
x=list((m1)[0])
pivot=list((m1)[1])
j=0
for i in range(r):
    l.append(x[j*(c+1):(j+1)*(c+1)])
    j+=1
print('\nrref of [A 0]:')
for i in l:
    print(i)

#get free variables
fv=[i for i in range(c)]
for i in pivot:
    fv.remove(i)

#get parametric form
zeroes=[0 for i in range(c)]
soln='\ngeneral solution in parametric vector form:\n'+str(zeroes)
for i in fv:
    soln+=' + '
    soln+=f'x{i+1}*'
    x=[-(l[k][i]) for k in range(r)]
    for j in range(1,c+1):
        if (j-1) not in pivot and len(x)<c:
            x.insert(j-1,0)
    x[i]=1
    soln=soln+str(x)
print(soln)


'''
#scratch implementation to find rref of a matrix
#0 and -0 are equal and the results close to zero may vary, depending at which step round function is used

#check non zero pivot column
def fun(l):
    pc=0
    while pc<len(l[0])-2:   
        for i in l:
            if i[pc]!=0:
                return pc
        pc+=1
    return pc

#take input
f = open(r"D:\Python\new\programming assignment\input.txt")
d=f.readlines()
r,c=int(d[0]),int(d[1])
l=[]
for i in range(2,len(d)):
    x=[]
    y=d[i].split()
    for j in y:
        x.append(float(j))
    x.append(0)
    l.append(x)

#echelon form
for i in range(r):
    y = fun(l[i:])
    if y==c-1 and l[i].count(0)==c+1:
        continue
    while l[i][y]==0:
        x=l.pop(i)
        l.append(x)
    for j in range(y,c):
        if l[i][j]!=0:
            l[i]=[round(z/l[i][j],4) for z in l[i]]
            break
    for m in range(i+1,r):
        x=l[m][y]/l[i][y]
        for k in range(c):
            l[m][k]-=x*l[i][k]
#push row of zeroes at last
y=0
for i in range(r):
    if l[y].count(0)==c+1:
        x=l.pop(y)
        l.append(x)
        y-=1
    y+=1

#pivot positions
pivot=[]
for i in range(r):
    for j in range(c):
        if l[i][j]!=0:
            pivot.append((i,j))
            break
print(l)

#rref
for i in range(-1,-len(pivot)-1,-1):
    a,b=pivot[i][0],pivot[i][1]
    for m in range(a):
        x=l[m][b]
        for k in range(c):
            l[m][k]=round(l[m][k]-x*l[a][k],3)
print('\nrref of [A 0]:')
for i in l:
    print(i)

#free variables
fv=[i for i in range(c)]
for i in pivot:
    fv.remove(i[1])

#solution
j=-1
soln='\nsolution:\n'
for i in fv:
    if j!=-1:
        soln+=' + '
    soln+=f'x{i+1}*'
    x=[-(l[k][i]) for k in range(r)]
    for j in range(1,c+1):
        if (j-1) not in pivot and len(x)<c:
            x.insert(j-1,0)
    x[i]=1
    soln=soln+str(x)
print(soln)
'''