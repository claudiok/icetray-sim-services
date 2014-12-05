#!/usr/bin/env python

from icecube import dataio
from icecube import dataclasses
from optparse import OptionParser

parser = OptionParser()

parser.add_option("-i","--infile",
                  dest="INFILE", 
                  help="Pickle file with the histograms.")

parser.add_option("-p","--output_path",
                  dest="OUTPATH", default = './',
                  help="Path to dump the histograms.")

(options, args) = parser.parse_args()

import cPickle as pickle
f = open(options.INFILE)
contents = pickle.load(f)

for title, hist in contents['Histograms'].iteritems() :
    print title
    #hist.draw(path = options.OUTPATH)
    hist.save()
