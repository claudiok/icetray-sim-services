#!/usr/bin/env python

from optparse import OptionParser

parser = OptionParser()
parser.add_option("-i","--inputpath", dest="INPATH",help="Path to GCD files to merge.")
parser.add_option("-o","--output", dest="OUTPUT",help="Name of output GCD file.")
(options, args) = parser.parse_args()

import os
import sys

from numpy import isnan

from icecube import icetray
from icecube import dataio
from icecube import dataclasses
from icecube.sim_services.sim_utils.gcd_utils import get_omgeo
from icecube.sim_services.sim_utils.gcd_utils import get_domcal

gcd_files = [(options.INPATH + fn,dataio.I3File(options.INPATH + fn)) \
             for fn in os.listdir(options.INPATH) \
             if fn.endswith(".i3.gz")]

print len(gcd_files) 
# verify the geometries are the same

# loop through and compare D and C frames
# checking *just* for things that PE generators need, which are
# 1) existence
# 2) RDE

# let's check for consistency first
omgeo0 = get_omgeo(gcd_files[0][1])

consistent_geometries = True
for fn,gcd in gcd_files[1:]:
    omgeo1 = get_omgeo(gcd)

    for omkey,i3omgeo in omgeo0:
        if omkey not in omgeo1 :
            print("ERROR : %s not in omgeo1")
            consistent_geometries = False
    for omkey,i3omgeo in omgeo1:
        if omkey not in omgeo0 :
            print("ERROR : %s not in omgeo0")
            consistent_geometries = False

if not consistent_geometries :
    print "not going to work"
    sys.exit()
    
print "so far so good..."

domcal0 = get_domcal(gcd_files[0][1])
corrections = list()
for omkey,i3omgeo in omgeo0:
    if omkey not in domcal0 \
           or isnan(domcal0[omkey].relative_dom_eff):
        # look for one in the other GCD files
        found = False
        for fn,gcd in gcd_files[1:]:
            domcal1 = get_domcal(dataio.I3File(fn))
            if omkey in domcal1 :
                if not isnan(domcal1[omkey].relative_dom_eff) :
                    found = True
                    print "found one for %s in %s" % (str(omkey), fn)
                    corrections.append(omkey)
                    if omkey not in domcal0 :
                        print "  replacing"
                        print "    RDE = ",domcal1[omkey].relative_dom_eff
                        domcal0[omkey] = domcal1[omkey]
                    else:
                        print "  resetting"
                        domcal0[omkey].relative_dom_eff = domcal1[omkey].relative_dom_eff
                    break
        if not found :
            print "nothing we can do for %s" % str(omkey)

print len(corrections)

for omkey,dc in domcal0 :
    if isnan(dc.relative_dom_eff) :
        print "%s still has a bad RDE"

geo_frame = None
cal_frame = None
stat_frame = None

old_gcd_file = dataio.I3File(gcd_files[0][0])
while old_gcd_file.more() :
    fr = old_gcd_file.pop_frame()
    print fr
    if fr.Stop == icetray.I3Frame.Geometry :
        geo_frame = fr
    if fr.Stop == icetray.I3Frame.Calibration :
        cal_frame = fr
    if fr.Stop == icetray.I3Frame.DetectorStatus :
        stat_frame = fr

if not geo_frame or not cal_frame or not stat_frame :
    print "fuck"
    sys.exit()

cal_frame["I3Calibration"].dom_cal = domcal0

outfile = dataio.I3File(options.OUTPUT, dataio.I3File.Mode.Writing)
outfile.push(geo_frame)
outfile.push(cal_frame)
outfile.push(stat_frame)
outfile.close()    
    
