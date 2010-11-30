#!/usr/bin/env python
from I3Tray import *

from os.path import expandvars

import os
import sys
import math
import random

from optparse import OptionParser
parser = OptionParser()

parser.add_option("-g","--gcd_file",
                  dest="gcd_file", default=expandvars("$I3_PORTS/test-data/sim/GeoCalibDetectorStatus_IC59.55040.i3.gz"),
                  help="I3File which contains the GCD.")

parser.add_option("-o","--output_path",
                  dest="output_path", default="./",
                  help="Path to store the I3File.")

parser.add_option("-f","--output_filename",
                  dest="output_filename", default="st.i3.gz",
                  help="Filename of the output I3File.")

parser.add_option("-m","--nhits_per_DOM", type = "int",
                  dest="nhits_per_DOM", default=10000,
                  help="Number of hits per DOM")

parser.add_option("-l","--time_const", type = "float",
                  dest="time_const", default=0.01*I3Units.ns,
                  help="Time Constant")

parser.add_option("-b","--binhits", 
                  action="store_true", dest = "binhits", default=False,
                  help="Whether to bin the hits or not.")

parser.add_option("-d","--binwidth", type = "float",
                  dest="binwidth", default=10*I3Units.ns,
                  help="Width of the hit binning.")

parser.add_option("-n","--nevents", type="int",
                  dest="nevents", default=5,
                  help="Number of events to generate")

parser.add_option("-u","--FearTheTurtle", 
                  action="store_true", dest = "fear_the_turtle", default=False,
                  help="System has numpy, pylab, and pilfer installed.")

(options, args) = parser.parse_args()

options.binwidth *= I3Units.ns
options.time_const *= I3Units.ns

if options.fear_the_turtle:
    import numpy
    import pylab
    from loot.plotting import I3Hist

###
# Generate the hit series to feed to I3TestGenericSource
###
TMIN = 0.*I3Units.microsecond
TMAX = 2.*I3Units.microsecond

if options.fear_the_turtle:
    hpdf = lambda b,x : b[0] * x * numpy.exp(-b[1]*x)
    x = numpy.arange(TMIN,TMAX,0.01*I3Units.ns)
    max_y = max(hpdf([1,options.time_const],x))
else:
    hpdf = lambda b,x : b[0] * x * math.exp(-b[1]*x)
    x = [TMIN + i*0.01*I3Units.ns for i in range(int((TMAX-TMIN)/0.01*I3Units.ns))]
    max_y = max([hpdf([1,options.time_const],xi) for xi in x])


times = list()
for i in range(options.nhits_per_DOM):
    x = random.uniform(TMIN,TMAX)
    y = random.uniform(0,max_y)
    while y > hpdf([1,options.time_const],x):
        x = random.uniform(TMIN,TMAX)
        y = random.uniform(0,max_y)
    times.append(x)

if options.binhits :
    nbins = int(float(TMAX-TMIN)/options.binwidth)

    if not options.fear_the_turtle:
        w = [0 for i in range(nbins)]
        t = [TMIN + (i + 0.5)*(options.binwidth) for i in range(nbins)]
        times.sort()
        for ht in times:
            if ht>=TMIN and ht<TMAX:                
                bin = int(nbins * (ht-TMIN)/(TMAX-TMIN))
                w[bin] += 1
    else:
        w,le = numpy.histogram(times,range=(TMIN,TMAX),bins=nbins)
        t = [t0+(options.binwidth/2.0) for t0 in le]

        h = I3Hist(times,range=(TMIN,TMAX),bins=nbins)
        h.draw(label="Unbinned")
        h.fit(hpdf,[10,0.01])
        h.drawf(label="Fit PDF")
        pylab.plot(t,w,label="Binned")
        pylab.title("Test Hit Distribution")
        pylab.xlabel("t(ns)")
        pylab.ylabel("NHits/%2.2fns" % options.binwidth)
        pylab.legend()
        figpath = expandvars("$HOME/work/plots/stress-tests")
        figname = figpath + ("/hit_dist_NPE%d_tc%3.3f_BW%2.2fns.png" % \
                             (options.nhits_per_DOM,options.time_const,options.binwidth))
        pylab.savefig(figname)
        
else:
    w = list()
    t = times

load("libdataclasses")
load("libphys-services")
load("libsim-services")
load("libdataio")
load("libpmt-simulator")
load("libromeo-interface")
load("libDOMsimulator")
load("libDOMcalibrator")
load("libNFE")

tray = I3Tray()

tray.AddService("I3ReaderServiceFactory","gcd")(
    ("filename",options.gcd_file),
    ("OmitEvent",True)
    )

from icecube import dataio
from icecube.sim_services.sim_utils.gcd_utils import get_time
time = get_time(dataio.I3File(options.gcd_file))
tray.AddService("I3MCTimeGeneratorServiceFactory","time-gen")(
        ("Year",time.GetUTCYear()),
        ("DAQTime",time.GetUTCDaqTime()),
        ("RunNumber",999)
        )

ic22 = "21,29,30,38,39,40,49,50,59,58,67,66,74,73,65,72,78,48,57,47,46,56"
ic2008= "63,64,55,71,70,76,77,75,69,60,68,61,62,52,44,53,54,45"
ic2009 = "2,3,4,5,6,10,11,12,13,17,18,19,20,26,27,28,36,37,83"
ic2010 = "8,9,16,25,35,85,84,82,81,86,43,34,24,15,23,33,42,51,32,41"

it79 = "2,3,4,5,6,9,10,11,12,13,17,18,19,20,21,26,27,28,29,30,36,37,38,"
it79 += "39,40,44,45,46,47,48,49,50,52,53,54,55,56,57,58,59,60,61,62,63,"
it79 += "64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,"
it79 += "8,16,25,35,43,34,24,15,23,33,42,51,32,41"
tray.AddService("I3GeometrySelectorServiceFactory","geo-selector")(
        ("StringsToUse",ic22 + "," + ic2008 + "," + ic2009 + "," + ic2010),
        ("StationsToUse",it79),
        ("GeoSelectorName","IC79-Geo")
        )

tray.AddService("I3SPRNGRandomServiceFactory","random")(
	("Seed",1),
	("NStreams",2),
 	("StreamNum",1))

tray.AddModule("I3Muxer","muxer")(
    ("GeometryService","IC79-Geo")
    )

tray.AddModule("I3TestGenericSource","hits")(
    ("HitTimes",t),
    ("Weights",w)
    )

tray.AddModule("I3PMTSimulator","pmt")

tray.AddModule("I3DOMsimulator","domsim")

tray.AddModule("I3DOMcalibrator","domcal")

tray.AddModule( "I3NFE", "NFE" )(
    ("InputWaveformName","CalibratedATWD")
    )

fn = options.output_path 
fn += "IC79_"
fn += "n%d_" % options.nhits_per_DOM
fn += "tc%3.3f_" % options.time_const
if options.binhits :
    fn += "dt%2.2f_" % options.binwidth
fn += options.output_filename

tray.AddModule("I3Writer","writer")(
    ("filename", fn),
    ("Streams",["Physics"]),
    ("SkipKeys",["InIceRawData",
                 "IceTopRawData",
                 "ATWDPortiaPulse",
                 "PortiaEvent",
                 "MCHitSeriesMap",
                 "InitialHitSeriesReco",
                 "I3EventHeader",
                 "FADCPortiaPulse",
                 "FADCPulseSeries"])
    )

tray.AddModule("TrashCan", "the can")

tray.Execute(options.nevents)
tray.Finish()
