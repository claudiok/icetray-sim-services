from icecube.icetray import I3Module
from icecube import dataclasses as dc

import sys
# there's really no such thing
# as an abstract base class in python
# this is really just a guide
#   class SanityCheckStub :
#   
#       def __init__(self):
#           print "implement me"
#   
#       # return True if all is OK
#       def check(self, frame ):
#           print "implement me"
#           return True
#   
#       def fail( self ) :
#           print "something went horribly wrong."

from utils.configurations import RunConfigurations

class SimulationSanityChecker( I3Module ) :
    def __init__( self, context ):
        super( SimulationSanityChecker, self ).__init__(context)

        self.list_of_sanity_checks = list()
        self.AddParameter("RunType", "List of sanity checks to run", None )
        self.AddParameter("GenerateReferences", "Generate reference histograms", False )
        self.AddOutBox("OutBox")

    def Configure( self ):

        self.run_type = self.GetParameter("RunType")
        self.generate_references = self.GetParameter("GenerateReferences")
        
        if self.run_type not in RunConfigurations :
            print "unknown run '%s' type passed as a parameter " % self.run_type
            print "here's a list of possible run types to choose from ( note : case in not important ) :"
            for key, value in RunConfigurations.iteritems() :
                print "  ", key
            raise Exception
            
        self.list_of_sanity_checks = [ cls() for cls in RunConfigurations[ self.run_type ] ]        

        for checker in self.list_of_sanity_checks :            
            checker.generate_references = self.generate_references
            checker.configure()            
    
    def DAQ( self, frame ):
        all_is_well = True
        for obj in self.list_of_sanity_checks :            
            try :
                sane = obj.check( frame )
                if not sane :
                    obj.fail()
            except Exception, e :
                sane = False
                print "Sanity check %s threw the following exception" % obj.__class__.__name__
                print e

            all_is_well = sane and all_is_well        

        if not all_is_well :
            print "This sample appears to have problems"
            raise Exception

        self.PushFrame( frame )

    def Finish( self ):        
        for obj in self.list_of_sanity_checks :
            obj.finish()

