C
      subroutine dtoh(dec,h1,h2)
      integer*1 dec,h1,h2,mask
      mask=15
      h1=dec .and. .not.mask
      h1=h1/16
      h2=dec .and. mask
      if(h2 .gt. 9) goto 10
      h2=h2+48
      goto 20
 10   h2=h2+55
 20   continue
      return
      end
