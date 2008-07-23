#!/usr/bin/env python
#
#  photonics example
#
from I3Tray import *

from os.path import expandvars

import os
import sys

load("libdataclasses")
load("libphys-services")
load("libdataio")
load("libsim-services")

tray = I3Tray()

nframes = 5

tray.AddService("I3ReaderServiceFactory","i3reader")(
    ("filename",expandvars("$I3_BUILD/GCD07082008.i3.gz")),
    ("OmitEvent", True),
    )
#tray.SetParameter("i3reader","filename",expandvars("$I3_PORTS/test-data/sim/GCD.i3.gz"))
#tray.SetParameter("i3reader","filenamelist",[expandvars("$I3_PORTS/test-data/sim/GCD_IC40_IT6_AM_20080306.i3.gz"), \
#                                             "/data/icecube01/users/olivas/SUSY-staus/single_staus/single_stau.i3.gz", ] )
#tray.SetParameter("i3reader","filename","database.i3")

tray.AddService("I3MCTimeGeneratorServiceFactory","time")(
    ("Year",2008),
    ("DAQTime",163163250000000000)
    )

tray.AddModule("I3Muxer","muxer")
tray.AddModule("I3DBHistogram","histo")

tray.AddModule("TrashCan", "the can")

tray.Execute(nframes)
tray.Finish()
