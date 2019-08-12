module viscosity_module

!  use amrex_mempool_module, only : bl_allocate, bl_deallocate
!  use eos_type_module
!  use eos_module
!  use network, only: nspec
!  use meth_params_module, only: rho_comp, temp_comp, spec_comp, &
!       cfl, use_soundspeed_firstdt, use_divu_firstdt, &
!       use_exact_base_state
!  use base_state_geometry_module, only:  max_radial_level, nr_fine, nr, dr

  implicit none

  private

contains

  subroutine visc_estdt(lev, dt, lo, hi, dx, &
                    scal, s_lo, s_hi, nc_s, &
                    u,    u_lo, u_hi, nc_u) bind (C,name="visc_estdt")

!    use amrex_fort_module, only: amrex_min, amrex_max

    integer  , value, intent(in   ) :: lev
    double precision, intent(inout) :: dt
    integer         , intent(in   ) :: lo(3), hi(3)
    double precision, intent(in   ) :: dx(3)
    integer         , intent(in   ) :: s_lo(3), s_hi(3), nc_s
    integer         , intent(in   ) :: u_lo(3), u_hi(3), nc_u
    double precision, intent(in   ) :: scal (s_lo(1):s_hi(1),s_lo(2):s_hi(2),s_lo(3):s_hi(3),nc_s)
    double precision, intent(in   ) :: u    (u_lo(1):u_hi(1),u_lo(2):u_hi(2),u_lo(3):u_hi(3),nc_u)
 
  !! this is on the commit prior to estdt being refactored for gpu so is a bit easier to understand / work from
  !! https://github.com/AMReX-Astro/MAESTROeX/blob/4463a9344e266962c4d28ea565c76b85a22ebbcb/Source/Src_nd/compute_dt.F90

  !! return a fixed dt for now
  dt = 0.1

  end subroutine visc_estdt

end module viscosity_module
