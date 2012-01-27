#!/usr/bin/env python

from icecube.sim_services.sanity_checker.utils.histogram import Histogram

from os.path import expandvars
REF_PATH = expandvars("$I3_BUILD/sim-services/resources/")
f = open(REF_PATH + "histogram_test.pickle","r")
import cPickle as pickle
h = pickle.load( f )

print "xmin = ", h.xmin
print "xmax = ", h.xmax
print "binwidth = ", h.binwidth
print "underflow = ",h.underflow
print "overflow = ", h.overflow
print "nbins = ", h.nbins
print "leading_edges = ", h.leading_edges
print "bin_values = ", h.bin_values
print "rchisq_tolerance = ", h.rchisq_tolerance






