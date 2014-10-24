#!/usr/bin/env python

from optparse import OptionParser

parser = OptionParser()
parser.add_option("-i","--inputfile", dest="GCDFILENAME",help="GCD file.")
(options, args) = parser.parse_args()

if not options.GCDFILENAME :
    print("You must specify a GCD file. (e.g. '-i <GCD_FILENAME>')")
    sys.exit(1)

import sys
import os
import math
from os.path import expandvars

from icecube import icetray
from icecube import dataclasses
from icecube import simclasses

from gcd_setup import gcd_extract 

gcd = gcd_extract(options.GCDFILENAME)

dom_geo_map = gcd['dom_geo_map']
dom_cal_map = gcd['dom_cal_map']
dom_status_map = gcd['dom_status_map']
vem_cal_map = gcd['vem_cal_map']
station_geo_map = gcd['station_geo_map']
bad_dom_list = gcd['bad_dom_list']
high_qe_dom_list = gcd['high_qe_dom_list']

geo_strings_to_check = range(1,87)
for string in geo_strings_to_check:
    found = False
    for omkey, i3omgeo in dom_geo_map:
        if omkey.string == string :
            found = True
    if not found:
        print("string %d does not exist in the geometry" % string)

c_and_d_strings_to_check = range(1,87)
for string in c_and_d_strings_to_check :
    found_cal = False
    found_stat = False
    found_vemcal = False
    for omkey in [icetray.OMKey(string,om) for om in range(65)] :
        if omkey in dom_cal_map :
            found_cal = True
        if omkey in dom_status_map :
            found_stat = True
        if omkey.om > 60 and string < 82:
            if omkey in vem_cal_map:
                found_vemcal = True
            else :
                print('%s is missing from the VEMCal' % omkey)
        if found_cal and found_stat : continue
    if not found_cal : print('string %s is missing from the calibration' % string)
    if not found_stat : print('string %s is missing from the detector status' % string)


from icecube.photonics_service.gcd_test import photonics_hit_maker_test
from icecube.vuvuzela.gcd_test import vuvuzela_test
from icecube.DOMLauncher.gcd_test import pmt_response_sim_test
from icecube.DOMLauncher.gcd_test import dom_launcher_test
#from icecude.topsimulator.gcd_test import top_simulator_test

all_pass = True
for omkey, i3omgeo in dom_geo_map:

    if omkey not in bad_dom_list \
           and omkey in dom_cal_map \
           and omkey in dom_status_map:
        domcal = dom_cal_map[omkey]
        domstat = dom_status_map[omkey]

        pass_phm = photonics_hit_maker_test(omkey, i3omgeo, domcal)
        pass_vuvuzela = vuvuzela_test(omkey, i3omgeo, domcal)
        pass_pmt = pmt_response_sim_test(omkey, domcal, domstat)
        pass_dom_launcher = dom_launcher_test(omkey, i3omgeo, domcal, domstat)
        #pass_top_sim = top_simulator_test(omkey, i3omgeo, domcal, domstat)

        all_pass = all_pass \
            and pass_phm \
            and pass_vuvuzela \
            and pass_pmt \
            and pass_dom_launcher

# report back to the mothership
if not all_pass :
    sys.exit(1)
else :
    sys.exit(0)        

