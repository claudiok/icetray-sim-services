#!/usr/bin/env python
from I3Tray import *

from icecube import icetray, dataclasses, dataio, sim_services

from os.path import expandvars
import sys

from icecube.sim_services.test_modules.tweak_trigger_test_module import I3TweakTriggerTestModule

tray = I3Tray()

gcd_file = expandvars("$I3_PORTS/test-data/sim/GeoCalibDetectorStatus_IC59.55000_candidate.i3.gz")

gcdfile = dataio.I3File(gcd_file)
frame = gcdfile.pop_frame()
while not frame.Has("I3DetectorStatus"):
    frame = gcdfile.pop_frame()

detstat = frame.Get("I3DetectorStatus")
time = detstat.endTime
tray.AddModule("I3MCEventHeaderGenerator","time-gen")(
        ("Year",time.GetUTCYear()),
        ("DAQTime",time.GetUTCDaqTime())
        )

tray.AddService("I3ReaderServiceFactory","gcd")(
    ("filename",gcd_file),
    ("OmitEvent",True)
    )

ro_config = dataclasses.I3TriggerStatus.I3TriggerReadoutConfig()
ro_config.readoutTimeMinus = 10*I3Units.microsecond
ro_config.readoutTimePlus = 10*I3Units.microsecond
ro_config.readoutTimeOffset = 1*I3Units.microsecond

ro_config_map = dataclasses.I3TriggerStatus.map_Subdetector_I3TriggerReadoutConfig()
ro_config_map[dataclasses.I3TriggerStatus.Subdetector.ALL] = ro_config

tray.AddService("I3TweakTriggerService","tweak-trigg")(
    ("TweakedServiceName","I3TweakTriggerService"),
    ("SourceID",dataclasses.I3Trigger.SourceID.IN_ICE), 
    ("TypeID",dataclasses.I3Trigger.TypeID.SIMPLE_MULTIPLICITY),
    ("ConfigID",1006), ##config for 2009
    ("TriggerName","SMT10"),
    ("ValueNameList",["threshold","timeWindow"]),
    ("ValueList",[10,4000]),
    ("ReadoutConfigMap",ro_config_map)
    )

tray.AddModule("I3Muxer","muxer")(
    ("DetectorStatusService","I3TweakTriggerService")
    )

tray.AddModule(I3TweakTriggerTestModule,"test_module",
    SourceID = dataclasses.I3Trigger.SourceID.IN_ICE,
    TypeID = dataclasses.I3Trigger.TypeID.SIMPLE_MULTIPLICITY, 
    ConfigID = 1006, 
    TriggerName = "SMT10",
    ValueNameList = ["threshold","timeWindow"],
    ValueList = [10,4000],
    ReadoutWindowConfigs = ro_config_map       
)

tray.AddModule("TrashCan","trash")
  
tray.Execute(5)
tray.Finish()

print "PASSED"
