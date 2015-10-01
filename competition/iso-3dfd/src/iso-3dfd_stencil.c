/*****************************************************************************
! Copyright(C) 2012 Intel Corporation. All Rights Reserved.
!
! The source code, information  and  material ("Material") contained herein is
! owned  by Intel Corporation or its suppliers or licensors, and title to such
! Material remains  with Intel Corporation  or its suppliers or licensors. The
! Material  contains proprietary information  of  Intel or  its  suppliers and
! licensors. The  Material is protected by worldwide copyright laws and treaty
! provisions. No  part  of  the  Material  may  be  used,  copied, reproduced,
! modified, published, uploaded, posted, transmitted, distributed or disclosed
! in any way  without Intel's  prior  express written  permission. No  license
! under  any patent, copyright  or  other intellectual property rights  in the
! Material  is  granted  to  or  conferred  upon  you,  either  expressly,  by
! implication, inducement,  estoppel or  otherwise.  Any  license  under  such
! intellectual  property  rights must  be express  and  approved  by  Intel in
! writing.
!
! *Third Party trademarks are the property of their respective owners.
!
! Unless otherwise  agreed  by Intel  in writing, you may not remove  or alter
! this  notice or  any other notice embedded  in Materials by Intel or Intel's
! suppliers or licensors in any way.
!
!*****************************************************************************
! Content:
! Source component of a simple example of ISO-3DFD implementation
!
!****************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "iso-3dfd.h"

/***************************************************************
 *
 * iso_3dfd_stencil: apply 8th order ISO stencil 
 *
 ***************************************************************/
void iso_3dfd_stencil (float *ptr_next, float *ptr_prev, float *ptr_vel, float *coeff,
	       	       const int n1, const int n2, const int n3) { 
  const int n1n2 = n1*n2;

  float *prev  = ptr_prev;
  float *next  = ptr_next;
  float *vel   = ptr_vel;

  int i1, i2, i3, ir;
  for(i3=0; i3<n3; i3++) {
    for(i2=0; i2<n2; i2++) {
      for(i1=0; i1<n1; i1++) {
        if( i1>=HALF_LENGTH && i1<(n1-HALF_LENGTH) && i2>=HALF_LENGTH && i2<(n2-HALF_LENGTH) && i3>=HALF_LENGTH && i3<(n3-HALF_LENGTH) ) {
          float div = 0.0;
          div += (*prev)*coeff[0];
          for(ir=1; ir<=HALF_LENGTH; ir++) {
            div += coeff[ir] * (*(prev+ir) + *(prev-ir));         // horizontal
            div += coeff[ir] * (*(prev+ir*n1) + *(prev-ir*n1));   // vertical
            div += coeff[ir] * (*(prev+ir*n1n2) + *(prev-ir*n1n2)); // in front / behind
          }
          *next = 2.0 * (*prev) - (*next) + div * (*vel);
        }
        ++next;
        ++prev;
        ++vel;
      }
    }
  }
}

void iso_3dfd(float *ptr_next, float *ptr_prev, float *ptr_vel, float *coeff,
	      const int n1, const int n2, const int n3, int nreps) {
  int it;
  for(it=0; it<nreps; it+=2){
   	iso_3dfd_stencil ( ptr_next, ptr_prev, ptr_vel, coeff, n1, n2, n3); 
	// here's where boundary conditions+halo exchanges happen

	// Swap previous & next between iterations
    	iso_3dfd_stencil ( ptr_prev, ptr_next, ptr_vel, coeff, n1, n2, n3);
  } // time loop
}
