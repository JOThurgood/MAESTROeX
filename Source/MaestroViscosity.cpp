
#include <Maestro.H>
#include <Maestro_F.H>

using namespace amrex;

// Viscosity
/* Solves dU/dt = mu del^2 U explicitly with FTCS
   up to dt_in
  
  Will be used in an operator splitting formalism.

  Think at step 1 want to use density in `sold`, and in step 5 and 8 use `s2`
  Think always want `uold`
  Not sure about w0. Think in step 1 you want `w0`, since that *is* w0_old by then, it sjust `w0_old=w0` doesn't happen till step 2
  At later times, think want to use `w0_old` to correspond to `uold`.

  Arguments

  Since you want w0 and w0_old at different times, it should be an argument rather than using the w0 data member directly
  You always use uold so it could be direct, but it might turn out this is wrong so perhaps it too should be an argument
  s varies so needs to be an argument.


*/

void
Maestro::Viscosity (Vector<MultiFab>& u_in,
                    RealVector& w0_in,
                    const Vector<MultiFab>& s_in, 
                    const Real dt_in)
{

  // think you always need to construct a full U, don't think the equation decouples properly?
  // (as opposed to diffusing utilde and w0 separately)

  // 1. construct full velocity. My gut instinct is this step is involved enough to be its own function,
  //    but to have things like lev in scope it would have to be a member of Maestro so I don't know if it is ill-advised

  // create a uold with filled ghost cells
  Vector<MultiFab> utilde(finest_level+1);
  for (int lev=0; lev<=finest_level; ++lev) {
      utilde[lev].define(grids[lev], dmap[lev], AMREX_SPACEDIM, ng_adv);
      utilde[lev].setVal(0.);
  }

  FillPatch(t_new, utilde, u_in, u_in, 0, 0, AMREX_SPACEDIM, 0, bcs_u, 1); // not sure about this. Why does it need t_new, etc etc

  // create a MultiFab to hold uold + w0
  Vector<MultiFab>      ufull(finest_level+1);
  for (int lev=0; lev<=finest_level; ++lev) {
      ufull[lev].define(grids[lev], dmap[lev], AMREX_SPACEDIM, ng_adv);
      ufull[lev].setVal(0.);
  }

  // create ufull = uold + w0
  Put1dArrayOnCart(w0_in,ufull,1,1,bcs_u,0,1);
  for (int lev=0; lev<=finest_level; ++lev) {
      MultiFab::Add(ufull[lev],utilde[lev],0,0,AMREX_SPACEDIM,ng_adv);
  }


  // 2. solve the full velocity to dt

  // some time loop   
  Real visc_time = 0.0;
  while (visc_time < dt_in){

    // Calculate the viscous time step - this is actually going to be a fair chunk of code for the paralleism. Make a Maestro::ViscosityDt function?
    // dt_visc = ViscosityDt(ufull,s_in)

    //  check that this timestep will not take us past dt_in. If it is, set the timestep to be such that visc_time == dt_in after the update
    //  if ( (visc_time + dt_visc) > dt_in ) dt_visc = dt_in - visc_time + SMALL // ?

    // diffuse the viscosity for dt_visc worth of time.
    // ViscosityExplicitSolve(ufull,s_in, dt_visc)

    // advance the time in the viscosity solve
    // visc_time += dt_visc 
  }

  // 3. decompose the full velocity and update u_in and w0_in 

}


// this will need a prototype declared in Maestro.H
void
Maestro::ViscosityExplicitSolve(Vector<MultiFab>& u_in,
                                const Vector<MultiFab>& s_in,
                                Real dt_in)
{
  // loop through the levels and individual boxes
  // call a fortran routine that will take u_in, s_in, dt_in and update that velocity field (internal uolds and unews will be dealt with within fortran)
}
