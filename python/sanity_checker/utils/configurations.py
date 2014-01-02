from icecube.icetray import I3Module
from icecube import dataclasses as dc

import sys

from icecube.sim_services.sanity_checker.modules.mctree_checkers import MCTreeSCModule
from icecube.sim_services.sanity_checker.modules.mctree_checkers import InIceMCTreeSCModule
from icecube.sim_services.sanity_checker.modules.mctree_checkers import IceTopMCTreeSCModule

RunConfigurations = dict()
RunConfigurations["corsika"] = [ MCTreeSCModule, InIceMCTreeSCModule ]
RunConfigurations["numu"] = [ MCTreeSCModule, InIceMCTreeSCModule ]
RunConfigurations["nue"] = [ MCTreeSCModule, InIceMCTreeSCModule ]
RunConfigurations["nutau"] = [ MCTreeSCModule, InIceMCTreeSCModule ]
RunConfigurations["icetop"] = [ MCTreeSCModule, IceTopMCTreeSCModule ]
