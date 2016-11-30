C
      subroutine dtoh
      integer*1 char(100),h1,h2,mask
      common char,h1,h2,i
      maks=15
      h1=char(i) .and. .not.mask
      h2=char(i) .and. mask
      if(h2 .gt. 9) goto 10
      h2=h2+48
      goto 20
 10   h2=h2+55
 20   continue
      return
      end
