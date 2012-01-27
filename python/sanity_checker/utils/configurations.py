from icecube.icetray import I3Module
from icecube import dataclasses as dc

import sys

from ..modules.mctree_checkers import CorsikaTreeChecker
from ..modules.launch_checkers import HLCLaunchChecker

RunConfigurations = dict()
RunConfigurations["CORSIKA"] =[ CorsikaTreeChecker, HLCLaunchChecker ]
