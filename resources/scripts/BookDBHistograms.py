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

nevents = 4

tray.AddService("I3ReaderServiceFactory","i3reader")
#tray.SetParameter("i3reader","filename",expandvars("$I3_PORTS/test-data/sim/GCD.i3.gz"))
tray.SetParameter("i3reader","filename","database.i3")

tray.AddModule("I3Muxer","muxer")
tray.AddModule("I3DBHistogram","histo")

tray.AddModule("TrashCan", "the can")

tray.Execute()
tray.Finish()
