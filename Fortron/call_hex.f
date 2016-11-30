C
      integer*1 char(100) ,h1,h2
      read(3,100) n
      read(3,110)(char(i),i=1,n)
      do 10 i=i,n
         call dtoh(char(i),h1,h2)
         write(3,120) char(i),h1,h2
 10   continue
      stop
 100  format(I3)
 110  format(10A1)
 120  format(' ascii code of' ,A2,'IS',I2,A1,'(H)')
      end
