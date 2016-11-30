C
      subroutine spec(vector,n,mean,numzro)
      real vector(n),mean
      mean =0.0
      numzro = 0
      do 30 i=1,n
         if(vector(i) .eq. 0.0) goto 10
         mean=mean+vector(i)
         goto 20
 10      numzro=numzro+1
 20      continue
 30   continue
      mean=mean/float(n)
      return
      end
