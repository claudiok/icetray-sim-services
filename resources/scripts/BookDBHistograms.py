#!/usr/bin/env python
#
#  photonics example
#
from I3Tray import *

from os.path import expandvars

import os
import sys

from icecube import icetray, dataclasses, phys_services, dataio, sim_services

tray = I3Tray()

nframes = 5

from optparse import OptionParser
parser = OptionParser()

parser.add_option("-g","--gcdfile",
                  dest = "gcdfile",
                  default = "/data/icecube01/users/olivas/GeoCalibDetectorStatus_IC59.55040.i3.gz",
                  help = "Name of output i3file.")

(options, args) = parser.parse_args()

gcd_file = dataio.I3File(options.gcdfile)

frame = gcd_file.pop_frame()
while not frame.Has("I3DetectorStatus"):
    frame = gcd_file.pop_frame()

detstat = frame.Get("I3DetectorStatus")
time = detstat.endTime
tray.AddService("I3MCTimeGeneratorServiceFactory","time-gen")(
        ("Year",time.GetUTCYear()),
        ("DAQTime",time.GetUTCDaqTime())
        )

tray.AddService("I3ReaderServiceFactory","gcd")(
    ("filename",options.gcdfile),
    ("OmitEvent",True)
    )

tray.AddModule("I3Muxer","muxer")
tray.AddModule("I3DBHistogram","histo")

tray.AddModule("TrashCan", "the can")

tray.Execute(nframes)
tray.Finish()
