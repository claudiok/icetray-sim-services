#!/usr/bin/env python
from I3Tray import *

from os.path import expandvars

from icecube import icetray, dataclasses, dataio

def get_time(gcdfile):
    frame = gcdfile.pop_frame()
    while not frame.Has("I3DetectorStatus"): frame = gcdfile.pop_frame()

    return frame.Get("I3DetectorStatus").startTime

def get_omgeo(gcdfile):
    frame = gcdfile.pop_frame()
    while not frame.Has("I3Geometry"): frame = gcdfile.pop_frame()

    return frame.Get("I3Geometry").omgeo

def get_domcal(gcdfile):
    frame = gcdfile.pop_frame()
    while not frame.Has("I3Calibration"): frame = gcdfile.pop_frame()

    return frame.Get("I3Calibration").domCal

def get_domstatus(gcdfile):
    frame = gcdfile.pop_frame()
    while not frame.Has("I3DetectorStatus"): frame = gcdfile.pop_frame()

    return frame.Get("I3DetectorStatus").domStatus

