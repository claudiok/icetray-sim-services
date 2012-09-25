#!/usr/bin/env python

import sys, os
from os.path import expandvars
from math import isnan

from I3Tray import *
from icecube import icetray, dataclasses, dataio, simclasses
from icecube.icetray import OMKey

from optparse import OptionParser

parser = OptionParser()

default_fn = expandvars("$I3_PORTS/test-data/sim/GeoCalibDetectorStatus_2012.56062_candidate.i3.gz")
parser.add_option("-i","--infile",
                  dest="INFILE", default = default_fn,
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

badOMs = list()
if "BadDomsList" in status_frame :
    print "Found a BadDomsList in the frame."
    print "Using this one instead."
    badOMs = status_frame.Get("BadDomsList")
else:
    print status_frame
    try :
        from icecube.BadDomList import bad_dom_list_static
        badOMs = bad_dom_list_static.IC86_static_bad_dom_list()
    except ImportError :
        print "ERROR : BadDomsList wasn't found in the frame"
        print "and either the BadDomList doesn't exist or"
        print "there's no static_bad_dom_list."
        sys.exit(1)

dom_geo = geometry.omgeo
dom_cal = calibration.dom_cal
dom_status = status.dom_status

c_and_d_strings_to_check = range(1,87)

low_noise_DOMs_l = [ icetray.OMKey(82,54),  icetray.OMKey(84,54),  icetray.OMKey(85,55)]

strings_IC86 = [ 1, 7, 14, 22, 31, 79, 80 ]

high_QE = [ icetray.OMKey( 79, i ) for i in range(30, 45) if i not in [ 32, 41, 43 ] ]
high_QE.extend( [ icetray.OMKey( 80, i ) for i in range(30, 44) ] )
high_QE.extend( [ icetray.OMKey( 43, 55 ) ] )#Mark Krasberg (post deployment after surface testing)
for s in range(81,87):
    high_QE.extend( [ icetray.OMKey( s, i ) for i in range(1, 61) ] )

for e,p in dom_geo:

    if e not in badOMs and e in dom_cal and e in dom_status:
        cal_this_om = dom_cal[e]
        status_this_om = dom_status[e]

        if float(cal_this_om.dom_cal_version[:3]) < 7.5:
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

        if p.omtype == dataclasses.I3OMGeo.IceCube :
            if e not in badOMs and isnan( calibration.dom_cal[e].dom_noise_rate ) :
                print "ERROR : no valid noise rate for this DOM %s " % str(e)
                print "  NOT CORRECTING"

            if isnan(cal_this_om.relative_dom_eff) :
                if e.string in strings_IC86 :
                    calibration.dom_cal[e].relative_dom_eff = 1.35 if e in high_QE else 1.0 
                    print " correcting RDE from 'nan' to %.2f in %s" % (calibration.dom_cal[e].relative_dom_eff,e)

            # check for unusually low noise DOMs that were incorrectly translated into the DB
            if e in low_noise_DOMs_l :
                noiseRate = calibration.dom_cal[e].dom_noise_rate
                if noiseRate < 400 * I3Units.hertz :
                    calibration.dom_cal[e].dom_noise_rate = noiseRate + 1*I3Units.kilohertz
                    print "  correcting noise from %fHz to %fHz in %s" % (noiseRate/I3Units.hertz,
                                                                          calibration.dom_cal[e].dom_noise_rate/I3Units.hertz,e)


# let's clean the trigger cruft out
# we only simulate InIce and IceTop trigggers and
# only SM, Cluster, Cylinder, and SlowMonopole
# so that's all we're keeping
for tkey, ts in status.trigger_status :
    if ( tkey.source != dataclasses.I3Trigger.IN_ICE and tkey.source != dataclasses.I3Trigger.ICE_TOP) or \
           ( tkey.type != dataclasses.I3Trigger.SIMPLE_MULTIPLICITY and
             tkey.type != dataclasses.I3Trigger.STRING and
             tkey.type != dataclasses.I3Trigger.VOLUME and
             tkey.type != dataclasses.I3Trigger.SLOW_PARTICLE ) :
        del status.trigger_status[tkey]
    if tkey.source == dataclasses.I3Trigger.IN_ICE and \
           tkey.type == dataclasses.I3Trigger.SLOW_PARTICLE :
        tkey.type = dataclasses.I3Trigger.SIMPLE_MULTIPLICITY

# now we clean out the AMANDA geometry as well
for omkey, i3omgeo in geometry.omgeo:
  if i3omgeo.omtype == dataclasses.I3OMGeo.AMANDA :
    print "Removing AMANDA OM %s from the geometry." % str(omkey)
    del geometry.omgeo[omkey]

for omkey, i3omgeo in geometry.omgeo:
  if i3omgeo.omtype == dataclasses.I3OMGeo.AMANDA :
    print "ERROR: Why is this %s AMANDA OM still in the geometry? " \
          % str(omkey)


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
