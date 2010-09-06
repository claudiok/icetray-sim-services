#!/usr/bin/env python

# This file can be manipulated to print the DOM ids with the nicknames and mainboard ids

import sys, os, math
from os.path import expandvars

from I3Tray import *
from icecube import icetray, dataclasses, dataio, simclasses

from icecube.BadDomList import bad_dom_list_static
badOMs = bad_dom_list_static.IC59_static_bad_dom_list()

from optparse import OptionParser

parser = OptionParser()

parser.add_option("-g","--gcdfile",
                  dest="GCDFILE", 
						default=expandvars("$I3_PORTS/test-data/sim/GeoCalibDetectorStatus_IC59.55040.i3.gz"),
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

#geo_strings_to_check = range(1,87)
#for s in geo_strings_to_check:
#	found = False
#	for e,p in dom_geo:
#		if e.GetString() == s :
#			found = True
#	if not found:
#		print "string %d does not exist in the geometry" % s

import scipy
from scipy.optimize import leastsq
import numpy
#import matplotlib.pyplot as plt
#import matplotlib.nxutils as nx
#import matplotlib.cm as cm


ic79_strs = [ 
	41, 32, 23, 15, 8, 2,
	51, 42, 33, 24, 16, 9, 3,
	60, 52, 43, 34, 25, 17, 10, 4,
	68, 61, 53, 44, 35, 26, 18, 11, 5,
	75, 69, 62, 54, 45, 36, 27, 19, 12, 6,
	76, 70, 63, 55, 46, 37, 28, 20, 13,
	77, 71, 64, 56, 47, 38, 29, 21,
	78, 72, 65, 57, 48, 39, 30,
	73, 66, 58, 49, 40,
	74, 67, 59, 50,
	81, 82, 83, 84, 85, 86
]

ic59_strs = [
	75, 76, 77, 78, 68, 69, 70, 71, 72, 73,
	74, 60, 61, 62, 63, 64, 65, 66, 67, 52,
	53, 54, 55, 56, 57, 58, 59, 44, 45, 46,
	47, 48, 49, 50, 36, 83, 37, 38, 39, 40,
	26, 27, 28, 29, 30, 17, 18, 19, 20, 21,
	10, 11, 12, 13,  2,  3,  4,  5,  6
]

ic40_strs = [
	60, 68, 75, 52, 61, 69, 76, 53, 62, 70,
	77, 44, 54, 63, 71, 78, 45, 55, 64, 72,
	46, 56, 65, 73, 47, 57, 66, 74, 38, 48,
	58, 67, 29, 39, 49, 59, 21, 30, 40, 50
]

bad_oms_run116120 = [
	icetray.OMKey(29,59),
	icetray.OMKey(29,60),
	icetray.OMKey(30,23),
	icetray.OMKey(30,60),
	icetray.OMKey(39,22),
	icetray.OMKey(50,36),
	icetray.OMKey(54,47),
	icetray.OMKey(69,44),
	icetray.OMKey(69,47),
	icetray.OMKey(69,48),
	icetray.OMKey(71,39),
	icetray.OMKey(74,9),
	icetray.OMKey(18,46),
	icetray.OMKey(26,46),
	icetray.OMKey(8,59),
	icetray.OMKey(8,60),
	icetray.OMKey(9,16),
	icetray.OMKey(24,56),
	icetray.OMKey(32,57),
	icetray.OMKey(32,58),
	icetray.OMKey(34,11),
	icetray.OMKey(34,12),
	icetray.OMKey(34,13),
	icetray.OMKey(34,14),
	icetray.OMKey(34,15),
	icetray.OMKey(34,17),
	icetray.OMKey(34,22),
	icetray.OMKey(42,47),
	icetray.OMKey(42,48),
	icetray.OMKey(86,27),
	icetray.OMKey(86,28),
	icetray.OMKey(5,5),
	icetray.OMKey(5,6),
	icetray.OMKey(11,1),
	icetray.OMKey(11,2),
	icetray.OMKey(27,7),
	icetray.OMKey(27,8),
	icetray.OMKey(68,42),
	icetray.OMKey(18,45),
	icetray.OMKey(19,59),
	icetray.OMKey(19,60),
	icetray.OMKey(28,57),
	icetray.OMKey(28,58),
	icetray.OMKey(59,45),
	icetray.OMKey(59,46),
	icetray.OMKey(60,55),
	icetray.OMKey(60,56),
	icetray.OMKey(62,51),
	icetray.OMKey(62,52),
	icetray.OMKey(54,51),
	icetray.OMKey(54,52),
	icetray.OMKey(6,11),
	icetray.OMKey(38,59),
	icetray.OMKey(39,61),
	icetray.OMKey(40,51),
	icetray.OMKey(40,52),
	icetray.OMKey(44,25),
	icetray.OMKey(44,26),
	icetray.OMKey(44,47),
	icetray.OMKey(44,48),
	icetray.OMKey(47,55),
	icetray.OMKey(47,56),
	icetray.OMKey(53,17),
	icetray.OMKey(53,18),
	icetray.OMKey(66,33),
	icetray.OMKey(66,34),
	icetray.OMKey(66,45),
	icetray.OMKey(66,46),
	icetray.OMKey(69,23),
	icetray.OMKey(69,24),
	icetray.OMKey(33,5),
	icetray.OMKey(33,6),
	icetray.OMKey(9,15),
	icetray.OMKey(45,19),
	icetray.OMKey(45,20),
	icetray.OMKey(51,27),
	icetray.OMKey(51,28),
	icetray.OMKey(49,55),
	icetray.OMKey(49,56),
	icetray.OMKey(84,40),
	icetray.OMKey(81,44),
	icetray.OMKey(74,10),
	icetray.OMKey(38,49)
]

#for s in ic79_strs:
#	found_cal = False
#	found_stat = False
#	for omkey in [icetray.OMKey(s,om) for om in range(61)] :
#		if omkey in dom_cal :
#			found_cal = True
#		if omkey in dom_status :
#			found_stat = True
#		if found_cal and found_stat : continue
#	if not found_cal : print 'string %s is missing from the calibration' % s
#	if not found_stat : print 'string %s is missing from the detector status' % s

high_QE = [icetray.OMKey(36,d) for d in range(44,60) if (d != 45 and d!= 47) ] 
for s in range(81,87):
	high_QE += [icetray.OMKey(s,d) for d in range(81)] 

table_dir = os.path.expandvars("$I3_SRC/sim-services/resources/tables") 

for s in ic79_strs:
	for d in xrange(1, 61):
		omkey = OMKey(s, d)
#		if ((omkey in dom_geo) and (not omkey in dom_cal)):
#		if ((omkey in dom_geo) and (not omkey in dom_status)):
#		if (omkey in bad_oms_run116120) and (omkey in dom_geo) and (omkey in dom_cal) and (omkey in dom_status):
		if ((omkey in dom_geo) and (omkey in dom_cal) and (omkey in dom_status)):
			cal_this_om = dom_cal[omkey]
			status_this_om = dom_status[omkey]
#			value = dataclasses.SPEDiscriminatorThreshold(status_this_om, cal_this_om) / I3Units.mV 
			value = dataclasses.SPEPMTThreshold(status_this_om, cal_this_om) / I3Units.mV 
#			value = dataclasses.FADCBaseline(status_this_om, cal_this_om)
			value = round(value, 2)
			if omkey == OMKey(66,39) or omkey == OMKey(68,40) or omkey == OMKey(68,43):
				omid_str = ''
				if s < 10 and d < 10:
					omid_str = '0' + str(s) + '-' + '0' + str(d)
				elif s >= 10 and d < 10:
					omid_str = str(s) + '-' + '0' + str(d)
				elif s < 10 and d >= 10:
					omid_str = '0' + str(s) + '-' + str(d)
				elif s >= 10 and d >= 10:
					omid_str = str(s) + '-' + str(d)

				f = open(table_dir + os.sep + 'nicknames.txt', 'r')
				for line in f:
					splitLine = line.split()
					if omid_str == splitLine[3]:
						print '{0}    {1}    {2}    {3}'.format(splitLine[3], splitLine[2], splitLine[0], value)
						break
				f.close()
#`				print omid_str

#for omkey in bad_oms_run116120 :
#	if (omkey in dom_cal and omkey in dom_status) and (not omkey in badOMs):
#		print str(omkey)


#for s in ic79_strs:
#	for d in xrange(1, 61):
#		omkey = OMKey(s, d)
#		if ((not omkey in badOMs) and (omkey in dom_status) and (omkey in dom_cal)):
#			cal_this_om = dom_cal[omkey]
#			status_this_om = dom_status[omkey]

#			value = status_this_om.pmtHV/I3Units.V
#			value = cal_this_om.DomNoiseRate*I3Units.second
#			value = dataclasses.PMTGain(status_this_om, cal_this_om) / 1.e7
#			value = cal_this_om.FrontEndImpedance / I3Units.ohm
#			value = dataclasses.TransitTime(status_this_om, cal_this_om);
#			value = dataclasses.SPEPMTThreshold(status_this_om, cal_this_om) / I3Units.mV
#			value = dataclasses.ATWDSamplingRate(1,status_this_om,cal_this_om)
#			value = status_this_om.lcMode

#			detector = 79
#			if s in ic59_strs: detector = 59
#			if s in ic40_strs: detector = 40
#			print s, d, value, detector
