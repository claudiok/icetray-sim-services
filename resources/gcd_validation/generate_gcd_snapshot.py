#!/usr/bin/env python
from I3Tray import I3Tray, I3Units

from icecube import dataclasses as dc

import os
from os.path import expandvars

season_to_MJD = { "2012" : 56062,\
                  "IC86" : 55697,\
                  "IC79" : 55380,\
                  "IC59" : 55000,\
                  "IC40" : 54649,\
                  }

from optparse import OptionParser

parser = OptionParser()
parser.add_option("-s","--season",
                  dest="SEASON", default="2012" ,
                  help="Season to generate (2012, IC86, IC79, IC59, IC40)")

(options, args) = parser.parse_args()

from icecube import icetray, dataclasses, phys_services, dataio, I3Db, BadDomList

tray = I3Tray()

dbhost = "dbs2.icecube.wisc.edu"
# dbhost = "icedb.umh.ac.be"
# dbhost = "localhost"

tray.AddService("I3DbOMKey2MBIDFactory","dbomkey2mbid",
    host = dbhost)

MJD = season_to_MJD[options.SEASON]

time = dc.I3Time()
time.set_mod_julian_time(MJD, 0, 0.0)
print "Using simulation time:", time
print "In DAQ time, Yr", time.utc_year,"ns:",time.utc_daq_time

tray.AddModule("I3InfiniteSource","streams", Stream=icetray.I3Frame.DAQ)
def seteventtime(fr):
    fr['DrivingTime'] = time
    evh = dc.I3EventHeader()
    evh.start_time = time
    evh.end_time = time + 10*I3Units.microsecond
    fr['I3EventHeader'] = evh

tray.AddModule(seteventtime, 'settime',
               Streams=[icetray.I3Frame.DAQ])

tray.AddService("I3DbGeometryServiceFactory","dbgeometry",
    host = dbhost,
    CompleteGeometry = False,
    AmandaGeometry = False,
    DeepCore = True)

tray.AddService("I3DbDetectorStatusServiceFactory","dbstatus",
                host = dbhost)

tray.AddService("I3DbCalibrationServiceFactory","dbcalibration",
                host = dbhost )

# get BadDomList from DB
xmlfile = expandvars('$I3_SRC/BadDomList/resources/scripts/QueryConfigurationIC86.xml')
tray.AddService('I3BadDomListFactory', 'BadDomListService',
                ServiceName = 'BadDomListService',
                Hostname = dbhost,
                Timeout = 180,
                QFileName = xmlfile,
                )

tray.AddModule("I3MetaSynth","muxme")

tray.AddModule('I3BadDomListModule', 'BadDoms',
               BadDomListServiceName = 'BadDomListService',
               CleanedKeys = list(),
               BadDomsListVector = "BadDomsList",
               CleanedKeysOnly = False,
               DisabledKeysOnly = True,
               AddGoodSlcOnlyKeys  = True,
               )

tray.AddModule('I3BadDomListModule', 'BadDomsSLC',
               BadDomListServiceName = 'BadDomListService',
               CleanedKeys = list(),
               BadDomsListVector = "BadDomsListSLC",
               CleanedKeysOnly = False,
               DisabledKeysOnly = True,
               AddGoodSlcOnlyKeys  = False,
               )

tray.AddModule("FrameCheck","framecheck",
               ensure_physics_has = ["DrivingTime",
                                     "I3Geometry",
                                     "I3DetectorStatus",
                                     "I3Calibration"] )

fn = "GeoCalibDetectorStatus_"+options.SEASON+"."+str(MJD)+"_candidate.i3.gz"
tray.AddModule("I3Writer","writer",
               streams = [icetray.I3Frame.Geometry,
                          icetray.I3Frame.Calibration,
                          icetray.I3Frame.DetectorStatus],
               filename= fn )

tray.AddModule("Dump","dump")

tray.AddModule("TrashCan","trash")

tray.Execute(1)
tray.Finish()
