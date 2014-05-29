#!/usr/bin/env python

from optparse import OptionParser

parser = OptionParser()

parser.add_option("-t","--test_pickle_filename",
                  dest="TESTFILENAME", 
                  help="Path to input pickle file.")

parser.add_option("-b","--benchmark_pickle_filename",
                  dest="BENCHMARKFILENAME", 
                  help="Path to benchmark pickle file.")

parser.add_option("-p","--output_plot_path",
                  dest="OUTPATH", 
                  help="Name of output path to dump plots.")

(options, args) = parser.parse_args()

import os
import cPickle as pickle
from icecube.sim_services.sanity_checker.utils.hist_comp import compare

test_file = open(options.TESTFILENAME)
bm_file = open(options.BENCHMARKFILENAME)

test_dict = pickle.load(test_file)
bm_dict = pickle.load(bm_file)

#print test_dict['Histograms'].keys()

all_is_well = True
for key, test_hist in test_dict['Histograms'].iteritems() :
    if key in bm_dict['Histograms'] :
        bm_hist = bm_dict['Histograms'][key]
        result = compare( test_hist, bm_hist, test_norm = False )
        all_is_well = all_is_well and result[1]
        print("%s : %s" % (key,result[1]))

print("Tests %s." % ("PASSED" if all_is_well else "FAILED"))


