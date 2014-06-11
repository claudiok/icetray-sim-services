#!/usr/bin/env python

from optparse import OptionParser

parser = OptionParser()

parser.add_option("-r","--reference_pickle_filename",
                  dest="REFFILENAME", 
                  help="Name of reference file with histograms.")

parser.add_option("-t","--test_pickle_filename",
                  dest="TESTFILENAME", 
                  help="Name of test file with histograms")

(options, args) = parser.parse_args()

import os
import cPickle as pickle
from icecube.sim_services.sanity_checker.utils.hist_comp import compare

ref_histos = pickle.load(open(options.REFFILENAME))
test_histos = pickle.load(open(options.TESTFILENAME))

for title, ref_hist in ref_histos.iteritems() :
    test_hist = test_histos[title]
    if test_hist :
        print title, compare(test_hist, ref_hist)
