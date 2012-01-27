#!/usr/bin/env python

from icecube.sim_services.sanity_checker.utils.histogram import Histogram

from os.path import expandvars
REF_PATH = expandvars("$I3_BUILD/sim-services/resources/")
f = open(REF_PATH + "histogram_test.pickle","r")
import cPickle as pickle
rhist = pickle.load( f )

h = Histogram( rhist = rhist )

import random

last_value = None
for i in range(10000):
    h.fill( random.uniform( -1., 2.) )
    this_value = h.is_within_tolerance()
    if this_value != last_value :
        print " %d : transition from %s to %s " % \
              ( i, str(last_value), str(this_value) )
    last_value = this_value





