C
      dimension l(3,2)
      data ((l(i,j),j=1,2),i=1,3) /6*1/
C     双层隐do表 j是内层循环变量 i是外层循环变量
C     每层循环都用一对括号
C     循环变量前面加括号

      do 10 i=1,3
         do 10 j=1,2
            write(*,100)l(i,j)
 10      continue
 100     format(1X,2I5)
         end
