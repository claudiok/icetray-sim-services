from icecube.icetray import I3Module
from icecube import dataclasses as dc

import sys

from icecube.sim_services.sanity_checker.modules.mctree_checkers import MCTreeSCModule

RunConfigurations = dict()
RunConfigurations["CORSIKA"] = [ MCTreeSCModule ]
RunConfigurations["NuMu"] = [ MCTreeSCModule ]
RunConfigurations["NuE"] = [ MCTreeSCModule ]
RunConfigurations["NuTau"] = [ MCTreeSCModule ]
