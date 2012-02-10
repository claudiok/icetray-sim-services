#!/usr/bin/env python

from icecube.sim_services.sanity_checker.utils.histogram import Histogram

s = {"xmin": 0., "xmax": 1. , "binwidth" : 0.1}
h = Histogram( settings = s )

import random

for i in range(1000):
    h.fill( random.uniform( -1., 2.) )

print str( h )

from os.path import expandvars
REF_PATH = expandvars("$I3_BUILD/sim-services/resources/")
f = open(REF_PATH + "histogram_test.pickle","w")

import cPickle as pickle
pickle.dump( h , f )




