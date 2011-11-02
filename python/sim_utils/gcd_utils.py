#!/usr/bin/env python
from I3Tray import *

from os.path import expandvars

from icecube import icetray, dataclasses, dataio

def get_time(gcdfile):
    frame = gcdfile.pop_frame()
    while not frame.Has("I3DetectorStatus"): frame = gcdfile.pop_frame()

    return frame.Get("I3DetectorStatus").start_time

def get_omgeo(gcdfile):
    frame = gcdfile.pop_frame()
    while not frame.Has("I3Geometry"): frame = gcdfile.pop_frame()

    return frame.Get("I3Geometry").omgeo

def get_domcal(gcdfile):
    frame = gcdfile.pop_frame()
    while not frame.Has("I3Calibration"): frame = gcdfile.pop_frame()

    return frame.Get("I3Calibration").dom_cal

def get_domstatus(gcdfile):
    frame = gcdfile.pop_frame()
    while not frame.Has("I3DetectorStatus"): frame = gcdfile.pop_frame()

    return frame.Get("I3DetectorStatus").dom_status

def get_triggerstatus(gcdfile):
    frame = gcdfile.pop_frame()
    while not frame.Has("I3DetectorStatus"): frame = gcdfile.pop_frame()

    return frame.Get("I3DetectorStatus").trigger_status

def put_triggerstatus(ts,gcdfile,name):
    #gcdfile.rewind()
    newgcd = dataio.I3File(name,dataio.I3File.Mode.Writing)
    frame = gcdfile.pop_frame()
    #newgcd.push(frame)
    while not frame.Has("I3DetectorStatus"):
        newgcd.push(frame)
        frame = gcdfile.pop_frame()
        
    ds = frame.Get("I3DetectorStatus")
    del frame["I3DetectorStatus"]
    ds.triggerStatus = ts
    frame["I3DetectorStatus"] = ds
    newgcd.push(frame)

    return newgcd
