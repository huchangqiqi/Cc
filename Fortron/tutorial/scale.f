C
      subroutine scale(n,alpha,x)
      integer n
      real alpha,x(*)
C     local variables
      integer i
      do 10 i = 1,n
         x(i) = alpha*x(i)
 10   continue
      return
      end
