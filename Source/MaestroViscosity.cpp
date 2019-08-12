
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

  Real visc_time = 0.0;
  while (visc_time < dt_in){

    // Calculate the viscous time step
    Real dt_visc = 1.e99; // this is always overwritten in ViscosityDt
    ViscosityDt(ufull,s_in, dt_visc);
    if ( (visc_time + dt_visc) > dt_in ) dt_visc = dt_in - visc_time; // limit if needed

    // update the ghosts of ufull / boundary conditions
    // how? 
    // diffuse the viscosity for dt_visc worth of time.
    // ViscosityExplicitSolve(ufull,s_in, dt_visc)

    // advance the subcycling time
     visc_time += dt_visc; 
  }

  // 3. decompose the full velocity and update u_in and w0_in 

}


// the u_in is the full velocity field, s_in is for density, dt_in will be mutated to the calculated dt
void
Maestro::ViscosityDt(const Vector<MultiFab>& u_in,
                     const Vector<MultiFab>& s_in,
                     Real& dt_in)
{

  Real dt_visc = 1.e20;
  Real dt_lev = 1.e99;

  // Loop over levels
  for (int lev = 0; lev <= finest_level; ++lev) {

    // get references to the MultiFabs atlev
    const MultiFab& u_in_mf = u_in[lev];
    const MultiFab& s_in_mf = s_in[lev];

#ifdef _OPENMP
#pragma omp parallel reduction(min:dt_lev)
#endif
    {
      
      Real dt_grid = 1.e99;

      // Loop over boxes (make sure mfi takes a cell-centered multifab as an argument)
      for ( MFIter mfi(u_in_mf, true); mfi.isValid(); ++mfi ) {

        // Get the index space of the valid region
        const Box& tileBox = mfi.tilebox();
        const Real* dx = geom[lev].CellSize();

        // call fortran 
        if (spherical ==0) {

          visc_estdt(&lev, &dt_grid, // why give it lev (and lev being mutable might be pretty dangerous) 
                      ARLIM_3D(tileBox.loVect()), ARLIM_3D(tileBox.hiVect()),
                      ZFILL(dx),
                      BL_TO_FORTRAN_FAB(s_in_mf[mfi]),
                      BL_TO_FORTRAN_FAB(u_in_mf[mfi]));

        } else {
          Abort("viscosity not currently supported for spherical");
        }


      } // end loop over boxes
   
      dt_lev = std::min(dt_lev, dt_grid);

    } // end OpenMP block

    // find the smallest dt over all processors
    ParallelDescriptor::ReduceRealMin(dt_lev);

    if (maestro_verbose > 0) {
      Print() << "Call to estdt for level " << lev << " gives dt_lev = " << dt_lev << std::endl;

    // update dt_visc over all levels
    dt_visc = std::min(dt_visc,dt_lev);

    }

  } // end lev loop

  // set dt_in to the final calculated dt
  dt_in = dt_visc;

}

/* 

 // this will need a prototype declared in Maestro.H
 void
 Maestro::ViscosityExplicitSolve(Vector<MultiFab>& u_in,
                                 const Vector<MultiFab>& s_in,
                                 Real dt_in)
 {
   // loop through the levels and individual boxes
   // call a fortran routine that will take u_in, s_in, dt_in and update that velocity field (internal uolds and unews will be dealt with within fortran)
 }

*/
