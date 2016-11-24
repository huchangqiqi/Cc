C     Mask
      INTEGER*1 K,MASK,ARY(8)
      READ(3,100) K
      ARY(1) = 0
      MASK = 1
      DO 10 I=1,7
         J=9-I
         ARY(J)=K .AND. MASK
         K=K/2
 10   CONTINUE
         WRITE(3,110) ARY
         STOP
 100     FORMAT(I3)
 110     FORMAT(1X,8I1)
         END
