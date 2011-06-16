#!/usr/bin/env python

import sys, os
from os.path import expandvars

from I3Tray import *
from icecube import icetray, dataclasses, dataio, simclasses

from icecube.BadDomList import bad_dom_list_static
#badOMs = bad_dom_list_static.IC59_static_bad_dom_list()
badOMs = bad_dom_list_static.IC79_static_bad_dom_list()

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
#for IceTop
vem_cal = calibration.vemCal
sta_geo = geometry.stationgeo

geo_strings_to_check = range(1,87)
for s in geo_strings_to_check:
	found = False
	for e,p in dom_geo:
		if e.GetString() == s :
			found = True
	if not found:
		print "string %d does not exist in the geometry" % s
	
#c_and_d_strings_to_check = [ 
#	75, 76, 77, 78, 68, 69, 70, 71, 72, 73,
#	74, 60, 61, 62, 63, 64, 65, 66, 67, 52,
#	53, 54, 55, 56, 57, 58, 59, 44, 45, 46,
#	47, 48, 49, 50, 36, 83, 37, 38, 39, 40,
#	26, 27, 28, 29, 30, 17, 18, 19, 20, 21,
#	10, 11, 12, 13,  2,  3,  4,  5,  6
#	]
c_and_d_strings_to_check = [ 
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

for s in c_and_d_strings_to_check :
	found_cal = False
	found_stat = False
	found_vemcal = False
#	for omkey in [icetray.OMKey(s,om) for om in range(61)] :
	for omkey in [icetray.OMKey(s,om) for om in range(65)] :
		if omkey in dom_cal :
			found_cal = True
		if omkey in dom_status :
			found_stat = True
		if omkey.GetOM() > 60 and s < 82:
			if omkey in vem_cal:
				found_vemcal = True
			else :
				print '%s is missing from the VEMCal' % omkey
		if found_cal and found_stat : continue
	if not found_cal : print 'string %s is missing from the calibration' % s
	if not found_stat : print 'string %s is missing from the detector status' % s

high_QE = [icetray.OMKey(36,d) for d in range(44,60) if (d != 45 and d!= 47) ] 
for s in range(81,87):
	high_QE += [icetray.OMKey(s,d) for d in range(81)] 

for e,p in dom_geo:
	if e in badOMs and e in dom_cal and e in dom_status:
		print "DOM %s is in the bad DOM list but contains status and calibration records" % str(e)
	
	if e not in badOMs and e in dom_cal and e in dom_status:				
		cal_this_om = dom_cal[e]
		status_this_om = dom_status[e]

		# checks for hit-maker
		if ( status_this_om.pmtHV < 100*I3Units.V or \
		     status_this_om.pmtHV > 2000*I3Units.V ) :
			print '  %s  pmtHV = %s V !!' % (str(e), status_this_om.pmtHV/I3Units.V)

		if e.GetOM() < 61 :
			if ( ( cal_this_om.RelativeDomEff != 1.0 and high_QE.count(e) == 0 ) or \
			     ( ( cal_this_om.RelativeDomEff < 1.34 or cal_this_om.RelativeDomEff > 1.36 ) and \
			       high_QE.count(e) == 1 ) ):
				print '  %s  RelativeDomEff = %s !!' % (str(e), cal_this_om.RelativeDomEff)
	
		# checks for noise-generator
		noiseRate = cal_this_om.DomNoiseRate * I3Units.second
		if (( ( high_QE.count(e) == 0 and ( noiseRate < 400 or noiseRate > 700) ) or \
		      ( high_QE.count(e) == 1 and ( noiseRate < 400 or noiseRate > 1100) )) and \
		    (e.GetOM() >= 1 and e.GetOM() <= 60)):
			print '  %s  noise rate = %s !!' % (str(e), noiseRate)



		############
		# checks for pmt-simulator
		pmtGain = dataclasses.PMTGain(status_this_om, cal_this_om) / 1.e7
		if ((not (pmtGain > 0.5 and pmtGain < 3.0)) and (e.GetOM() >= 1 and e.GetOM() <= 60)):
			print '  %s  pmtGain = %s !!' % (str(e), pmtGain)
			
		# For IceTop 
		pmtHGain = dataclasses.PMTGain(status_this_om, cal_this_om) / 5.e6
		pmtLGain = dataclasses.PMTGain(status_this_om, cal_this_om) / 1.e5
		if (e.GetOM() > 60) :
			if(status_this_om.domGainType == dataclasses.I3DOMStatus.DOMGain.High):
				if (not (pmtHGain > 0.5 and pmtHGain < 3.0)):
					print '  %s  pmtGain = %s !!' % (str(e), pmtHGain)
				elif e.GetString() in [39,67,26]:
					print '  Checking switched gain DOMs %s  pmtGain = %s !!' % (str(e), pmtHGain)
			elif(status_this_om.domGainType == dataclasses.I3DOMStatus.DOMGain.Low):
				if (not (pmtLGain > 0.5 and pmtLGain < 3.0)):
					print '  %s  pmtGain = %s !!' % (str(e), pmtLGain)
				elif e.GetString() in [39,67,26]:
					print '  Checking switched gain DOMs %s  pmtGain = %s !!' % (str(e), pmtLGain)
			else:
				print 'Unknown Gain for  %s  Gain %s pmtGain = %s !!!!!' % (str(e), status_this_om.domGainType,dataclasses.PMTGain(status_this_om, cal_this_om))
			
		impedence = cal_this_om.FrontEndImpedance / I3Units.ohm
		if ((not (impedence == 43 or impedence == 50)) and (e.GetOM() >= 1 and e.GetOM() <= 64)):    # IT DOMs have same impedance
			print '  %s  impedence = %s !!' % (str(e), impedence)


		transitTime = dataclasses.TransitTime(status_this_om, cal_this_om);
		if ((not (transitTime > 130 and transitTime < 151)) and (e.GetOM() >= 1 and e.GetOM() <= 60)):
			print '%s  transitTime = %s !!' % (str(e), transitTime)
		## For IceTop transit times are different (because of different gains, see Chris Weaver's presentation in IceCal Call 06-10-2011)
		elif ((not (transitTime > 130 and transitTime < 165)) and (e.GetOM() >= 61 and e.GetOM() <= 64)) or (e.GetOM() == 61 and e.GetString() == 26): # (26,61) is known to be a bit different sometimes
			print 'IceTop : %s  transitTime = %s  voltage = %s (behaviour should be ~2000/sqrt(voltage)+87.21) !!' % (str(e), transitTime,status_this_om.pmtHV/icetray.I3Units.volt)
                ###########

			
		# checks for DOMsimulator
		threshold = dataclasses.SPEPMTThreshold(status_this_om,
							cal_this_om) / I3Units.mV
		if threshold < 0 :
			print '  %s  %f' % (str(e), threshold)

		atwdaSamplingRate = dataclasses.ATWDSamplingRate(0,status_this_om,cal_this_om)
		if atwdaSamplingRate < 295*I3Units.megahertz or atwdaSamplingRate > 310*I3Units.megahertz :
			print '  %s  ATWDaSamplingRate = %s MHz!!' % (str(e), atwdaSamplingRate/I3Units.megahertz)
			
		atwdbSamplingRate = dataclasses.ATWDSamplingRate(1,status_this_om,cal_this_om)
		if atwdbSamplingRate < 295*I3Units.megahertz or atwdbSamplingRate > 310*I3Units.megahertz :
			print '  %s  ATWDbSamplingRate = %s MHz!!' % (str(e), atwdbSamplingRate/I3Units.megahertz)

		# ATWD gain
		#for channel in [0,1,2]:
		#	g = cal_this_om.ATWDGain(channel)

		# GetATWDBinCount
		#for chip in [0,1]:
		#	for channel in [0,1,2]:
		#		for bin in range(128): 
		#			fit = cal_this_om.GetATWDBinCalibFit(chip,channel,bin)
					
		# checks for DOMcalibrator
		if cal_this_om.DOMCalVersion != "7.5.2" :
			print '  %s  DOMCalVersion = %s !!' % (str(e), cal_this_om.DOMCalVersion)

		# checks for topsimulator : snowheight (part of TankGeo), vemcal, MET (later)
		if e.GetOM()> 60:
			vemcal_this_om = vem_cal[e]
			if ( (vemcal_this_om.pePerVEM <= 0) or (vemcal_this_om.pePerVEM > 220)):
				print 'IceTop %s pePerVEM is %f' % (e,vemcal_this_om.pePerVEM)
			if( (vemcal_this_om.corrFactor <= 0) or (vemcal_this_om.corrFactor > 1.1)):
				print 'IceTop %s corrFactor is %f' % (e,vemcal_this_om.corrFactor)
			if( (status_this_om.domGainType==dataclasses.I3DOMStatus.DOMGain.High) and (vemcal_this_om.hglgCroddOver <= 1500) or (vemcal_this_om.hglgCroddOver > 4000)): # weird pybinding name corrected in the dataclasses trunk
				print 'IceTop %s hglgCrossOver is %f' % (e,vemcal_this_om.hglgCroddOver)


for e,st in sta_geo:
	for t in st:
		snowh = t.snowheight
		omkeyInTank = [omk for omk in t.omKeyList]
		if ((snowh < 0.01*I3Units.m) or (snowh > 1.8*I3Units.m)):
			print 'IceTop station %s with DOMs(%s) snowheight is %f' % (e,omkeyInTank,snowh)
