#!/usr/bin/env python
from I3Tray import *

from icecube import icetray, dataclasses, dataio, sim_services

from os.path import expandvars
import sys


class I3TweakTriggerTestModule(icetray.I3Module):

    def __init__(self, context):
        icetray.I3Module.__init__(self, context)
        self.AddParameter('SourceID', '',-1)
        self.AddParameter('TypeID', '', -1)
        self.AddParameter('ConfigID', '', -1)
        self.AddParameter('TriggerName', '', '')
        self.AddParameter('ValueNameList', '', [])
        self.AddParameter('ValueList', '', [])
        self.AddParameter('I3TriggerStatus', '', dataclasses.I3TriggerStatus())

    def Configure(self):
        self.sourceID = self.GetParameter('SourceID')
        self.typeID = self.GetParameter('TypeID')
        self.configID = self.GetParameter('ConfigID')
        self.triggerName = self.GetParameter('TriggerName')
        self.valueNameList = self.GetParameter('ValueNameList')
        self.valueList = self.GetParameter('ValueList')
        self.triggerStatus = self.GetParameter('I3TriggerStatus')

    def Physics(self, frame):
        print "Physics!!!"

        det_stat = frame.Get("I3DetectorStatus")
        trigger_status = det_stat.triggerStatus

        tkey = dataclasses.TriggerKey()
        tkey.SourceID = self.sourceID
        tkey.TypeID = self.typeID
        tkey.ConfigID = self.configID

        if not tkey in trigger_status:
            print "trigger not found in the detector status"
            print "  Source ID = ",tkey.SourceID
            print "  Type ID = ",tkey.TypeID
            print "  Config ID = ",tkey.ConfigID

            for k,v in trigger_status:
                print "%d %d %d" % (k.SourceID,k.TypeID,k.ConfigID)
            sys.exit(1)

tray = I3Tray()

gcd_file = expandvars("$I3_PORTS/test-data/sim/GeoCalibDetectorStatus_IC59.55000_candidate.i3.gz")

gcdfile = dataio.I3File(gcd_file)
frame = gcdfile.pop_frame()
while not frame.Has("I3DetectorStatus"):
    frame = gcdfile.pop_frame()

detstat = frame.Get("I3DetectorStatus")
time = detstat.endTime
tray.AddService("I3MCTimeGeneratorServiceFactory","time-gen")(
        ("Year",time.GetUTCYear()),
        ("DAQTime",time.GetUTCDaqTime())
        )

tray.AddService("I3ReaderServiceFactory","gcd")(
    ("filename",gcd_file),
    ("OmitEvent",True)
    )

ts = dataclasses.I3TriggerStatus()

tray.AddService("I3TweakTriggerService","tweak-trigg")(
    ("TweakedServiceName","I3TweakTriggerService"),
    ("SourceID",0), ## IN_ICE
    ("TypeID",0), ##SIMPLE_MULTIPLICITY
    ("ConfigID",1006), ##config for 2009
    ("TriggerName","inice_trig"),
    ("ValueNameList",["threshold","timeWindow"]),
    ("ValueList",[10,5000]),
    ("I3TriggerStatus",ts),
    )

tray.AddModule("I3Muxer","muxer")(
    ("DetectorStatusService","I3TweakTriggerService")
    )

tray.AddModule(I3TweakTriggerTestModule,"test_module",
    SourceID = 0,
    TypeID = 0, 
    ConfigID = 1006, 
    TriggerName = "inice_trig",
    ValueNameList = ["threshold","timeWindow"],
    ValueList = [10,5000]
)

tray.AddModule("TrashCan","trash")
  
tray.Execute(5)
tray.Finish()

