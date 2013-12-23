from icecube.icetray import I3Module
from icecube import dataclasses as dc

import sys

from icecube.sim_services.sanity_checker.modules.mctree_checkers import MCTreeSCModule
from icecube.sim_services.sanity_checker.modules.mctree_checkers import InIceMCTreeSCModule
from icecube.sim_services.sanity_checker.modules.mctree_checkers import IceTopMCTreeSCModule

RunConfigurations = dict()
RunConfigurations["CORSIKA"] = [ MCTreeSCModule, InIceMCTreeModule ]
RunConfigurations["NuMu"] = [ MCTreeSCModule, InIceMCTreeModule ]
RunConfigurations["NuE"] = [ MCTreeSCModule, InIceMCTreeModule ]
RunConfigurations["NuTau"] = [ MCTreeSCModule, InIceMCTreeModule ]
RunConfigurations["IceTop"] = [ MCTreeSCModule, IceTopMCTreeModule ]
