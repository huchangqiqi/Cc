C
      dimemsion itnum(10),price(10)
      call open(6,'pricetabdat',0)
      do 10 i=1,10
         read(3,100) itnum(i),price(i)
         write(6,100) itnum(i),price(i)
 10   continue
      endfile(6)
      stop
 100  format(I4,F4.2)
 110  format(1X,I4,F4.2)
      end
