
#include <Maestro.H>
#include <Maestro_F.H>

using namespace amrex;

// compute heating term, rho_Hext, then
// react the state over dt_in and update rho_omegadot, rho_Hnuc
void
Maestro::Viscosity ()
{
  std::cout << "Hi from Viscosity.cpp" << std::endl;
}
