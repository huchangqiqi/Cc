C
      dimension m(5)
      do 10 i=1,5
         m(i) = 0
         write(*,100)(m(j),j=1,i)
 10   continue
      stop
 100  format(1X,5I2)
      end

