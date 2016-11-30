C
      program george
      read(*,*)n,k
      ncomb = nfact(n)/(nfact(k) * nfact(n-k))
      print*,'the cmbination value of size',
     +     k,'in size ', n,'=',ncomb
      end

