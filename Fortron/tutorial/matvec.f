C
      subroutine matvec(m,n,A,lda,x,y)
      integer m,n,lda
      real x(*),y(*),A(lda,*)
C
C     compute y = A*x where A is m by n and
C     stored in an array with leading dim lda
C
C     local variables
      integer i,j
C
C     init y
      do 10 i=1,m
         y(i) = 0.0
 10   continue

C     matrix-vector product by saxpy on columns in A
C     notice that the length of each columns of A is
C     m,not n!!!

      do 20 j = 1,n
         call saxpy(m,x(j),A(1,j),y)
 20   continue
      return
      end
