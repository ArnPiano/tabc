Tabc=function(x1,x2,alt,B=1000000)
{x=c(x1,x2)
n1=length(x1)
n2=length(x2)
n=n1+n2
mediana=median(x) 
ranghi=rank(x)
ta.ob=sum(x1)-sum(x2)
tb.ob=length(x1[x1>=mediana])-length(x2[x2>=mediana])
tc.ob=sum(ranghi[1:n1])-sum(ranghi[(n1+1):n])
ta.perm=vector(,B)
tb.perm=vector(,B)
tc.perm=vector(,B)
tabc.perm=vector(,B)

for (b in 1:B)
{x.perm=sample(x)
x1.perm=x.perm[1:n1]
x2.perm=x.perm[(n1+1):(n1+n2)]
ranghi.perm=rank(x.perm)
ta.perm[b]=sum(x1.perm)-sum(x2.perm)
tb.perm[b]=length(x1.perm[x1.perm>=mediana])-length(x2.perm[x2.perm>=mediana])
tc.perm[b]=sum(ranghi.perm[1:n1])-sum(ranghi.perm[(n1+1):n])
}

if (alt=="greater") 
{pv.ta.ob=length(ta.perm[ta.perm>=ta.ob])/B
pv.tb.ob=length(tb.perm[tb.perm>=tb.ob])/B
pv.tc.ob=length(tc.perm[tc.perm>=tc.ob])/B
}
if (alt=="less") 
{pv.ta.ob=length(ta.perm[ta.perm<=ta.ob])/B
pv.tb.ob=length(tb.perm[tb.perm<=tb.ob])/B
pv.tc.ob=length(tc.perm[tc.perm<=tc.ob])/B
}
if (alt=="two.sided") 
{pv.ta.ob=length(abs(ta.perm)[abs(ta.perm)>=abs(ta.ob)])/B
pv.tb.ob=length(abs(tb.perm)[abs(tb.perm)>=abs(tb.ob)])/B
pv.tc.ob=length(abs(tc.perm)[abs(tc.perm)>=abs(tc.ob)])/B
}

tabc.ob=min(pv.ta.ob,pv.tb.ob,pv.tc.ob)
##HERE
for (b in 1:B)
{if (alt=="greater") 
{pv.ta.perm=length(ta.perm[ta.perm>=ta.perm[b]])/B
pv.tb.perm=length(tb.perm[tb.perm>=tb.perm[b]])/B
pv.tc.perm=length(tc.perm[tc.perm>=tc.perm[b]])/B
}
  if (alt=="less") 
  {pv.ta.perm=length(ta.perm[ta.perm<=ta.perm[b]])/B
  pv.tb.perm=length(tb.perm[tb.perm<=tb.perm[b]])/B
  pv.tc.perm=length(tc.perm[tc.perm<=tc.perm[b]])/B
  }
  if (alt=="two.sided") 
  {pv.ta.perm=length(abs(ta.perm)[abs(ta.perm)>=abs(ta.perm[b])])/B
  pv.tb.perm=length(abs(tb.perm)[abs(tb.perm)>=abs(tb.perm[b])])/B
  pv.tc.perm=length(abs(tc.perm)[abs(tc.perm)>=abs(tc.perm[b])])/B
  }
  
  tabc.perm[b]=min(pv.ta.perm,pv.tb.perm,pv.tc.perm)
}

pv.tabc=length(tabc.perm[tabc.perm<=tabc.ob])/B
print(pv.tabc)
}

##################################
library(Rcpp)
sourceCpp("cppTabc2.cpp")
x1=rnorm(50)
x2=rnorm(50)
B=100000

##################################

start.time = proc.time()

Tabc(x1,x2,"two.sided",B)

duration.time = proc.time() - start.time
print(duration.time)

################################## 
start.time = proc.time()

p=cppTabc(x1,x2,"two.sided",B) 
print(p)

myduration.time = proc.time() - start.time 
print(myduration.time) 

print(1-(myduration.time[3] / duration.time[3] ))
