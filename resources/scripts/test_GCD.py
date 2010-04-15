#!/usr/bin/env python

import sys, os
from os.path import expandvars

from I3Tray import *
from icecube import icetray, dataclasses, dataio, simclasses

from icecube.BadDomList import bad_dom_list_static
badOMs = bad_dom_list_static.IC59_static_bad_dom_list()

from optparse import OptionParser

parser = OptionParser()

parser.add_option("-g","--gcdfile",
                  dest="GCDFILE", default=expandvars("$I3_PORTS/test-data/sim/GeoCalibDetectorStatus_IC59.55040.i3.gz"),
                  help="GCD file to test.")

(options, args) = parser.parse_args()


gcdfile = dataio.I3File(options.GCDFILE)

frame = gcdfile.pop_frame()
while not frame.Has('I3Geometry'): frame = gcdfile.pop_frame()
geometry = frame.Get('I3Geometry')

while not frame.Has('I3Calibration'): frame = gcdfile.pop_frame()
calibration = frame.Get('I3Calibration')

while not frame.Has('I3DetectorStatus'): frame = gcdfile.pop_frame()
status = frame.Get('I3DetectorStatus')

dom_geo = geometry.omgeo
dom_cal = calibration.domCal
dom_status = status.domStatus

geo_strings_to_check = range(1,87)
for s in geo_strings_to_check:
	found = False
	for e,p in dom_geo:
		if e.GetString() == s :
			found = True
	if not found:
		print "string %d does not exist in the geometry" % s
	
c_and_d_strings_to_check = [ 
	75, 76, 77, 78, 68, 69, 70, 71, 72, 73,
	74, 60, 61, 62, 63, 64, 65, 66, 67, 52,
	53, 54, 55, 56, 57, 58, 59, 44, 45, 46,
	47, 48, 49, 50, 36, 83, 37, 38, 39, 40,
	26, 27, 28, 29, 30, 17, 18, 19, 20, 21,
	10, 11, 12, 13,  2,  3,  4,  5,  6
	]

for s in c_and_d_strings_to_check :
	found_cal = False
	found_stat = False
	for omkey in [icetray.OMKey(s,om) for om in range(61)] :
		if omkey in dom_cal :
			found_cal = True
		if omkey in dom_status :
			found_stat = True
		if found_cal and found_stat : continue
	if not found_cal : print 'string %s is missing from the calibration' % s
	if not found_stat : print 'string %s is missing from the detector status' % s

for e,p in dom_geo:

	if e in badOMs and e in dom_cal and e in dom_status:
		print "DOM %s is in the bad DOM list but contains status and calibration records" % str(e)
	
	if e not in badOMs and e in dom_cal and e in dom_status:				
		cal_this_om = dom_cal[e]
		status_this_om = dom_status[e]

		# checks for hit-maker
		if ( status_this_om.pmtHV < 600*I3Units.V or \
		     status_this_om.pmtHV > 1600*I3Units.V ) :
			print '  %s  pmtHV = %s V !!' % (str(e), status_this_om.pmtHV/I3Units.V)

		if ( ( cal_this_om.RelativeDomEff != 1 and e.GetString() <= 80 and e.GetOM() <= 60) or \
		     ( cal_this_om.RelativeDomEff != 1.25 and e.GetString() > 80 ) ):
			print '  %s  RelativeDomEff = %s !!' % (str(e), cal_this_om.RelativeDomEff)
			
	
		# checks for noise-generator
		noiseRate = cal_this_om.DomNoiseRate * I3Units.second
		if ((not (noiseRate > 300 and noiseRate < 800)) and (e.GetOM() >= 1 and e.GetOM() <= 60)):
			print '  %s  noise rate = %s !!' % (str(e), noiseRate)


      # checks for pmt-simulator
		pmtGain = dataclasses.PMTGain(status_this_om, cal_this_om) / 1.e7
		if ((not (pmtGain > 0.5 and pmtGain < 3.0)) and (e.GetOM() >= 1 and e.GetOM() <= 60)):
			print '  %s  pmtGain = %s !!' % (str(e), pmtGain)

		impedence = cal_this_om.FrontEndImpedance / I3Units.ohm
		if ((not (impedence == 43 or impedence == 50)) and (e.GetOM() >= 1 and e.GetOM() <= 60)):
			print '  %s  impedence = %s !!' % (str(e), impedence)

		# Checking TauParameters
#		if ((not ((cal_this_om.TauParameters.P0 == 10960 and cal_this_om.TauParameters.P1 == 56665 and
#						cal_this_om.TauParameters.P2 == 6.5 and cal_this_om.TauParameters.P3 == 500 and
#						cal_this_om.TauParameters.P4 == 0 and cal_this_om.TauParameters.P5 == 1 and
#						cal_this_om.TauParameters.TauFrac == -0.5) or (cal_this_om.TauParameters.P0 == 400
#							and cal_this_om.TauParameters.P1 == 5000 and cal_this_om.TauParameters.P2 == 16
#							and cal_this_om.TauParameters.P3 == 400 and cal_this_om.TauParameters.P4 ==
#							5000 and cal_this_om.TauParameters.P5 == 16 and
#						cal_this_om.TauParameters.TauFrac == -3.3))) and (e.GetOM() >= 1 and e.GetOM() <= 60)):
		print '%s %s   %s  %s  %s  %s  %s  %s' % (str(e), cal_this_om.TauParameters.P0, 
														cal_this_om.TauParameters.P1,
														cal_this_om.TauParameters.P2,
														cal_this_om.TauParameters.P3,
														cal_this_om.TauParameters.P4,
														cal_this_om.TauParameters.P5,
														cal_this_om.TauParameters.TauFrac)

		transitTime = dataclasses.TransitTime(status_this_om, cal_this_om);
		if ((not (transitTime > 130 and transitTime < 151)) and (e.GetOM() >= 1 and e.GetOM() <= 60)):
			print '%s  transitTime = %s !!' % (str(e), transitTime)
		
		# checks for DOMsimulator
		threshold = dataclasses.SPEPMTThreshold(status_this_om,
							cal_this_om) / I3Units.mV
		if threshold < 0 :
			print '  %s  %f' % (str(e), threshold)

		# checks for DOMcalibrator
		if cal_this_om.DOMCalVersion != "7.4.0" :
			print '  %s  DOMCalVersion = %s !!' % (str(e), cal_this_om.DOMCalVersion)

