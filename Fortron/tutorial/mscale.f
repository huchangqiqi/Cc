C
      subroutine mscale(m,n,alpha,A,lda)
      integer m,n,lda
      real alpha,A(lda,*)
C     local variables
      integer j
      do 10 j = 1,n
         call scale(m,alpha,A(1,j))
 10   continue
      return
      end
