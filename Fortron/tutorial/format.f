C
      real x
      x = 0.025
      write(*,100) 'x=',x, '=x'
      write(*,'(A,F8.3)') 'The answer is x = ' ,x
      write(*,110) 'The answer is x = ',x
      write(*,120) x

 100  format(A,F5.3,A)
 110  format(A,F8.3)
 120  format('The answer is x = ',F8.3)
      end
