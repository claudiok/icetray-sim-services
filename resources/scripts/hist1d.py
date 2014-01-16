#!/usr/bin/env python

import numpy as n
import pylab as p
from icecube.sim_services.sanity_checker.kombu import histfactory

data1 = n.random.normal(2,2,1e3)
data2 = n.random.normal(5,4,1e3)
data = n.hstack((data1,data2))

h = histfactory.generate_hist1d( data, 40, label="x")

p.figure(figsize=(8,4))

p.subplot(131)
h.line()

p.subplot(132)
h.scatter()

p.subplot(133)
h.band()

p.savefig("hist1d.png")
