#!/usr/bin/env python

from optparse import OptionParser

parser = OptionParser()

parser.add_option("-i","--infile",
                  dest="INFILE", 
                  help="I3File to check")

parser.add_option("-o","--outfile",
                  dest="OUTFILE", 
                  help="I3File to write")

parser.add_option("-g","--gcdfile",
                  dest="GCDFILE", 
                  help="path to GCD file")

parser.add_option("-p","--path_to_i3files",
                  dest="INPATH", 
                  help="path to I3Files")

parser.add_option("-n","--nevents", default = None,
                  dest="NEVENTS", type = 'int',
                  help="number of events to check")

(options, args) = parser.parse_args()

import os
import sys
from os.path import expandvars

from I3Tray import I3Tray
from icecube import icetray, dataclasses, dataio, simclasses

#icetray.set_log_level_for_unit('I3Reader',icetray.I3LogLevel.LOG_TRACE)

if not options.GCDFILE :
        print ("please specify a GCD file.")
        sys.exit()

if not options.INFILE and not options.INPATH :
	print("please specify either a path or file.")
	sys.exit()

if options.INFILE and options.INPATH :
	print("please specify a path or file, but not both.")
	sys.exit()

filelist = [ options.GCDFILE ]
if options.INFILE :
	filelist.append( options.INFILE )

if options.INPATH :
	if not options.INPATH.endswith("/") :
		options.INPATH += "/"
	for fn in os.listdir( options.INPATH ) :
		if fn.rfind(".i3") > 0 and fn.find("GeoCalibDetectorStatus") < 0 :
			# this should be a valid I3File
			filelist.append( options.INPATH + fn )

tray = I3Tray()

tray.AddModule("I3Reader", "read",
	       filenamelist = filelist )

from os.path import expandvars
from icecube.sim_services.sanity_checker import SimulationSanityChecker
tray.AddModule( SimulationSanityChecker, "sanitycheck",
		RunType = "CORSIKA_Weighted" ,
        Prescale = 10,
		OutputFilename = options.OUTFILE )

if options.NEVENTS :
	tray.Execute(options.NEVENTS)
else:
	print(" calling execute")
	tray.Execute()

tray.Finish()
