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
    
from icecube import dataclasses as dc
from icecube.sim_services.sanity_checker.utils.histogram import Histogram
from icecube.sim_services.sanity_checker.bases.checker import SCBaseModule
from os.path import expandvars

class HLCLaunchSCModule( SCBaseModule ) :
    def __init__(self):
        SCBaseModule.__init__(self)

        s = { "xmin": 0 , "xmax": 100 , "binwidth" : 1 } 
        self.nHLCLaunchHist = Histogram( settings = s ) 
        self.nSLCLaunchHist = Histogram( settings = s ) 

        # only register on generation
        self.register( "nHLCLaunchHist" )
        self.register( "nSLCLaunchHist" ) 

    def check( self, frame ):
        launchmap = frame.Get("InIceRawData")

        nHLCLaunches = 0
        nSLCLaunches = 0
        for k,v in launchmap :
            nHLCLaunches += sum( [ 1 for l in v if l.lc_bit ] )
            nSLCLaunches += sum( [ 1 for l in v if not l.lc_bit ] )

        self.nSLCLaunchHist.fill( nSLCLaunches )
        self.nHLCLaunchHist.fill( nHLCLaunches )

        return SCBaseModule.check(self)



        


            

