C fortran only evaluates the start ,end ,and step experssions once,
C before the first pass thought the body of the loop
C
      integer i,j
      read(*,*)j
      do 20 i = 1,j
         j = j+1
 20   continue
      write(*,*) j
      end


C  while-loops
      while( logical expr ) do
         statements
      enddo


label if(logical expr) then
      statements
      goto label
      endif

C     -----------------------
C do-loops

      do
         statements
      until (logical expr)

label continue
      statements
      if(logical expr) goto label
