from icecube.icetray import I3Module
from icecube import dataclasses as dc

import sys

from icecube.sim_services.sanity_checker.modules.mctree_checkers import CorsikaTreeSCModule
from icecube.sim_services.sanity_checker.modules.launch_checkers import HLCLaunchSCModule

RunConfigurations = dict()
RunConfigurations["CORSIKA"] =[ CorsikaTreeSCModule, HLCLaunchSCModule ]
