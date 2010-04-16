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

new_gcdfile = dataio.I3File('new_gcd.i3.gz', dataio.I3File.Mode.Writing)

geo_frame = gcdfile.pop_frame()
while not geo_frame.Has('I3Geometry'): geo_frame = gcdfile.pop_frame()
geometry = geo_frame.Get('I3Geometry')

cal_frame = gcdfile.pop_frame()
while not cal_frame.Has('I3Calibration'): cal_frame = gcdfile.pop_frame()
calibration = cal_frame.Get('I3Calibration')

status_frame = gcdfile.pop_frame()
while not status_frame.Has('I3DetectorStatus'): status_frame = gcdfile.pop_frame()
status = status_frame.Get('I3DetectorStatus')

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

		if e.GetOM() == 39 and e.GetString() == 72 :
			print "%s LCMode is %s" % ( str(e), int(dom_status[e].lcMode) )
			if status_this_om.lcMode != dataclasses.I3DOMStatus.LCMode.UpOrDown :
				status.domStatus[e].lcMode = dataclasses.I3DOMStatus.LCMode.UpOrDown
				print " %s correcting LCMode to %d" % ( str(e), int(dom_status[e].lcMode) )
				
		if cal_this_om.DOMCalVersion != "7.4.0" :
			print '  %s  %s' % (str(e), cal_this_om.DOMCalVersion)

			calibration.domCal[e].DOMCalVersion = '7.5.0'
			print calibration.domCal[e].DOMCalVersion
		
		threshold = dataclasses.SPEPMTThreshold(status_this_om,
							cal_this_om) / I3Units.mV

		if threshold < 0 :
			print '  %s  %f' % (str(e), threshold)
			print 'changing the calib'
			fit = dataclasses.LinearFit()
			fit.slope = NaN
			fit.intercept = NaN
			calibration.domCal[e].PMTDiscCalib = fit
			print calibration.domCal[e].PMTDiscCalib.slope
			print calibration.domCal[e].PMTDiscCalib.intercept

del cal_frame['I3Calibration']
cal_frame['I3Calibration'] = calibration
new_gcdfile.push(geo_frame)
new_gcdfile.push(cal_frame)
new_gcdfile.push(status_frame)
new_gcdfile.close()
