#!/usr/bin/env python

import unittest

from I3Tray import I3Tray
from icecube import icetray
from icecube import dataclasses
from icecube import sim_services

class Source(icetray.I3Module):
    def __init__(self, context):
        icetray.I3Module.__init__(self, context)
        self.AddOutBox("OutBox")

    def Configure(self):
        pass

    def Process(self):
        frame = icetray.I3Frame(icetray.I3Frame.DAQ)        
        event_header = dataclasses.I3EventHeader()
        frame["I3EventHeader"] = event_header
        self.PushFrame(frame)
        

class TestI3ModifyStartTime(unittest.TestCase):

    def setUp(self):
        self.tray = I3Tray()
        self.tray.AddModule(Source)

    def test_modify_start_time(self):
                
        self.tray.AddModule("I3ModifyStartTime",
                            StartTime = dataclasses.I3Time(53005))

        def TestModule(frame):
            event_start_time = frame["I3EventHeader"].start_time
            self.assertNotEqual(event_start_time, dataclasses.I3EventHeader().start_time)
            
            
        self.tray.AddModule(TestModule, streams = [icetray.I3Frame.DAQ])

        self.tray.Execute(1)

    def test_failure(self):            
        self.tray.AddModule("I3ModifyStartTime")
        self.assertRaises(RuntimeError, self.Execute)

    def Execute(self):
        self.tray.Execute(1)
                
if __name__ == '__main__':
    unittest.main()
