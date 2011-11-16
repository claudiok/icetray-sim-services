#!/usr/bin/env python

import sys, os, math
from os.path import expandvars

from I3Tray import *
from icecube import icetray, dataclasses, dataio, simclasses

from icecube.BadDomList import bad_dom_list_static
#badOMs = bad_dom_list_static.IC59_static_bad_dom_list()
badOMs = bad_dom_list_static.IC86_static_bad_dom_list()

from optparse import OptionParser

parser = OptionParser()

parser.add_option("-g","--gcdfile",
                  dest="GCDFILE", default=expandvars("$I3_BUILD/GeoCalibDetectorStatus_IC86.55697_corrected.i3.gz"),
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
dom_cal = calibration.dom_cal
dom_status = status.dom_status
#for IceTop
vem_cal = calibration.vem_cal
sta_geo = geometry.stationgeo

geo_strings_to_check = range(1,87)
for s in geo_strings_to_check:
	found = False
	for e,p in dom_geo:
		if e.string == s :
			found = True
	if not found:
		print "string %d does not exist in the geometry" % s
	
c_and_d_strings_to_check = range(1,87)

for s in c_and_d_strings_to_check :
	found_cal = False
	found_stat = False
	found_vemcal = False
	for omkey in [icetray.OMKey(s,om) for om in range(65)] :
		if omkey in dom_cal :
			found_cal = True
		if omkey in dom_status :
			found_stat = True
		if omkey.om > 60 and s < 82:
			if omkey in vem_cal:
				found_vemcal = True
			else :
				print '%s is missing from the VEMCal' % omkey
		if found_cal and found_stat : continue
	if not found_cal : print 'string %s is missing from the calibration' % s
	if not found_stat : print 'string %s is missing from the detector status' % s

high_QE = [icetray.OMKey(36,d) for d in range(44,60) if (d != 45 and d!= 47) ] 
for s in range(79,87):
	high_QE += [icetray.OMKey(s,d) for d in range(81)] 

for e,p in dom_geo:
	if e in badOMs and e in dom_cal and e in dom_status:
		print "DOM %s is in the bad DOM list but contains status and calibration records" % str(e)
	
	if e not in badOMs and e in dom_cal and e in dom_status:				
		cal_this_om = dom_cal[e]
		status_this_om = dom_status[e]

		# checks for hit-maker
		if ( status_this_om.pmt_hv < 100*I3Units.V or \
		     status_this_om.pmt_hv > 2000*I3Units.V ) :
			print '  %s  pmt_hv = %s V !!' % (str(e), status_this_om.pmt_hv/I3Units.V)
		
		if(math.isnan(status_this_om.pmt_hv*0)):
			print '  %s  pmt_hv = %s V !!' % (str(e), status_this_om.pmt_hv/I3Units.V)

		if e.om < 61 :
			if ( ( cal_this_om.relative_dom_eff != 1.0 and high_QE.count(e) == 0 ) or \
			     ( ( cal_this_om.relative_dom_eff < 1.34 or cal_this_om.relative_dom_eff > 1.36 ) and \
			       high_QE.count(e) == 1 ) ):
				print '  %s  relative_dom_eff = %s !!' % (str(e), cal_this_om.relative_dom_eff)
	        if(math.isnan(cal_this_om.relative_dom_eff)):
			print '  %s  relative_dom_eff = %s !!' % (str(e), cal_this_om.relative_dom_eff)
		
	        
		# checks for noise-generator
		noiseRate = cal_this_om.dom_noise_rate * I3Units.second
		if (( ( high_QE.count(e) == 0 and ( noiseRate < 300 or noiseRate > 850) ) or \
		      ( high_QE.count(e) == 1 and ( noiseRate < 400 or noiseRate > 1100) )) and \
		    (e.om >= 1 and e.om <= 60)):
			print '  %s  noise rate = %s !!' % (str(e), noiseRate)
		
		if(math.isnan(noiseRate)):
			print '  %s  noise rate = %s !!' % (str(e), noiseRate)
			
		############
		# checks for pmt-simulator
		pmtGain = dataclasses.pmt_gain(status_this_om, cal_this_om) / 1.e7
		if ((not (pmtGain > 0.5 and pmtGain < 3.0)) and (e.om >= 1 and e.om <= 60)):
			print '  %s  pmt_gain = %s !!' % (str(e), pmtGain)
		if(math.isnan(pmtGain*0)):
			print '  %s  pmt_gain = %s !!' % (str(e), pmtGain)
		# For IceTop 
		pmtHGain = dataclasses.pmt_gain(status_this_om, cal_this_om) / 5.e6
		pmtLGain = dataclasses.pmt_gain(status_this_om, cal_this_om) / 1.e5
		if (e.om > 60) :
			if(status_this_om.dom_gain_type == dataclasses.I3DOMStatus.DOMGain.High):
				if (not (pmtHGain > 0.5 and pmtHGain < 3.0)):
					print '  %s  pmtGain = %s !!' % (str(e), pmtHGain)
				elif e.string in [39,67,26]:
					print '  Checking switched gain DOMs %s  pmtGain = %s !!' % (str(e), pmtHGain)
			elif(status_this_om.dom_gain_type == dataclasses.I3DOMStatus.DOMGain.Low):
				if (not (pmtLGain > 0.5 and pmtLGain < 3.0)):
					print '  %s  pmtGain = %s !!' % (str(e), pmtLGain)
				elif e.string in [39,67,26]:
					print '  Checking switched gain DOMs %s  pmtGain = %s !!' % (str(e), pmtLGain)
			else:
				print 'Unknown Gain for  %s  Gain %s pmtGain = %s !!!!!' % (str(e), status_this_om.dom_gain_type,dataclasses.pmt_gain(status_this_om, cal_this_om))
			
		impedence = cal_this_om.front_end_impedance / I3Units.ohm
		if ((not (impedence == 43 or impedence == 50)) and (e.om >= 1 and e.om <= 64)):    # IT DOMs have same impedance
			print '  %s  impedence = %s !!' % (str(e), impedence)


		transitTime = dataclasses.transit_time(status_this_om, cal_this_om);
		if ((not (transitTime > 130 and transitTime < 151)) and (e.om >= 1 and e.om <= 60)):
			print '%s  transitTime = %s !!' % (str(e), transitTime)
		## For IceTop transit times are different (because of different gains, see Chris Weaver's presentation in IceCal Call 06-10-2011)
		elif ((not (transitTime > 130 and transitTime < 165)) and (e.om >= 61 and e.om <= 64)) or (e.om == 61 and e.string == 26): # (26,61) is known to be a bit different sometimes
			print 'IceTop : %s  transitTime = %s  voltage = %s (behaviour should be ~2000/sqrt(voltage)+87.21) !!' % (str(e), transitTime,status_this_om.pmt_hv/icetray.I3Units.volt)
		 ###########
	        if(math.isnan(transitTime*0)):
			print '%s  transitTime = %s !!' % (str(e), transitTime)
		# checks for DOMsimulator
		threshold = dataclasses.spe_pmt_threshold(status_this_om,
							cal_this_om) / I3Units.mV
		if threshold < 0 :
			print '  %s  %f' % (str(e), threshold)

		atwdaSamplingRate = dataclasses.atwd_sampling_rate(0,status_this_om,cal_this_om)
		if atwdaSamplingRate < 295*I3Units.megahertz or atwdaSamplingRate > 310*I3Units.megahertz :
			print '  %s  ATWDaSamplingRate = %s MHz!!' % (str(e), atwdaSamplingRate/I3Units.megahertz)
			
		atwdbSamplingRate = dataclasses.atwd_sampling_rate(1,status_this_om,cal_this_om)
		if atwdbSamplingRate < 295*I3Units.megahertz or atwdbSamplingRate > 310*I3Units.megahertz :
			print '  %s  ATWDbSamplingRate = %s MHz!!' % (str(e), atwdbSamplingRate/I3Units.megahertz)
					
		# checks for DOMcalibrator
		if cal_this_om.dom_cal_version[:3] != "7.5" :
			print '  %s  dom_cal_version = %s !!' % (str(e), cal_this_om.dom_cal_version)
		
		# checks for topsimulator : snowheight (part of TankGeo), vemcal, MET (later)
		if e.om> 60:
			vemcal_this_om = vem_cal[e]
			if ( (vemcal_this_om.pe_per_vem <= 0) or (vemcal_this_om.pe_per_vem > 220)):
				print 'IceTop %s pe_per_vem is %f' % (e,vemcal_this_om.pe_per_vem)
			if( (vemcal_this_om.corr_factor <= 0) or (vemcal_this_om.corr_factor > 1.1)):
				print 'IceTop %s corr_factor is %f' % (e,vemcal_this_om.corr_factor)
			if( (status_this_om.dom_gain_type==dataclasses.I3DOMStatus.DOMGain.High) and (vemcal_this_om.hglg_cross_over <= 1500) or (vemcal_this_om.hglg_cross_over > 4000)): # weird pybinding name corrected in the dataclasses trunk
				print 'IceTop %s hglg_cross_over is %f' % (e,vemcal_this_om.hglg_cross_over)


for e,st in sta_geo:
	for t in st:
		snowh = t.snowheight
		omkeyInTank = [omk for omk in t.omkey_list]
		if ((snowh < 0.01*I3Units.m) or (snowh > 1.8*I3Units.m)):
			print 'IceTop station %s with DOMs(%s) snowheight is %f' % (e,omkeyInTank,snowh)
