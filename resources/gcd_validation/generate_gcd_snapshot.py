#!/usr/bin/env python
from I3Tray import *

from icecube.dataclasses import *

import os

season_to_MJD = { "IC86" : 55750,\
		  "IC79" : 55380,\
		  "IC59" : 55000,\
		  "IC40" : 54649,\
		  }
		  

from optparse import OptionParser

parser = OptionParser()
parser.add_option("-s","--season",
                  dest="SEASON", default="IC86" ,
                  help="Season to generate (IC86, IC79, IC59, IC40)")

(options, args) = parser.parse_args()

load("libicetray")
load("libdataclasses")
load("libphys-services")
load("libdataio")
load("libI3Db")

tray = I3Tray()

dbhost = "dbs2.icecube.wisc.edu"
# dbhost = "icedb.umh.ac.be"
# dbhost = "localhost"
              
tray.AddService("I3DbOMKey2MBIDFactory","dbomkey2mbid")(
    ("host", dbhost)
    )

MJD = season_to_MJD[options.SEASON]

time = I3Time()
time.set_mod_julian_time(MJD, 0, 0.0)
print "Using simulation time:", time
print "In DAQ time, Yr", time.utc_year,"ns:",time.utc_daq_time

tray.AddModule("I3InfiniteSource","streams", Stream=icetray.I3Frame.Physics)
def seteventtime(fr):
	fr['DrivingTime'] = time
tray.AddModule(seteventtime, 'settime')

tray.AddService("I3DbGeometryServiceFactory","dbgeometry")(
    ("host", dbhost),
    ("CompleteGeometry", False),
    ("AmandaGeometry", False),
    ("DeepCore", True)
    )

tray.AddService("I3DbDetectorStatusServiceFactory","dbstatus")(
    ("host", dbhost)
    )
tray.AddService("I3DbCalibrationServiceFactory","dbcalibration")(
    ("host", dbhost)
    )

tray.AddModule("I3MetaSynth","muxme")

tray.AddModule("Dump","dump")

tray.AddModule("FrameCheck","framecheck")(
    ("ensure_physics_has",["DrivingTime",
                           "I3Geometry",
                           "I3DetectorStatus", "I3Calibration"])
    )

tray.AddModule("I3Writer","writer",
	       streams = ["Geometry","Calibration","DetectorStatus"],
               filename="GeoCalibDetectorStatus_"+options.SEASON+"."+str(MJD)+"_candidate.i3.gz")

tray.AddModule("TrashCan","trash")

tray.Execute(1)
tray.Finish()

