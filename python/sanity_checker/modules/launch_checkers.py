from math import isnan
from icecube import dataclasses as dc
from icecube.sim_services.sanity_checker.utils.histogram import Histogram
from icecube.sim_services.sanity_checker.bases.checker import Checker
from os.path import expandvars
import cPickle as pickle

REF_PATH = expandvars("$I3_BUILD/sim-services/resources/simprod_reference_files/")
class HLCLaunchChecker( Checker ) :
    def __init__(self):
        Checker.__init__(self)

    def configure( self ):
        if self.generate_references :
            s = {"xmin": 0 , "xmax": 100 , "binwidth" : 1 } 
            self.nHLCLaunchHist = Histogram( settings = s ) 
            self.nSLCLaunchHist = Histogram( settings = s ) 
        else :
            f = open( REF_PATH + "nHLCLaunchHistRef.pickle" ,"r")
            nHLCLaunchHistRef = pickle.load( f )
            self.nHLCLaunchHist = Histogram( nHLCLaunchHistRef ) 

            f = open( REF_PATH + "nSLCLaunchHistRef.pickle" ,"r")
            nSLCLaunchHistRef = pickle.load( f )
            self.nSLCLaunchHist = Histogram( nSLCLaunchHistRef ) 

        self.register( self.nHLCLaunchHist  )
        self.register( self.nSLCLaunchHist  )        

    def check( self, frame ):
        launchmap = frame.Get("InIceRawData")

        nHLCLaunches = 0
        nSLCLaunches = 0
        for k,v in launchmap :
            nHLCLaunches += sum( [ 1 for l in v if l.lc_bit ] )
            nSLCLaunches += sum( [ 1 for l in v if not l.lc_bit ] )

        self.nHLCLaunchHist.fill( nHLCLaunches )
        self.nSLCLaunchHist.fill( nSLCLaunches )

        return Checker.check(self)

    def finish( self ):
        if self.generate_references :
            proto = pickle.HIGHEST_PROTOCOL
            f = open( REF_PATH + "nHLCLaunchHistRef.pickle" ,"w")
            pickle.dump( self.nHLCLaunchHist, f, proto )

            f = open( REF_PATH + "nSLCLaunchHistRef.pickle" ,"w")
            pickle.dump( self.nSLCLaunchHist, f, proto )

            


            

