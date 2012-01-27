#!/usr/bin/env python

import os
from os.path import expandvars

from I3Tray import I3Tray
from icecube import icetray, dataclasses, dataio, simclasses

from optparse import OptionParser

parser = OptionParser()

parser.add_option("-i","--infile",
                  dest="INFILE", 
                  help="I3File to check")

parser.add_option("-g","--gcdfile",
                  dest="GCDFILE", 
                  help="path to GCD file")

parser.add_option("-p","--path_to_i3files",
                  dest="INPATH", 
                  help="path to I3Files")

parser.add_option("-n","--nevents", default = None,
                  dest="NEVENTS", 
                  help="number of events to check")

(options, args) = parser.parse_args()

if options.INFILE and options.INPATH :
	print "please specify a path or file, but not both"
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

print filelist
tray.AddModule("I3Reader", "read",
	       filenamelist = filelist )

from icecube.sim_services.sanity_checker import SimulationSanityChecker
tray.AddModule( SimulationSanityChecker, "sanitycheck",
		RunType = "CORSIKA" ,
		GenerateReferences = False )

tray.AddModule("Dump", "dump")
tray.AddModule("TrashCan", "the can")

print options.NEVENTS
if options.NEVENTS :
	tray.Execute(options.NEVENTS)
else:
	print " calling execute"
	tray.Execute()

tray.Finish()
