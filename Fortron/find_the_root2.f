C
      function root(func,a,b,tol)
      x1=a
      x2=b
      f1=func(x1)
      c=(x1+x2)/2
 10   if(abs(c-x1) .GT. TOL) then
         if(f1*func(c) .GT. 0.0) then
            x1=c
         else
            x2=c
         endif
      c=(x1+x2)/2.0
      go to 10
      endif
      root = c
      return
      end
C
   

