from icecube.icetray import I3Module
from icecube import dataclasses as dc
import os

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

from icecube.sim_services.sanity_checker.utils.configurations import RunConfigurations
if sys.version_info[0] >= 3:
    import pickle
else:
    import cPickle as pickle

class SimulationSanityChecker( I3Module ) :
    def __init__( self, context ):
        super( SimulationSanityChecker, self ).__init__(context)

        self.sanity_check_modules = list()
        self.AddParameter("RunType", "List of sanity checks to run", None )
        self.AddParameter("GenerateReferences", "Generate reference histograms", False )
        self.AddParameter("InputRefFilename", "Name of reference file", None )
        self.AddParameter("OutputRefFilename", "Name of reference file", None )
        self.AddOutBox("OutBox")

    def Configure( self ):

        # check the configurations
        # run_type and generate_references  OR
        # filename
        # unpickle loads all of the configurations
        
        self.run_type = self.GetParameter("RunType")
        self.infilename = self.GetParameter("InputRefFilename")
        self.outfilename = self.GetParameter("OutputRefFilename")

        if self.infilename :
            if os.path.exists( self.infilename ):
                f = open( self.infilename, "r" )
                self.sanity_check_modules = pickle.load( f )
                
                # resets the counters to 0
                # demotes histograms to their references
                # and resets the rest of the variables
                for scm in self.sanity_check_modules :
                    scm.reset_registry()
            else:
                print("ERROR : input filename %s does not exist" % self.filename)
                raise Exception
        else:
            # no input filename
            if not self.run_type :
                print("Need to set RunType or InputRefFilename if you're planning to generate output.")
                raise Exception

            if self.run_type not in RunConfigurations :
                print("unknown run '%s' type passed as a parameter " % self.run_type)
                print("here's a list of possible run types to choose from ( note : case is not important ) :")
                for key, value in RunConfigurations.items() :
                    print("  ", key)
                raise Exception

            # Sanity Checkers can only have a trivial ctor
            self.sanity_check_modules = [ cls() for cls in RunConfigurations[ self.run_type ] ]        


    def DAQ( self, frame ):
        all_is_well = True
        for obj in self.sanity_check_modules :            
            try :
                # needs a check method that take a frame
                sane = obj.check( frame )
                if not sane :
                    # needs a fail method
                    # this is what you do when things go wrong
                    obj.fail()
            except Exception as e :
                sane = False
                print("Sanity check %s threw the following exception" % obj.__class__.__name__)
                print(e)

            all_is_well = sane and all_is_well        

        if not all_is_well :
            print("This sample appears to have problems")
            raise Exception

        self.PushFrame( frame )

    def Finish( self ):        

        if self.outfilename :
            f = open( self.outfilename, "w")        
            pickle.dump( self.sanity_check_modules, f , pickle.HIGHEST_PROTOCOL )

