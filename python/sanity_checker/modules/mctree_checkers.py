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
from icecube.sim_services.sanity_checker.bases.checker import SCBaseModule

class MCTreeSCModule( SCBaseModule ) :
    def __init__(self):
        SCBaseModule.__init__(self)

        self.nanLengthTrackCounter = Counter( tolerance = 0 )
        self.noInIceMuonsCounter = Counter( tolerance = 100 )
        self.noInIceParticlesCounter = Counter( tolerance = 0 )
        self.unpropagatedTausCounter = Counter( tolerance = 0 )
        self.negLengthParticleCounter = Counter( tolerance = 0 )

        self.nanLengthTrackCounter.failure_msg = "Too many events with NAN length InIce muons."
        self.noInIceMuonsCounter.failure_msg = "Too many events with no InIce muons."
        self.noInIceParticlesCounter.failure_msg = "Too many events with no InIce particles."
        self.unpropagatedTausCounter.failure_msg = "InIce tau not propagated correctly."
        self.negLengthParticleCounter.failure_msg = "Particles should not have negative lengths."

        self.register( "nanLengthTrackCounter" )
        self.register( "noInIceMuonsCounter" )
        self.register( "noInIceParticlesCounter" )
        self.register( "unpropagatedTausCounter" )
        self.register( "negLengthParticleCounter" )

    # returns True if all is well
    def check( self, frame ):
        mctree = frame.Get("I3MCTree")

        n_inice_muons = 0
        n_inice_particles = 0
        for p in mctree :

            if p.location_type == dc.I3Particle.InIce :
                n_inice_particles += 1
            # no particles should have negative lengths
            self.negLengthParticleCounter.test_condition( (not isnan(p.length)) \
                                                          and p.length < 0 )

            if p.location_type == dc.I3Particle.InIce :
                if ( p.type == dc.I3Particle.MuPlus \
                     or p.type == dc.I3Particle.MuMinus \
                     or p.type == dc.I3Particle.TauPlus \
                     or p.type == dc.I3Particle.TauPlus ) :

                    self.nanLengthTrackCounter.test_condition( isnan(p.length) )
                    n_inice_muons += 1

                # tau specific checks
                if ( p.type == dc.I3Particle.TauPlus or \
                     p.type == dc.I3Particle.TauPlus ) :
                    if( p.energy > 1*I3Units.TeV \
                        and (p.length == 0 \
                             or len(mctree.get_daughters(p)) == 0 ) ):
                        print mctree
                        self.unpropagatedTausCounter.test_condition( True )

        self.noInIceMuonsCounter.test_condition( n_inice_muons == 0 )
        self.noInIceParticlesCounter.test_condition( n_inice_particles == 0 )

        # call the base class 'check' method
        return SCBaseModule.check(self)
