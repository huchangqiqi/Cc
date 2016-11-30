C
      dimension itnum(10),price(10)
      read(5,100)(itnum(i),price(i),i=1,10)
      read(5,110)inum,qty
 10   if(inum .eq. 9999) goto 40
      i=1
 20   if(i .eq. 10 .or. inum .le. itnum(i)) goto 30
      i=i+1
      goto 20
      continue
      if(inum .eq. itnum(i)) then
         cost=price(i)*qty
         write(6,120)inum,cost
      else
         write(6,130)inum
      endif
      read(5,110),inum,qty
      goto 10
 40   continue
      stop
 100  format(I4,F4.2)
 110  format(I4,F5.0)
 120  format(1X,5Hitem ,I4,6H cost ,F7.2)
 130  format(1X,5Hitem ,I4, 10H not found)

