#!/usr/bin/env python

# we have to import here because defaults below use it
from I3Tray import I3Units 

from os.path import expandvars
from optparse import OptionParser
parser = OptionParser()

parser.add_option("-g","--gcd_file",
                  dest="gcd_file",
                  default=expandvars("$I3_BUILD/GeoCalibDetectorStatus_IC86.55697_corrected.i3.gz"),
                  help="I3File which contains the GCD.")

parser.add_option("-o","--output_path",
                  dest="output_path", default="./",
                  help="Path to store the I3File.")

parser.add_option("-f","--output_filename",
                  dest="OUTPUT_FILENAME", default="st.i3.gz",
                  help="Filename of the output I3File.")

parser.add_option("-m","--nhits_per_DOM", type = "int",
                  dest="nhits_per_DOM", default=20,
                  help="Number of hits per DOM")

parser.add_option("-t","--time_const", type = "float",
                  dest="time_const", default=0.01*I3Units.ns,
                  help="Time Constant")

parser.add_option("-b","--binhits", 
                  action="store_true", dest = "binhits", default=False,
                  help="Whether to bin the hits or not.")

parser.add_option("-d","--binwidth", type = "float",
                  dest="binwidth", default=10*I3Units.ns,
                  help="Width of the hit binning.")

parser.add_option("-n","--nevents", type="int",
                  dest="nevents", default=6,
                  help="Number of events to generate")

parser.add_option("-u","--FearTheTurtle", 
                  action="store_true", dest = "fear_the_turtle", default=False,
                  help="System has numpy, pylab, and pilfer installed.")

parser.add_option("-l","--logfile",
                  dest="LOGFILE", default="./gcd_logfile" ,
                  help="Name of logfile.")


(options, args) = parser.parse_args()

from I3Tray import I3Tray

options.binwidth *= I3Units.ns
options.time_const *= I3Units.ns

# FIXME : try the import and dump plots to a user-configured
# plot_dir (perhaps in sim-services/gcd_validation/plots)
if options.fear_the_turtle:
    import numpy
    import pylab
    # This doesn't exist anymore
    #from loot.plotting import I3Hist

import os
import sys
import math
import random

from icecube import icetray
from icecube import dataclasses
from icecube import dataio
from icecube import phys_services
from icecube import sim_services
from icecube import vuvuzela
from icecube import DOMLauncher
from icecube import WaveCalibrator
from icecube import wavedeform
from icecube.sim_services.gcd_validation.pe_generator import StressTestPEGenerator

icetray.logging.rotating_files(options.LOGFILE)

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

        # FIXME : Make me work again.
        # Just use pylab's histogram class.
        # This was written back in the day when it *really* sucked.
        # Now it only slightly sucks.
#        
#        h = I3Hist(times,range=(TMIN,TMAX),bins=nbins)
#        h.draw(label="Unbinned")
#        h.fit(hpdf,[10,0.01])
#        h.drawf(label="Fit PDF")
#        pylab.plot(t,w,label="Binned")
#        pylab.title("Test Hit Distribution")
#        pylab.xlabel("t(ns)")
#        pylab.ylabel("NHits/%2.2fns" % options.binwidth)
#        pylab.legend()
#        figpath = expandvars("$HOME/work/plots/stress-tests")
#        figname = figpath + ("/hit_dist_NPE%d_tc%3.3f_BW%2.2fns.png" % \
#                             (options.nhits_per_DOM,options.time_const,options.binwidth))
#        pylab.savefig(figname)
        
else:
    w = list()
    t = times

tray = I3Tray()

tray.AddModule("I3InfiniteSource", "source",\
               prefix = options.gcd_file , \
               stream = icetray.I3Frame.DAQ ) 

from icecube import dataio
from icecube.sim_services.sim_utils.gcd_utils import get_time
time = get_time(dataio.I3File(options.gcd_file))
tray.AddModule("I3MCEventHeaderGenerator","time-gen")(
        ("Year",time.utc_year),
        ("DAQTime",time.utc_daq_time),
        ("RunNumber",999)
        )

tray.AddService("I3SPRNGRandomServiceFactory","random")(
	("Seed",1),
	("NStreams",2),
 	("StreamNum",1))

tray.AddModule(StressTestPEGenerator ,"pes",\
               Streams = [icetray.I3Frame.DAQ],\
               hit_times = t, weights = w )

tray.AddModule("Vuvuzela","noise",
               SimulateNewDOMs = True)

tray.AddModule("PMTResponseSimulator","pmt")

tray.AddModule("DOMLauncher","domsim")

tray.AddModule('I3WaveCalibrator', 'wavecal',\
               Launches = "I3DOMLaunchSeriesMap")
   
tray.AddModule('I3Wavedeform', 'DeformInIce')

tray.AddModule("I3Writer","writer",
    filename = options.OUTPUT_FILENAME,\
    Streams = [icetray.I3Frame.TrayInfo,\
               icetray.I3Frame.Geometry,\
               icetray.I3Frame.Calibration,\
               icetray.I3Frame.DetectorStatus,\
               icetray.I3Frame.DAQ],\
    )

tray.AddModule("Dump","dump")
    
tray.AddModule("TrashCan", "the can")

tray.Execute(options.nevents)
tray.Finish()

# report all is well to the mothership
sys.exit(0)
