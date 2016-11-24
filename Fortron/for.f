C
      x=2.0
      y=0.0
      term=x
      do 10 n=1,10
         term=term*x/float(n)
         y=y+term
         write(*,100)n,y
 10   continue
      stop
 100  format(1X,2HN=,I2,5X,2HY=,F8.4)
      end
