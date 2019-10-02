# Linear Gravity Waves Problem

## Contains

  * `experimental_override` - old code relating to linear wave action conservation (see also the corresponding python notebooks for info)

  * `base_state.f90` - replaces the code in the main source tree at compile. Sets up an isothermal atmosphere based on a number of runtime parameters.

  * `initdata.f90` - adds perturbations to the atmosphere to set up the standing waves.

  * `_parameters` - specifies the custom runtime parameters, their types and default values

  * `simple.net` - single fluid species.

  * `prep_visit.sh` - simple script to create a list of plotfiles that visit can open / treat as a timemseries. 

  * `analysis/` - a number of scripts for data analysis associated with the problem

  * `experimental_override/`  - some old stuff, doesn't effect compile (to do with linear wave action hack).

## Problem Setup

## Analysis Scripts

Jupyter notebooks (using Yt) are included in `analysis/`.


* `simulation_quick_look.ipynb` a number of analyses related top the problem. Visualisation of base state at t=0, td-diagrams showing the evolution along 1d cuts in time (e.g. the base state evolution), measured vs expected periods of standing waves measured at single points, timestep history (dt vs t), movie frames. 

* `how_to_read_base_data_without_yt.ipynb` Shows how to get base state data from plotfiles into python without using Yt at all (the base state data within the plotfiles is just stored as simple textfiles on a uniform grid, so dont have any AMR complications and don't need Yt).

* `detailed_look_at_ICs.ipynb` compares expected and recorded values of the base state pressure and density in detail. 

* `energy_evolution.ipynb` a look at the time-series recorded of kinetic, internal, and gravitiational potential energy (i.e. the contents of `diag_vel.out`). Currently shows that the total energy is apparently oscillating, and that their is a currently unexplained mismatch in delta ke and delta ge. Shows also that total internal energy is oscillating (should it?)

* `first_look_at_linear_wave_action.ipynb` A look at the linear wave action conservation. I found it was ok.

* `energies_vs_linear_wave_aciton.ipynb` comparison of linear wave action and full / normal energy definitions.

* `amplitude_test.ipynb` Compares measured vs expected periods at probe points for a varienty of amplitudes. Defines a function which plots the expected signal at a single point for the standing oscillations (for given kh, kv, gamma, g, H) and the measured signal from simulation data at that point. Note that if you chose points close to the nodes this can produce some misleading/confusing plots.

Python scripts in `analysis/`

* `quick_2d_movie.py` script which takes command line args to produce a movie for a time-series of plotfiles. Run  `python quick_2d_movie.py  -h` for help.
