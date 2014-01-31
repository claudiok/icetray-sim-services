#!/usr/bin/env python

from I3Tray import I3Tray
from icecube import icetray
from icecube import dataio
from icecube import dataclasses
from icecube.sim_services.sanity_checker import I3BasicHistos

from optparse import OptionParser

parser = OptionParser()

parser.add_option("-i","--infile",
                  dest="INFILE", 
                  help="I3File to check.")

parser.add_option("-o","--outfile",
                  dest="OUTFILE", 
                  help="Output pickle file.")

parser.add_option("-r","--run_type",
                  dest="RUNTYPE", 
                  help="Type of run.")

(options, args) = parser.parse_args()


tray = I3Tray()

tray.AddModule("I3Reader","reader",
               filename = options.INFILE)

tray.AddModule( I3BasicHistos, "basic_histos",
                RunType = options.RUNTYPE,
                OutputFilename = options.OUTFILE
                )

tray.AddModule("Dump","dump")
tray.AddModule("TrashCan", "the can")
tray.Execute()
tray.Finish()

