C
      real function r(m,t)
      integer m
      real t

      r = 0.1 *t * (m**2+ 14*m + 46)
      if(r .LT. 0 ) r = 0.0
      return
      end
