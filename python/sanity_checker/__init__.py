import os
import sys

from icecube.icetray import I3Module
from icecube import dataclasses as dc
from .utils.configurations import RunConfigurations
from .utils.configurations import HistogramConfigurations

if sys.version_info[0] >= 3:
    import pickle
else:
    import cPickle as pickle

class SimulationSanityChecker( I3Module ) :
    def __init__( self, context ):
        super( SimulationSanityChecker, self ).__init__(context)

        self.sanity_check_modules = list()
        self.AddParameter("RunType", "Type of run to check", None )
        self.AddParameter("OutputFilename", "Name of output pickle file", None )
        # FIXME : What should be the default of the data livetime?
        self.AddParameter("HistoDataLiveTime", "Live time of the data", 1 )
        self.AddParameter("Prescale", "Prescale", 1 )
        self.AddOutBox("OutBox")

        self._daq_frame_counter = 0

    def Configure( self ):

        self.run_type = self.GetParameter("RunType").lower()
        self.outfilename = self.GetParameter("OutputFilename")
        self.datalivetime = self.GetParameter("HistoDataLiveTime")
        self.prescale = self.GetParameter("Prescale")

        # no input filename
        if not self.run_type :
            print("Need to set RunType or InputRefFilename if you're planning to generate output.")
            raise Exception

        if self.run_type not in RunConfigurations :
            print("Unknown RunType '%s' type passed as a parameter " % self.run_type)
            print("here's a list of possible run types to choose from ( note : case is not important ) :")
            for key, value in RunConfigurations.iteritems() :
                print("  %s" % key)
            raise Exception

        self.sanity_check_modules = [ obj for obj in RunConfigurations[ self.run_type ] ]        

        # Generate the dictionary of histograms, where the key is the histogram's title
        self.histograms = dict()
        for h in HistogramConfigurations[ self.run_type ] :
            self.histograms[h.name] = h


    def DAQ( self, frame ):
        self._daq_frame_counter += 1
        if self._daq_frame_counter % self.prescale != 0 :
            self.PushFrame( frame )
            return        

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

        frame["DataLiveTime"] = dc.I3Double(self.datalivetime)
        for t,h in self.histograms.iteritems() :
            h.fill(frame)
        del frame["DataLiveTime"]

        self.PushFrame( frame )

    def Finish( self ):        

        for t,h in self.histograms.iteritems() :
            h.generate_histogram()

        if self.outfilename :
            f = open( self.outfilename, "w")        
            pickle.dump( {"Modules" : self.sanity_check_modules,\
                          "Histograms": self.histograms},\
                          f , pickle.HIGHEST_PROTOCOL )


