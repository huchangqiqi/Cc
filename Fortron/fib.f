C
      write(*,100)
      read(*,110) x1,x2
C     get 8 more terms
      do 10 i=3,10
         x=x1+x2
         write(6,120)i,x
         x1=x2
         x2=x
10    continue
      stop
 100  format(15Hfibonacci series/1X
     +     5X ,5Habcde)
110   FORMAT(2F10.5)
120   FORMAT(1X,5HTERM ,I3,3H IS,F10.5)
      end



