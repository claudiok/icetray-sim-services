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
tray.AddModule("I3MCEventHeaderGenerator","time-gen")(
        ("Year",2007),
        ("DAQTime", 314159)
        )

tray.AddService("I3FileOMKey2MBIDFactory","omkey2mbid")
tray.SetParameter("omkey2mbid","Infile",expandvars("$I3_BUILD/phys-services/resources/doms.txt"))

#db_host = "icedb.umh.ac.be"
#db_host = "ppemons.umh.ac.be"
db_host = "dbs2.icecube.wisc.edu"

#tray.AddService("I3DbGeometryServiceFactory","geometry")
#tray.SetParameter("geometry","CompleteGeometry",0)
#tray.SetParameter("geometry","CustomDate",0)
#tray.SetParameter("geometry","XShift",450.)
#tray.SetParameter("geometry","YShift",0.0)
#tray.SetParameter("geometry","Host",db_host)

amageofile = expandvars("$I3_BUILD/phys-services/resources/amanda.geo")
icecubegeofile = expandvars("$I3_BUILD/phys-services/resources/icecube.geo")
tray.AddService("I3TextFileGeometryServiceFactory","geometry")(
              ("AmandaGeoFile",amageofile),
              ("IceCubeGeoFile",icecubegeofile))

#stringsToUse = "21,29,30,38,39,40,49,50,59,58,67,66,74,73,65,72,78,48,57,47,46,56,64"
#stringsToUse = "21,29,30,38,39,40,49,50,59,58"

tray.AddService("I3GeometrySelectorServiceFactory","geo-selector")(
#        ("StringsToUse",stringsToUse),
        ("StringsToUse", "1:80"),
        ("GeoSelectorName","I3GeometrySelectorService")
        )

#tray.AddService("I3DbCalibrationServiceFactory","dbcalibration")
#tray.SetParameter("dbcalibration","customdate",0)
#tray.SetParameter("dbcalibration","host",db_host)

#tray.AddService("I3DbDetectorStatusServiceFactory","dbdetectorstatus")
#tray.SetParameter("dbdetectorstatus","customdate",0)
#tray.SetParameter("dbdetectorstatus","host",db_host)

#tray.AddService("I3MCSourceServiceFactory","mcsource")
#tray.SetParameter("mcsource","GeoServiceName","I3GeometrySelectorService")
#tray.SetParameter("mcsource","CalServiceName","DummyCal")
#tray.SetParameter("mcsource","StatusServiceName","DummyStat")

tray.AddModule("I3Muxer","muxer")(
    ("GeometryService","I3GeometrySelectorService"),
#    ("CalibrationService","DummyCal"),
#    ("DetectorStatusService","DummyStat")
    )
tray.AddModule("I3Writer","writer")(
    ("filename", "database.i3")
     )

tray.AddModule("TrashCan", "the can")

tray.Execute(nevents)
tray.Finish()
