#!/usr/bin/env python

from icecube import dataio
from icecube import dataclasses
from optparse import OptionParser

parser = OptionParser()

parser.add_option("-i","--infile",
                  dest="INFILE", 
                  help="Pickle file with the histograms.")

parser.add_option("-p","--output_path",
                  dest="OUTPATH", 
                  help="Path to dump the histograms.")

(options, args) = parser.parse_args()

import cPickle as pickle
f = open(options.INFILE)
histograms = pickle.load(f)

for title, hist in histograms.iteritems() :
    hist.draw(path = options.OUTPATH)
