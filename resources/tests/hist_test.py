#!/usr/bin/env python
import cPickle as pickle
import random
from os.path import expandvars
from icecube.icetray.I3Test import *

from icecube.sim_services.sanity_checker.utils.histogram import Histogram
from icecube.sim_services.sanity_checker.utils.histogram import calc_rchisq

REF_PATH = expandvars("$I3_BUILD/sim-services/resources/")

s = {"xmin": 0., "xmax": 1. , "binwidth" : 0.1}
h = Histogram( settings = s )

for i in range(1000):
    h.fill( random.uniform( -1., 2.) )

of = open(REF_PATH + "histogram_io_test.pickle","w")
pickle.dump( h , of )
of.close()


f = open(REF_PATH + "histogram_io_test.pickle","r")
ih = pickle.load( f )

# make sure these are different objects
ENSURE( h != ih, "these should be different objects" )

# poor man's object comparison
ENSURE( str(h) == str(ih), "they should 'look' the same" )

h_target = Histogram( rhist = h )

last_value = None
for i in range(10000):
    h_target.fill( random.uniform( -1., 2.) )    
    this_value = h_target.is_within_tolerance()
    rchisq = calc_rchisq( h_target, h )
    if this_value != last_value :
        print " %d : transition from %s to %s " % \
              ( i, str(last_value), str(this_value) )
    last_value = this_value

ENSURE( last_value < 2.0 , "chisq should converge close to 1" )





