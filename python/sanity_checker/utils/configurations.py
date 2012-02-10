from icecube.icetray import I3Module
from icecube import dataclasses as dc

import sys

from ..modules.mctree_checkers import CorsikaTreeSCModule
from ..modules.launch_checkers import HLCLaunchSCModule

RunConfigurations = dict()
RunConfigurations["CORSIKA"] =[ CorsikaTreeSCModule, HLCLaunchSCModule ]
