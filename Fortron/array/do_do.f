C
      dimension l(3,2)
      data ((l(i,j),j=1,2) ,i=1,3)/6*1/
      do 10 i=1,3
         write(*,100) (l(i,j),j=1,2)

C      do 10 m=1,3
C         do 10 n=1,2
C            write(*,100) l(m,n)

 10   continue
 100  format(1X,2I5)
      end

