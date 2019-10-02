# Linear Gravity Waves Problem

## Problem Setup

## Analysis Scripts

Jupyter notebooks (using Yt) are included in `analysis/`.

* `how_to_read_base_data_without_yt.ipynb` Shows how to get base state data from plotfiles into python without using Yt at all (the base state data within the plotfiles is just stored as simple textfiles on a uniform grid, so dont have any AMR complications and don't need Yt).

* `detailed_look_at_ICs.ipynb` compares expected and recorded values of the base state pressure and density in detail. 

Python scripts in `analysis/`

* `quick_2d_movie.py` script which takes command line args to produce a movie for a time-series of plotfiles. Run  `python quick_2d_movie.py  -h` for help.

There is also `prep_visit.sh` which encapsulates a useful command for processing a series of plotfiles into something visit can treat as a time-series (if you are into that sort of thing). 
