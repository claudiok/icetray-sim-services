from icecube.icetray import I3Module
from icecube import dataclasses as dc

import sys

from ..modules.mctree_checkers import MCTreeSCModule
from ..modules.mctree_checkers import InIceMCTreeSCModule
from ..modules.mctree_checkers import IceTopMCTreeSCModule

RunConfigurations = dict()
RunConfigurations["corsika"] = [ MCTreeSCModule, InIceMCTreeSCModule ]
RunConfigurations["numu"] = [ MCTreeSCModule, InIceMCTreeSCModule ]
RunConfigurations["nue"] = [ MCTreeSCModule, InIceMCTreeSCModule ]
RunConfigurations["nutau"] = [ MCTreeSCModule, InIceMCTreeSCModule ]
RunConfigurations["icetop"] = [ MCTreeSCModule, IceTopMCTreeSCModule ]

from ..histograms.primary_energy import primary_energy_h
from ..histograms.primary_type import primary_type_h

HistogramConfigurations = dict()
HistogramConfigurations["corsika"] = [ primary_energy_h, primary_type_h ]
HistogramConfigurations["numu"] = [ primary_energy_h, primary_type_h ]
HistogramConfigurations["nue"] = [ primary_energy_h, primary_type_h ]
HistogramConfigurations["nutau"] = [ primary_energy_h, primary_type_h ]
HistogramConfigurations["icetop"] = [ primary_energy_h, primary_type_h ]
