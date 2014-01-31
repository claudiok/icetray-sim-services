#!/usr/bin/env python

from icecube import dataio
from icecube import dataclasses
from optparse import OptionParser

parser = OptionParser()

parser.add_option("-i","--infile",
                  dest="INFILE", 
                  help="I3File to check.")

parser.add_option("-o","--outfile",
                  dest="OUTFILE", 
                  help="Output pickle file.")

parser.add_option("-r","--run_type",
                  dest="RUNTYPE", 
                  help="Type of run.")

(options, args) = parser.parse_args()

f = dataio.I3File(options.INFILE)

from icecube.sim_services.sanity_checker.utils.configurations import HistogramConfigurations

histograms = dict()
for h in HistogramConfigurations[options.RUNTYPE] :
    histograms[h.draw_args["title"]] = h

while f.more() :
    fr = f.pop_frame()
    for t,h in histograms.iteritems() : h.fill(fr)

for t,h in histograms.iteritems() :
    h.generate_histogram()

import cPickle as pickle
f = open(options.OUTFILE, "w")
pickle.dump(histograms,f)

