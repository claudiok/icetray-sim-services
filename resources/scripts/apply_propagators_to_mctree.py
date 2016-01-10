#!/usr/bin/env python

from optparse import OptionParser

parser = OptionParser()

parser.add_option("-i","--infile",
                  dest="INFILE", 
                  help="I3File to check")

parser.add_option("-o","--outfile",
                  dest="OUTFILE", 
                  help="I3File to write")

(options, args) = parser.parse_args()


import sys
import math
from os.path import expandvars

from I3Tray import I3Tray
from icecube import simprod_scripts

tray = I3Tray()

# set up a random number generator
randomService = phys_services.I3SPRNGRandomService(
    seed = 12345,
    nstreams = 10000,
    streamnum = 1)


tray.AddModule("I3Reader", Infile = options.INFILE)               

tray.AddSegment(simprod_scripts.PropagateMuons, 
    InputMCTreeName = "I3MCTree",
    OutputMCTreeName = "I3MCTree_new",
    RandomService = randomService
    )

tray.AddModule('I3Writer', Filename = options.OUTFILE)

tray.Execute()
tray.Finish()
