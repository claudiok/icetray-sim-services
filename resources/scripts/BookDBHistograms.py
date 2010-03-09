#!/usr/bin/env python
#
#  photonics example
#
from I3Tray import *

from os.path import expandvars

import os
import sys

from icecube import icetray, dataclasses, phys_services, dataio, sim_services

from icecube.sim_services.sim_utils.gcd_utils import get_time

tray = I3Tray()

nframes = 5

from optparse import OptionParser
parser = OptionParser()

parser.add_option("-g","--gcdfile",
                  dest = "gcdfile",
                  default = "./database.i3",
                  help = "Name of output i3file.")

(options, args) = parser.parse_args()

gcd_file = dataio.I3File(options.gcdfile)

time = get_time(gcd_file)
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
