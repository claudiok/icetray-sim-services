#!/usr/bin/env python

from I3Tray import *
from icecube import icetray, dataclasses, dataio, simclasses
from os.path import expandvars

from optparse import OptionParser
parser = OptionParser()

parser.add_option("-g","--gcd_file", dest="GCDFILE",                  
                  default=expandvars("$I3_PORTS/test-data/sim/GeoCalibDetectorStatus_IC59.55040.i3.gz"),
                  help="I3File which contains the GCD.")

parser.add_option("-i","--infile", dest="INFILE",                   
                  help="Input file to read.")

parser.add_option("-m","--nhits_per_DOM", type = "int",
                  dest="nhits_per_DOM", default=10000,
                  help="Number of hits per DOM")

(options, args) = parser.parse_args()

f = dataio.I3File(options.INFILE)

from icecube.BadDomList import bad_dom_list_static
badDOMList = bad_dom_list_static.IC79_static_bad_dom_list_HLC()

from icecube.sim_services.sim_utils.gcd_utils import get_omgeo, get_domcal, get_domstatus
omgeo = get_omgeo( dataio.I3File(options.GCDFILE) )
domcal = get_domcal( dataio.I3File(options.GCDFILE) )
domstat = get_domstatus( dataio.I3File(options.GCDFILE) )
goodDOMList = [omkey for omkey,g in omgeo \
               if badDOMList.count(omkey) == 0 and omkey.GetOM() <= 60 and omkey.GetString() > 0]

counter = 0
while f.more():
    counter += 1
    frame = f.pop_physics()

    print "[  Frame %d ]" % (counter)

    rpmap = frame.Get("NFEATWDPulses")
    pmtmap = frame.Get("MCPMTResponseMap")

    # make sure this DOM is not in the bad DOM list
    for omkey, rpseries in rpmap :
        if badDOMList.count(omkey) > 0 :
            print "%s : this DOM is in the BAD DOM List!!!" % str(omkey)

        charge = sum([rp.Charge for rp in rpseries])
        if(charge/float(options.nhits_per_DOM) < 0.2 or \
           charge/float(options.nhits_per_DOM) > 2.0 ) :
            print "%s : what do you think about this (%f) charge and this (%f) charge ratio? " % \
                  (str(omkey),charge,charge/float(options.nhits_per_DOM))
            
    # make sure every DOM in the good DOM list has a hit
    for omkey in goodDOMList :
        if omkey not in rpmap:
            print "%s : this DOM is good but produced no hits!!!" % str(omkey)
            if omkey not in pmtmap :
                print "   %s : this DOM has no PMT waveform!!!" % str(omkey)
            if omkey not in domcal :
                print "   %s : this DOM has no domcal entry!!!" % str(omkey)
            else:
                print "        impedance = %f ohms" % ( (domcal[omkey].FrontEndImpedance)/I3Units.ohm)
            if omkey not in domstat :
                print "   %s : this DOM has no domstat entry!!!" % str(omkey)
            else:
                print "        voltage = %f V" % ( (domstat[omkey].pmtHV)/I3Units.V)
            if omkey in domcal and omkey in domstat :
                print "        gain = %f " % ( dataclasses.PMTGain(domstat[omkey],domcal[omkey]) )
                print "        ttime = %f ns " % ( dataclasses.TransitTime(domstat[omkey],domcal[omkey])/I3Units.ns )
            
