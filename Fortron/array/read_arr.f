C
      real a(2,3)
C      read (*,100) a
C      write (*,110) a(1,1),a(1,2),a(1,3)

      read(*,100) ((a(i,j),j=1,3),i=1,2)
      a(2,1) = 99.00
      write(*,100) ((a(i,j),j=1,3),i=1,2)
 100  format(3F5.2)
 110  format(3F5.2)
      end
