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
load("libI3Db")

tray = I3Tray()

nevents = 4

# The following DAQTime of 240258490029587286
# corresponds to the first event time of run 00089771

# The following DAQTime of 236184998900773063
# corresponds to the first event time of run 00089742
#MJD = 54301

tray.AddService("I3MCTimeGeneratorServiceFactory","time-gen")(
    #("Mjd",MJD),
    ("Year",2007),
    ("DAQTime",173357040000000000),
    ("RunNumber",108923)
    )

tray.AddService("I3FileOMKey2MBIDFactory","omkey2mbid")
tray.SetParameter("omkey2mbid","Infile",expandvars("$I3_BUILD/phys-services/resources/doms.txt"))

#db_host = "icedb.umh.ac.be"
#db_host = "ppemons.umh.ac.be"
db_host = "dbs2.icecube.wisc.edu"

tray.AddService("I3DbGeometryServiceFactory","geometry")(
    ("Host",db_host)
    )

tray.AddService("I3DbCalibrationServiceFactory","dbcalibration")(
    ("host",db_host)
    )

tray.AddService("I3DbDetectorStatusServiceFactory","dbdetectorstatus")(
    ("host",db_host)
    )

tray.AddModule("I3Muxer","muxer")

tray.AddModule("I3DBHistogram","histo")

tray.AddModule("I3Writer","writer")(
    ("filename", "database.i3")
     )

tray.AddModule("TrashCan", "the can")

tray.Execute(nevents)
tray.Finish()
