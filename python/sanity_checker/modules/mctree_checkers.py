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

class InIceMCTreeChecker( SanityChecker ) :
    def __init__(self):
        SanityChecker.__init__(self)

        self.emptyTreeCounter = Counter( tolerance = 0 )
        self.negLengthTrackCounter = Counter( tolerance = 0 )
        self.nanLengthTrackCounter = Counter( tolerance = 0 )
        self.noInIceMuonsCounter = Counter( tolerance = 100 )
        self.noInIceParticlesCounter = Counter( tolerance = 0 )
        self.unpropagatedTausCounter = Counter( tolerance = 0 )

        self.emptyTreeCounter.failure_msg = "Too many events with empty trees."
        self.nanLengthTrackCounter.failure_msg = "Too many events with NAN length InIce muons."
        self.negLengthTrackCounter.failure_msg = "Particles should not have negative lengths."
        self.noInIceMuonsCounter.failure_msg = "Too many events with no InIce muons."
        self.noInIceParticlesCounter.failure_msg = "Too many events with no InIce particles."
        self.unpropagatedTausCounter.failure_msg = "InIce tau not propagated correctly."

        self.registry.append(self.emptyTreeCounter)
        self.registry.append(self.nanLengthTrackCounter)
        self.registry.append(self.negLengthTrackCounter)
        self.registry.append(self.noInIceMuonsCounter)
        self.registry.append(self.noInIceParticlesCounter)
        self.registry.append(self.unpropagatedTausCounter)

    # returns True if all is well
    def check( self, frame ):
        mctree = frame.Get("I3MCTree")

        self.emptyTreeCounter.assert_true( len(mctree) > 0 )
        
        n_inice_muons = 0
        n_inice_particles = 0
        for p in mctree :

            if p.location_type == dc.I3Particle.InIce :
                n_inice_particles += 1
                if ( p.type == dc.I3Particle.MuPlus \
                     or p.type == dc.I3Particle.MuMinus \
                     or p.type == dc.I3Particle.TauPlus \
                     or p.type == dc.I3Particle.TauPlus ) :

                    if not isnan(p.length) and p.length < 0 :
                        self.negLengthTrackCounter.assert_true(False)
                        self.negLengthTrackCounter.failure_msg += "\n  %s" % p
                        
                    self.nanLengthTrackCounter.assert_true( not isnan(p.length) )
                    n_inice_muons += 1

                # tau specific checks
                if ( p.type == dc.I3Particle.TauPlus or \
                     p.type == dc.I3Particle.TauPlus ) :
                    if( p.energy > 1*I3Units.TeV \
                        and (p.length == 0 or len(mctree.get_daughters(p)) == 0 ) ):                             
                        print(mctree)
                        print(p)
                        self.unpropagatedTausCounter.failure_msg += "\n"
                        self.unpropagatedTausCounter.failure_msg += "  This test fails if InIce taus above 1TeV have no daughters or 0 length."
                        self.unpropagatedTausCounter.failure_msg += "    tau energy = %f TeV\n" % (p.energy/I3Units.TeV)
                        self.unpropagatedTausCounter.failure_msg += "    tau length = %f m\n" % (p.length/I3Units.m)
                        self.unpropagatedTausCounter.failure_msg += "    n daughters = %d\n" % len(mctree.get_daughters(p))
                        self.unpropagatedTausCounter.assert_true(False)
                        

        self.noInIceMuonsCounter.assert_true( n_inice_muons > 0 )
        self.noInIceParticlesCounter.assert_true( n_inice_particles > 0 )

        # call the base class 'check' method
        return SanityChecker.check(self)

class IceTopMCTreeChecker( SanityChecker ) :
    def __init__(self):
        SanityChecker.__init__(self)

    # returns True if all is well
    def check( self, frame ) : 
        # call the base class 'check' method
        return SanityChecker.check(self)
