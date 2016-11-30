C
      func(x) = x*x-x-2.0
      x1=0.0
      x2=3.0
      f1=func(x1)
      root=(x1+x2)/2.0
 10   if(abs(root-x1) .GT. 5E-5)then
         if(f1*func(root) .GT. 0.0) then
            x1 = root
         else
            x2 = root
         endif
         root = (x1+x2)/2.0
         go to 10
      endif
      write(*,100) root
      stop
 100  format(1X,'ROOT BETWEEN 0 AND 3 IS',E14.7)
      end
