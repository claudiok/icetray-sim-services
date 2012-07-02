#!/usr/bin/env python

import sys, os, math
from os.path import expandvars

from I3Tray import *
import icecube
from icecube import icetray, dataclasses, dataio, simclasses

from icecube.sim_services import bad_dom_list_static

# this should be organized more like tests
# make a bad GCD file that can be used for
# testing

##########
#
# Options
#
##########
GCD_FN_DEF = expandvars("$I3_BUILD/GeoCalibDetectorStatus_IC86.55697_corrected.i3.gz")
from optparse import OptionParser

parser = OptionParser()

parser.add_option("-g","--gcdfile",
                  dest="GCDFILE", default = GCD_FN_DEF,
                  help="GCD file to test.")

(options, args) = parser.parse_args()

##########
#
# Get the GCD file and pull the frames out of it
#
##########

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
vem_cal = calibration.vem_cal #for IceTop
dom_status = status.dom_status
sta_geo = geometry.stationgeo

##########
#
# First simple checks on the geometry.
# Make sure there are no missing strings.
#
##########
geo_strings_to_check = range(1,87)
for s in geo_strings_to_check:
	found = False
	for e,p in dom_geo:
		if e.string == s :
			found = True
	if not found:
		print "string %d does not exist in the geometry" % s
	
##########
#
# First simple checks on the calibration
# and detector status.  Make sure there
# are no missing entries
#
##########
c_and_d_strings_to_check = range(1,87)
icecube_omkeys = [icetray.OMKey(s,om) for s in range(1,87) for om in range(1,61)]
icetop_omkeys = [icetray.OMKey(s,om) for s in range(1,80) for om in range(61,65)]
for s in c_and_d_strings_to_check :
	found_cal = False
	found_stat = False
	for omkey in icecube_omkeys :
		if omkey in dom_cal :
			found_cal = True
		if omkey in dom_status :
			found_stat = True
		if omkey not in dom_geo :
			print '%s is missing from the IceCube geometry' % omkey
		if found_cal and found_stat : continue

	for station in range(1,81) :
		if station not in sta_geo :
			print '%s is missing from the IceTop geometry' % omkey

	for omkey in icetop_omkeys :
		if omkey in dom_cal :
			found_cal = True
		if omkey in dom_status :
			found_stat = True
		if omkey not in vem_cal :
			print '%s is missing from the VEMCal' % omkey
			
	if not found_cal : print 'string %s is missing from the calibration' % s
	if not found_stat : print 'string %s is missing from the detector status' % s

##########
#
# Make a list of the high QE DOMs.
#
##########
high_QE = [icetray.OMKey(36,d) for d in range(44,60) if (d != 45 and d!= 47) ] 
high_QE += [ icetray.OMKey( 79, i ) for i in range(30, 45) if i not in [ 32, 41, 43 ] ]
high_QE += ( [ icetray.OMKey( 80, i ) for i in range(30, 44) ] )
for s in range(81,87):
	high_QE += [icetray.OMKey(s,d) for d in range(1,61)]
high_QE += [icetray.OMKey(43,55)] #Mark Krasberg (post deployment after surface testing)

##########
#
# Make a list of the bad DOMs, including SLC only.
#
##########
badOMs = bad_dom_list_static.IC86_static_bad_dom_list()
slc_only_doms = bad_dom_list_static.IC86_static_bad_dom_list_HLC()

for omkey,omgeo in dom_geo:
	if omkey in badOMs \
	       and omkey in dom_cal \
	       and omkey in dom_status:
		print "DOM %s is in the bad DOM list but contains"\
		      " status and calibration records" % str(omkey)
	
	if omkey not in badOMs \
	       and omkey in dom_cal \
	       and omkey in dom_status:		
		this_domcal = dom_cal[omkey]
		this_domstatus = dom_status[omkey]

		this_dom_type = dom_geo[ omkey ].omtype 

		# break each of these out to separate modules

	        ##########
		# checks for hit-maker
	        ##########
		if ( this_domstatus.pmt_hv < 100*I3Units.V or \
		     this_domstatus.pmt_hv > 2000*I3Units.V ) :
			print '  %s  pmt_hv = %s V !!' % \
			      (str(omkey), this_domstatus.pmt_hv/I3Units.V)
		
		if(math.isnan(this_domstatus.pmt_hv)):
			print '  %s  pmt_hv = %s V !!' % \
			      (str(omkey), this_domstatus.pmt_hv/I3Units.V)

		if this_dom_type == dataclasses.I3OMGeo.IceCube :
			if ( ( this_domcal.relative_dom_eff != 1.0 \
			       and high_QE.count(e) == 0 ) \
			     or ( ( this_domcal.relative_dom_eff < 1.34 \
				    or this_domcal.relative_dom_eff > 1.36 ) \
				  and high_QE.count(e) == 1 ) ):
				 print '  %s  relative_dom_eff = %s !!' % \
				       (str(e), this_domcal.relative_dom_eff)

	        if(math.isnan(this_domcal.relative_dom_eff)):
			print '  %s  relative_dom_eff = %s !!' % \
			      (str(e), this_domcal.relative_dom_eff)
		
	        
	        ##########
		# checks for noise-generator
	        ##########
		noiseRate = this_domcal.dom_noise_rate * I3Units.second
		if ((( high_QE.count(e) == 0 and \
			( noiseRate < 300 or noiseRate > 850) ) \
		      or ( high_QE.count(e) == 1 and \
			   ( noiseRate < 400 or noiseRate > 1100)))) \
		    and	this_dom_type == dataclasses.I3OMGeo.IceCube :
			print '  %s  noise rate = %s !!' % (str(e), noiseRate)
		
		if(math.isnan(noiseRate)):
			print '  %s  noise rate = %s !!' % (str(e), noiseRate)
			
	        ##########
		# checks for pmt-simulator
	        ##########
		pmtGain = dataclasses.pmt_gain(this_domstatus, this_domcal) / 1.e7
		if ((not (pmtGain > 0.5 and pmtGain < 3.0)) and (e.om >= 1 and e.om <= 60)):
			print '  %s  pmt_gain = %s !!' % (str(e), pmtGain)
		if(math.isnan(pmtGain*0)):
			print '  %s  pmt_gain = %s !!' % (str(e), pmtGain)

	        ##########
		# For IceTop 
	        ##########
		pmtHGain = dataclasses.pmt_gain(this_domstatus, this_domcal) / 5.e6
		pmtLGain = dataclasses.pmt_gain(this_domstatus, this_domcal) / 1.e5
		if this_dom_type == dataclasses.I3OMGeo.IceTop :
			if(this_domstatus.dom_gain_type == dataclasses.I3DOMStatus.DOMGain.High):
				if (not (pmtHGain > 0.5 and pmtHGain < 3.0)):
					print '  %s  pmtGain = %s !!' % (str(e), pmtHGain)
				elif e.string in [39,67,26]:
					print '  Checking switched gain DOMs %s  pmtGain = %s !!' % \
					      (str(e), pmtHGain)
			elif(this_domstatus.dom_gain_type == dataclasses.I3DOMStatus.DOMGain.Low):
				if (not (pmtLGain > 0.5 and pmtLGain < 3.0)):
					print '  %s  pmtGain = %s !!' % (str(e), pmtLGain)
				elif e.string in [39,67,26]:
					print '  Checking switched gain DOMs %s  pmtGain = %s !!' % \
					      (str(e), pmtLGain)
			else:
				print 'Unknown Gain for  %s  Gain %s pmtGain = %s !!!!!' % \
				      (str(e), this_domstatus.dom_gain_type,dataclasses.pmt_gain(this_domstatus, this_domcal))
			
		# IT DOMs have same impedance
		impedence = this_domcal.front_end_impedance / I3Units.ohm
		if ((not (impedence == 43 or impedence == 50)) and (e.om >= 1 and e.om <= 64)):    
			print '  %s  impedence = %s !!' % (str(e), impedence)


		transitTime = dataclasses.transit_time(this_domstatus, this_domcal);
		if ((not (transitTime > 130 and transitTime < 151)) and (e.om >= 1 and e.om <= 60)):
			print '%s  transitTime = %s !!' % (str(e), transitTime)
		# For IceTop transit times are different (because of different gains,
		# see Chris Weaver's presentation in IceCal Call 06-10-2011)
		elif ((not (transitTime > 130 and transitTime < 165)) \
		      and (e.om >= 61 and e.om <= 64))\
		      or (e.om == 61 and e.string == 26) : # (26,61) is known to be a bit different sometimes
			print 'IceTop : %s  transitTime = %s  voltage = %s (behaviour should be ~2000/sqrt(voltage)+87.21) !!' \
			      % (str(e), transitTime,this_domstatus.pmt_hv/icetray.I3Units.volt)

	        if( math.isnan(transitTime) ):
			print '%s  transitTime = %s !!' % (str(e), transitTime)

	        ##########
		# checks for DOMsimulator
	        ##########

		if e in slc_only_doms :
			if this_domstatus.lc_mode != dataclasses.I3DOMStatus.SoftLC :
				print "  DOM %s : LCMode is not SoftLC ( LCMode = %s )" %\
				      (str(e), this_domstatus.lc_mode)
			else:
				print "  DOM %s : LCMode is set correctly to %s " %\
				      (str(e), this_domstatus.lc_mode)
		
		threshold = dataclasses.spe_pmt_threshold(this_domstatus,
							this_domcal) / I3Units.mV
		if threshold < 0 :
			print '  %s  %f' % (str(e), threshold)

		atwdaSamplingRate = dataclasses.atwd_sampling_rate(0,this_domstatus,this_domcal)
		if atwdaSamplingRate < 295*I3Units.megahertz or atwdaSamplingRate > 310*I3Units.megahertz :
			print '  %s  ATWDaSamplingRate = %s MHz!!' % (str(e), atwdaSamplingRate/I3Units.megahertz)
			
		atwdbSamplingRate = dataclasses.atwd_sampling_rate(1,this_domstatus,this_domcal)
		if atwdbSamplingRate < 295*I3Units.megahertz or atwdbSamplingRate > 310*I3Units.megahertz :
			print '  %s  ATWDbSamplingRate = %s MHz!!' % (str(e), atwdbSamplingRate/I3Units.megahertz)
					
		# checks for DOMcalibrator
		if this_domcal.dom_cal_version[:3] != "7.6" :
			print '  %s  dom_cal_version = %s !!' % (str(e), this_domcal.dom_cal_version)
		
		# checks for topsimulator : snowheight (part of TankGeo), vemcal, MET (later)
		if e.om> 60:
			vemthis_domcal = vem_cal[e]
			if ( (vemthis_domcal.pe_per_vem <= 0) or (vemthis_domcal.pe_per_vem > 220)):
				print 'IceTop %s pe_per_vem is %f' % (e,vemthis_domcal.pe_per_vem)
			if( (vemthis_domcal.corr_factor <= 0) or (vemthis_domcal.corr_factor > 1.1)):
				print 'IceTop %s corr_factor is %f' % (e,vemthis_domcal.corr_factor)
			if( (this_domstatus.dom_gain_type==dataclasses.I3DOMStatus.DOMGain.High) and \
			    (vemthis_domcal.hglg_cross_over <= 1500) or (vemthis_domcal.hglg_cross_over > 4000)): 
				print 'IceTop %s hglg_cross_over is %f' % (e,vemthis_domcal.hglg_cross_over)


for e,st in sta_geo:
	for t in st:
		snowh = t.snowheight
		omkeyInTank = [omk for omk in t.omkey_list]
		if ((snowh < 0.01*I3Units.m) or (snowh > 1.8*I3Units.m)):
			print 'IceTop station %s with DOMs(%s) snowheight is %f' % (e,omkeyInTank,snowh)
