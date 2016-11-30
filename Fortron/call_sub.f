C
      dimension ary(10)
      read(3,100) ary
      call spec(ary,10,aver,nzero)
      write(3,110) aver,nzero
      stop
 100  format(10F8.2)
 110  format(1X,F8.2,I3)
      end
