C
      real x(5),y(5),alpha
      integer n
      n=5
      alpha = 2
      read (*,*)(x(i),i=1,5)
      read (*,*)(y(i),i=1,5)
      call saxpy(n,alpha,x,y)
      write(*,*)(y(i),i=1,5)
      end

