#!/usr/bin/env python

import sys, os
from os.path import expandvars
from math import isnan

from I3Tray import *
from icecube import icetray, dataclasses, dataio, simclasses

from icecube.BadDomList import bad_dom_list_static
badOMs = bad_dom_list_static.IC86_static_bad_dom_list()

from optparse import OptionParser

parser = OptionParser()

parser.add_option("-i","--infile",
                  dest="INFILE", default=expandvars("$I3_PORTS/test-data/sim/GeoCalibDetectorStatus_IC86.55750_candidate.i3.gz"),
                  help="GCD file to correct.")

parser.add_option("-o","--outfile",
                  dest="OUTFILE", default=None,
                  help="Corrected GCD file.")

(options, args) = parser.parse_args()

if not options.OUTFILE :
	print "you must specify and output file."
	sys.exit()

infile = dataio.I3File(options.INFILE)

geo_frame = infile.pop_frame()
while not geo_frame.Has('I3Geometry'): geo_frame = infile.pop_frame()
geometry = geo_frame.Get('I3Geometry')

cal_frame = infile.pop_frame()
while not cal_frame.Has('I3Calibration'): cal_frame = infile.pop_frame()
calibration = cal_frame.Get('I3Calibration')

status_frame = infile.pop_frame()
while not status_frame.Has('I3DetectorStatus'): status_frame = infile.pop_frame()
status = status_frame.Get('I3DetectorStatus')

dom_geo = geometry.omgeo
dom_cal = calibration.dom_cal
dom_status = status.dom_status
	
c_and_d_strings_to_check = range(1,87)

low_noise_DOMs_l = [ icetray.OMKey(82,54),  icetray.OMKey(84,54),  icetray.OMKey(85,55)]

strings_IC86 = [ 1, 7, 14, 22, 31, 79, 80 ]

for e,p in dom_geo:

	if e not in badOMs and e in dom_cal and e in dom_status:				
		cal_this_om = dom_cal[e]
		status_this_om = dom_status[e]

		if float(cal_this_om.dom_cal_version[:3]) < 7.5 :
			print "Bad DOMCal"
			print '  %s  DOMCalVersion = %s' % (str(e), cal_this_om.dom_cal_version)
			calibration.dom_cal[e].dom_cal_version = '7.5.3'
			print '  correcting to ',calibration.dom_cal[e].dom_cal_version		
		
		threshold = dataclasses.spe_pmt_threshold(status_this_om,
							cal_this_om) / I3Units.mV

		if threshold < 0 :
			print 'Pathological PMT discriminator threshold'
			print '  %s  threshold = %2.2f mV' % (str(e), threshold)
			fit = dataclasses.LinearFit()
			fit.slope = NaN
			fit.intercept = NaN
			calibration.dom_cal[e].PMTDiscCalib = fit
			print '  correcting to %2.2f mV' % \
			      (dataclasses.spe_pmt_threshold(status_this_om,calibration.dom_cal[e])/I3Units.mV)

		# check the noise rates and relative DOM efficiencies in the new strings
		if e.string in strings_IC86 :
			if isnan(cal_this_om.dom_noise_rate) :
				if e.string in [79, 80] :
					calibration.dom_cal[e].dom_noise_rate = 750 * I3Units.hertz
					print "  correcting noise from 'nan' to %d Hz in %s" % (calibration.dom_cal[e].dom_noise_rate/I3Units.hertz,e)
				if e.string in [1, 7, 14, 22, 31] :
					calibration.dom_cal[e].dom_noise_rate = 500 * I3Units.hertz
					print "  correcting noise from 'nan' to %d Hz in %s" % (calibration.dom_cal[e].dom_noise_rate/I3Units.hertz,e)

 			if isnan(cal_this_om.relative_dom_eff) :
				if e.string in [79, 80] :
					calibration.dom_cal[e].relative_dom_eff = 1.35
					print "  correcting RDE from 'nan' to %.2f in %s" % (calibration.dom_cal[e].relative_dom_eff,e)
				if e.string in [1, 7, 14, 22, 31] :
					calibration.dom_cal[e].relative_dom_eff = 1.0
					print "  correcting RDE from 'nan' to %.2f in %s" % (calibration.dom_cal[e].relative_dom_eff,e)				

		# check for unusually low noise DOMs that were incorrectly translated into the DB
		if e in low_noise_DOMs_l :
			noiseRate = calibration.dom_cal[e].dom_noise_rate
			if noiseRate < 400 * I3Units.hertz :
				calibration.dom_cal[e].dom_noise_rate = noiseRate + 1*I3Units.kilohertz
				print "  correcting noise from %fHz to %fHz in %s" % (noiseRate/I3Units.hertz, calibration.dom_cal[e].dom_noise_rate/I3Units.hertz,e)
				
			
del geo_frame['I3Geometry']
geo_frame['I3Geometry'] = geometry

del cal_frame['I3Calibration']
cal_frame['I3Calibration'] = calibration

del status_frame['I3DetectorStatus']
status_frame['I3DetectorStatus'] = status


outfile = dataio.I3File(options.OUTFILE, dataio.I3File.Mode.Writing)
outfile.push(geo_frame)
outfile.push(cal_frame)
outfile.push(status_frame)
outfile.close()
