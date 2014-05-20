from I3Tray import I3Units
from icecube.sim_services.sanity_checker.utils.counter import Counter
from icecube.sim_services.sanity_checker.bases.sanity_checker import SanityChecker

class WFChecker( SanityChecker ) :
    def __init__(self):
        SanityChecker.__init__(self)

        self.chCounter = Counter( tolerance = 100 )
        self.registry.append( self.chCounter )

    # returns True if all is well
    def check( self, frame ):

        print frame
        
        if frame.Has("InIceRawData") :
            launches = frame["InIceRawData"]

            for k,v in launches :

                base_time = v[0].time
                sum_deltat
                for d in v :
                    sum_deltat += base_time - d.time
                    print sum_deltat

                    self.chCounter.assert_true(sum_deltat<1e-6)
                    self.chCounter.failure_msg = "Time is not correct %f " % d.time

        return SanityChecker.check(self)

