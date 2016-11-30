C
      real a(30,30),x(30),p(30)
      read (5,100) n,((a(i,j),j=1,n),i=1,n),(x(i),i=1,n)
      do 10 j=1,n
         p(j) = 0.0
         do 10 k=i,n
            p(j) = p(j) + a(j,k)*x(k)
 10      continue
      write (6,100) (p(j),j=1,n)
 100  format(I2/ (10F12.4))
 110  format(1X,10(F12.4,1X))
      END
