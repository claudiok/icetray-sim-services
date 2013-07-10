#!/usr/bin/env python
import sys
if sys.version_info[0] >= 3:
    import pickle
else:
    import cPickle as pickle
import pylab

f = open("sim-services/resources/simprod_reference_files/CORSIKA_comp.pickle", "rb")
c = pickle.load( f )

pylab.figure()

h = c[1].nHLCLaunchHist
print(h)

bw = h.binwidth
bc = [ le + bw/2.0 for le in h.leading_edges ]
pylab.plot( bc, h.bin_values, "bo" )

pylab.figure()
pylab.plot( bc, h.reference_hist.bin_values, "ro" )

pylab.show()
