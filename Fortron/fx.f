C
      READ(*,100)N,A,B
      H=(B-A)/FLOAT(N)
      TOTAL=0.0
      X=A
      F0=X*X + 2.0*X + 1.0
      DO 10 i = 1,N
         X = X + H
         F1 = X*X+2.0*X+1.0
         AVE=(F0+F1)/2.0
         TOTAL=TOTAL+AVE
         F0=F1
 10   CONTINUE
      TOTAL=TOTAL*H
      WRITE(*,110)N,A,B,TOTAL
      STOP
 100  FORMAT(I5,2F6.2)
 110  FORMAT(1X,I5,3F10.3)
      END
