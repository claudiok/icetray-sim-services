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
        self.AddParameter('ReadoutWindowConfigs', '', {})

    def Configure(self):
        self.sourceID = self.GetParameter('SourceID')
        self.typeID = self.GetParameter('TypeID')
        self.configID = self.GetParameter('ConfigID')
        self.triggerName = self.GetParameter('TriggerName')
        self.valueNameList = self.GetParameter('ValueNameList')
        self.valueList = self.GetParameter('ValueList')
        self.readoutWindowConfigs = self.GetParameter('ReadoutWindowConfigs')

    def Physics(self, frame):
        print "Physics!!!"

        det_stat = frame.Get("I3DetectorStatus")
        trigger_status = det_stat.triggerStatus

        tkey = dataclasses.TriggerKey()
        tkey.Source = self.sourceID
        tkey.Type = self.typeID
        tkey.ConfigID = self.configID

        if not tkey in trigger_status:
            print "trigger not found in the detector status"
            print "  Source ID = ",tkey.Source
            print "  Type ID = ",tkey.Type
            print "  Config ID = ",tkey.ConfigID

            for k,v in trigger_status.iteritems():
                print "%d %d %d" % (k.Source,k.Type,k.ConfigID)
            sys.exit(1)

        i3ts = trigger_status[tkey]        

        for p in i3ts.GetTriggerSettings():
            print p

        ###
        # test the trigger name
        ###
        if i3ts.GetTriggerName() != self.triggerName :
            print "FAILED : trigger name %s != %s" % (i3ts.GetTriggerName(),self.triggerName)
            sys.exit(1)

        ###
        # test the trigger settings
        ###
        if len(i3ts.GetTriggerSettings()) != len(self.valueNameList) :
            print "FAILED : len settings %d %d " % \
                  (len(i3ts.GetTriggerSettings()),len(self.valueNameList))
            sys.exit(1)

        ###
        # test that the names and values are set correctly
        ###
        for name, value in zip(self.valueNameList, self.valueList):
            found = False
            valid = False
            for test_name, test_value in i3ts.GetTriggerSettings():
                if name == test_name :
                    found = True
                    if test_value != value :
                        print "FAILED : value mismatch"
                        sys.exit(1)
        
                    if not found :
                        print "FAILED : value not found"
                        sys.exit(1)
        ###
        # test the readout windows
        ###
        readouts = i3ts.GetReadoutSettings()
        print "len(readouts) = ",len(readouts)
        print "len(self.readoutWindowConfigs) = ",len(self.readoutWindowConfigs)
        if len(readouts) != len(self.readoutWindowConfigs) :
            print "FAILED : ReadoutWindowConfigs len settings %d %d " % \
                  (len(readouts),len(self.readoutWindowConfigs))
            sys.exit(1)

        for e in readouts:
            k = e.key()
            v = e.data()
            if not k in self.readoutWindowConfigs :
                print "FAILED : key %d not found readout window config"
                sys.exit(1)

            test_ro_config = self.readoutWindowConfigs[k]
            if v.readoutTimeMinus != test_ro_config.readoutTimeMinus and \
               v.readoutTimePlus != test_ro_config.readoutTimePlus and \
               v.readoutTimeOffset != test_ro_config.readoutTimeOffset :
                print "FAILED : readout window values not set properly"
                print v.readoutTimeMinus, v.readoutTimePlus, v.readoutTimeOffset
                print test_ro_config.readoutTimeMinus, test_ro_config.readoutTimePlus, test_ro_config.readoutTimeOffset,
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

ro_config = dataclasses.I3TriggerStatus.I3TriggerReadoutConfig()
ro_config.readoutTimeMinus = 10*I3Units.microsecond
ro_config.readoutTimePlus = 10*I3Units.microsecond
ro_config.readoutTimeOffset = 1*I3Units.microsecond

key = dataclasses.I3TriggerStatus.Subdetector.ALL

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
