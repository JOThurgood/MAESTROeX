! This routine returns the externally imposed (i.e. not reactions)
! heating source term to the enthalpy equation (actually rho * H
! is returned, where H has units of erg/g/s).

module make_heating_module

  use amrex_paralleldescriptor_module, only: parallel_IOProcessor => amrex_pd_ioprocessor
  use meth_params_module, only: prob_lo, rho_comp 
  use amrex_constants_module

  implicit none

  private

contains

  subroutine make_heating(lo, hi, &
                          rho_Hext, r_lo, r_hi, &
                          scal,     s_lo, s_hi, nc_s, &
                          dx, time ) bind (C,name="make_heating")

    integer         , intent (in   ) :: lo(3), hi(3)
    integer         , intent (in   ) :: r_lo(3), r_hi(3)
    integer         , intent (in   ) :: s_lo(3), s_hi(3), nc_s
    double precision, intent (inout) :: rho_Hext(r_lo(1):r_hi(1),r_lo(2):r_hi(2),r_lo(3):r_hi(3)     )
    double precision, intent (in   ) ::     scal(s_lo(1):s_hi(1),s_lo(2):s_hi(2),s_lo(3):s_hi(3),nc_s)
    double precision, intent (in   ) :: dx(3), time

    ! local                              
    integer :: i,j,k                       
    double precision :: x,y,z,r0
    double precision :: fwtm, amp ! make a rt parameter later

    r0 = ((dble(hi(2)) + HALF)*dx(2) - (dble(lo(2)) + HALF)*dx(2))/2.0d0 !translate to Ly/2
!    amp = 1.d17 
    amp = 100
    fwtm = ((dble(hi(2)) + HALF)*dx(2) - (dble(lo(2)) + HALF)*dx(2))/10.0d0
                                         
    do k = lo(3),hi(3)                     
      !z = (dble(j)+0.5d0)*dx(3) + prob_lo(3) ! 2d
      do j = lo(2),hi(2)                  
        y = (dble(j)+0.5d0)*dx(2) + prob_lo(2)
        do i = lo(1),hi(1)               
          x =  (dble(i)+0.5d0)*dx(1) + prob_lo(1)
          rho_Hext(i,j,k) = scal(i,j,k,rho_comp) * &
                            amp * exp(-(y-r0)**2 / 2d0 / (fwtm / 4.29193d0)**2 )
        end do                        
      end do                           
    end do      

  end subroutine make_heating

end module make_heating_module

