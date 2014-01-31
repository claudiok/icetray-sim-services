#!/usr/bin/env python

from optparse import OptionParser

parser = OptionParser()

parser.add_option("-i","--inpath",
                  dest="INPATH", 
                  help="Path to input pickle files.")

parser.add_option("-o","--output_pickle_filename",
                  dest="OUTFILENAME", 
                  help="Name of output file with combined histograms.")

parser.add_option("-p","--output_plot_path",
                  dest="OUTPATH", 
                  help="Name of output path to dump plots.")

(options, args) = parser.parse_args()

import os
import cPickle as pickle

if not options.INPATH.endswith('/') :
    options.INPATH += '/'

histograms = None
for fn in os.listdir(options.INPATH) :
    if fn.endswith(".pickle") :        
        f = open(options.INPATH + fn)
        if not histograms :
            histograms = pickle.load(f)
            f.close()
            continue

        new_histograms = pickle.load(f)
        for key,hist in histograms.iteritems() :
            hist += new_histograms[key]
        f.close()

for title, hist in histograms.iteritems() :
    hist /= len(histograms)

f = open(options.OUTFILENAME, "w")
pickle.dump(histograms,f)
f.close()

for title, hist in histograms.iteritems() :
    hist.draw(path = options.OUTPATH)
