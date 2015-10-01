      SUBROUTINE COMPLEXPROC1(SV,CR,CR1,NRS, NP,NS,MP,CSUM)
      IMPLICIT COMPLEX(8) (C) 
      IMPLICIT INTEGER (I, M, N) 
      REAL(8), DIMENSION(NP, NS) :: SV
      COMPLEX(8), DIMENSION(NP) :: CR, CR1

      DO M =1, NP
         CSUM = CSUM + CR(M) * CONJG(CR1(M))
&           * SV(M, 1+I2+2*I1)
      ENDDO

      DO I1 =0, NRS-1
         DO I2 =0, NRS-1
            DO M =1, NP
               MM  = M + I1*MP
               MM_ = M + I2*MP

               CSUM = CSUM + CR(MM_) * CONJG(CR1(MM))
&                  * SV(M, 1+I2+2*I1)
            ENDDO
         ENDDO
      ENDDO

      END SUBROUTINE COMPLEXPROC1
