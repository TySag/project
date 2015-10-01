      PROGRAM MAIN
      IMPLICIT COMPLEX(8) (C) 
      IMPLICIT INTEGER (I, J, M, N) 
      INTEGER, PARAMETER:: NS=8, NP=13284
      
      REAL(8):: x, y, z
      REAL(8), DIMENSION(NP, NS):: SV
      COMPLEX(8), DIMENSION(NP):: CR, CR1

      REAL:: t1, t2
      
      NRS = 3
      MP = 2

!     data initialization
      do i = 1, NP
          do j = 1, NS
              call random_number(x)
              SV(i, j) = x 
          end do

          call random_number(y)
          call random_number(z)
          CR(i) = cmplx(y, z)

          call random_number(y)
          call random_number(z)
          CR1(i) = cmplx(y, z)
      end do

!     data process 
      call CPU_TIME(t1)
      do I = 1, 110000
         CSUM = (0.0, 0.0)
         CALL COMPLEXPROC(SV, CR, CR1, NRS, 
&           NP-10, NS, MP, CSUM)
      end do
      call CPU_TIME(t2)
      print *, "total time(s): ", t2 - t1

!     calculate the result of original version      
      CSUM1 = (0.0, 0.0)
      CALL COMPLEXPROC1(SV, CR, CR1, NRS, 
&        NP-10, NS, MP, CSUM1)
      

!     correctness verification
      IF (CSUM1 .eq. CSUM) then
          print *, "verification: correct"
      ELSE
          print *, "verification: incorrect, cal=", CSUM,
&            " orginal=", CSUM1
      ENDIF

      END

