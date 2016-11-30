C
      subroutine saxpy(n,alpha,x,y)
      integer n
      real alpha,x(*),y(*)
C
C saxpy: compute y:= alpha*x + y
C where x and y are vectors of length n (at least)
C
C     local variables
      integer i

      do 10 i = 1,n
         y(i) = alpha*x(i)+y(i)
 10   continue
      return
      end
