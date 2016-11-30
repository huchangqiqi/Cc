C
      integer x
      real y
      x = 1
      y = 9.99
      x = x + 1.0
      y = y + 1
      write(*,100) x
      write(*,110) y
 100  format(I2)
 110  format(F5.2)
      end
