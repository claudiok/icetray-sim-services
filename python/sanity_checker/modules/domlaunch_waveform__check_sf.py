# isnan is new in python 2.6 and we need to be able to support
# python versions as old as 2.4.  the hack below should do the
# trick, but whenever possible use the real thing
# look for isnan in math, then numpy, then i3math
# if it's not in any of those modules use the str compare hack
try:
    from math import isnan
except ImportError :
    try:
        from numpy import isnan
    except ImportError :
        try:
            from icecube.i3math import isnan
        except ImportError :
            isnan = lambda x : str(x) == "nan"

from I3Tray import I3Units
from icecube import dataclasses as dc
from icecube.sim_services.sanity_checker.utils.counter import Counter
from icecube.sim_services.sanity_checker.bases.sanity_checker import SanityChecker

class DOMLaunchWaveFormChecker( SanityChecker ) :
    def __init__(self):
        SanityChecker.__init__(self)

        self.empty_launch = Counter( tolerance = 0 )
        self.registry.append( self.empty_launch )

    # returns True if all is well
    def check( self, frame ):
        launch_map = frame.Get("InIceRawData")
        for d,launches in launch_map:
            for launch in launches:
                     
            # no particles should have negative lengths
            if(launch.lc_bit):
                count_sum = 0
                for raw_wf in launch.raw_atwd:
                    count_sum += sum(raw_wf)
                
                self.empty_launch.assert_true( count_sum>0)
                self.empty_launch.failure_msg = "DOM %s had empty waveforms even though it was in LC" % d
            
        # call the base class 'check' method
        return SanityChecker.check(self)
