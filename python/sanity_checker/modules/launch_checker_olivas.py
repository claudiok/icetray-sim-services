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

class DOMLaunchChecker( SanityChecker ) :
    def __init__(self):
        SanityChecker.__init__(self)
        
        self.emptyWaveformCounter = Counter( tolerance = 0 )
        self.registry.append( self.emptyWaveformCounter )
        
        # returns True if all is well
        def check( self, frame ):
            launchmap = frame.Get("InIceRawData")
            for omkey, launches in launchmap :
                for launch in launches :
                    integral = sum([v for v in launch.raw_atwd[0]])
                    self.assert_true( integral != 0)
                    self.negLengthParticleCounter.failure_msg = "Found and empty waveform.  Integral = %f" % integral
                        
            # call the base class 'check' method
            return SanityChecker.check(self)
