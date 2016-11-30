C
      integer nmax,u,u_new,n
      parameter (nmax=1000,u=20,u_new=21)
      real x(nmax),y(nmax),z(nmax)

C     open the file
      open(u,FILE='point.dat',STATUS='OLD')
      open(u_new,FILE='new_point.dat',STATUS='NEW')

C     read the number of points
      read(u,*) n
      if( n .GT. nmax) then
         write(*,*) 'Error: n= ' ,n , 'is larger then nmax =',nmax
         goto 9999
      endif

C     loop over the data points
      do 10 i=1,n
         read(u,100) x(i) ,y(i),z(i)
 10   enddo
 100  format (3(F5.2))

C     close the file
      close(u)

C     process the data
C     write to the new_point.dat
      do 20 i = 1,n
         write(u_new,200) x(i),y(i),z(i)
 20   enddo
 200  format(3(F5.2))

      close(u_new)




 9999 stop
      end
