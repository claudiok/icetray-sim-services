#!/usr/bin/env python
import cPickle as pickle
import pylab

f = open("sim-services/resources/simprod_reference_files/CORSIKA_comp.pickle")
c = pickle.load( f )

pylab.figure()

h = c[1].nHLCLaunchHist
print h

bw = h.binwidth
bc = [ le + bw/2.0 for le in h.leading_edges ]
pylab.plot( bc, h.bin_values, "bo" )

pylab.figure()
pylab.plot( bc, h.reference_hist.bin_values, "ro" )

pylab.show()
