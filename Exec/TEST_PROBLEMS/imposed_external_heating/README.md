# Heated Layer 

Sets up the same isothermal atmosphere as in the `lgw` problem.

Uses external heating to heat a segment of the atmosphere (a constant rate of heating, of the functional form described in `make_heating.F90`, is added in the reaction steps of the code)

You might also consider not heating it in this way, but just adding a temperature perturbation at t=0? (Not sure what this would do but maybe worth trying?)

No specific analysis scripts were made for this probelm. To get started, maybe copy them over from the `lgw` problem (which is in its own branch)
