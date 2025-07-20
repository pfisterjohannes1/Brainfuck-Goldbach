
import sys


code=[]
with open(sys.argv[1]) as f:
  code=''.join( i for i in f.readlines() )
  code = ''.join( i for i in code  if i in "+-<>[].," )

jt={}
s=[]
for i,c in enumerate(code):
  if c=='[': s.append(i)
  if c==']':
    b=s.pop()
    jt[i]=b
    jt[b]=i
if len(s): raise Exception("not matched []") 



p=0
d=[0]
i=0
debug=0
newline=0
while p<len(code):
  debug=debug+1
  if code[p]=='+': d[i]=d[i]+1
  if code[p]=='-': d[i]=d[i]-1
  if code[p]=='.': 
      print(f"{d[i]:>10}",end="\n"*newline)
      newline=1-newline
  if code[p]==',': d[i] = int(input("N:"))
  if code[p]=='>':
    i=i+1
    if i==len(d): d.append(0)
  if code[p]=='<':
    if i: i=i-1
    else: d.insert(0,0)
  if code[p]=='[' and not d[i]:
    p = jt[p]
  if code[p]==']' and d[i]:
    p = jt[p]

  p=p+1

  
  


