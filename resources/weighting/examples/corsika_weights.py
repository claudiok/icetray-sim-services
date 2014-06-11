#!/usr/bin/env python

"""
Calculate weights for 5-component CORSIKA simulation.
"""

from icecube import icetray, dataclasses, dataio
from I3Tray import I3Tray

from icecube.sim_services.weighting import WeightCalculator
from icecube.sim_services.weighting import fluxes

import sys
infiles = sys.argv[1:-1]
outfile = sys.argv[-1]

tray = I3Tray()
tray.AddModule('I3Reader', 'reader', filenamelist=infiles)
tray.AddModule(WeightCalculator, 'GaisserH3aWeight', Dataset=10285, Flux=fluxes.GaisserH3a())
tray.Execute()
