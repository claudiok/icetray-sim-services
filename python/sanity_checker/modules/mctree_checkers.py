from math import isnan
from icecube import dataclasses as dc

from icecube.sim_services.sanity_checker.utils.counter import Counter
from icecube.sim_services.sanity_checker.bases.checker import Checker

class CorsikaTreeChecker( Checker ) :
    def __init__(self):
        Checker.__init__(self)

    def configure(self):
        # if we get 10 events in a row flag an error
        self.missingStochasticCounter = Counter( ) 
        self.daughterlessCounter = Counter( ) 
        self.nanLengthMuonCounter = Counter( ) 
        self.noInIceMuonsCounter = Counter( ) 

        self.missingStochasticCounter.failure_msg = "Too many events with InIce muons missing stochastics."
        self.daughterlessCounter.failure_msg = "Too many events with daughterless InIce muons."
        self.nanLengthMuonCounter.failure_msg = "Too many events with NAN length InIce muons."
        self.noInIceMuonsCounter.failure_msg = "Too many events with daughterless InIce muons."

        self.register( self.missingStochasticCounter  )
        self.register( self.daughterlessCounter  )
        self.register( self.nanLengthMuonCounter  )
        self.register( self.noInIceMuonsCounter  )
        
    # returns True if all is well
    def check( self, frame ):
        mctree = frame.Get("I3MCTree")

        n_inice_muons = 0
        for p in mctree :
            # only consider inice muons
            if ( p.type == dc.I3Particle.MuPlus or \
                 p.type == dc.I3Particle.MuPlus ) and \
                 p.location_type == dc.I3Particle.InIce :

                self.nanLengthMuonCounter.test_condition( isnan(p.length) )

                n_inice_muons += 1
                # now check that the muon was propagated correctly
                stochastics = mctree.get_daughters( p )

                self.daughterlessCounter.test_condition( len(stochastics) == 0 )

                no_cascades = True
                for s in stochastics :
                    if s.is_cascade :
                        no_cascades = False
                        break

                self.missingStochasticCounter.test_condition( no_cascades )

        self.noInIceMuonsCounter.test_condition( n_inice_muons == 0 )

        # call the base class 'check' method
        return Checker.check(self)

            

