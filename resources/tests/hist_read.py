#!/usr/bin/env python

from icecube.sim_services.sanity_checker.utils.histogram import Histogram

from os.path import expandvars
REF_PATH = expandvars("$I3_BUILD/sim-services/resources/")
f = open(REF_PATH + "histogram_test.pickle","r")
import cPickle as pickle
h = pickle.load( f )

print str( h )





