#!/usr/bin/env python

import numpy as n
import pylab as p
from icecube.sim_services.sanity_checker.kombu import histfactory

bins = n.linspace(-10,10,101)
h1 = histfactory.generate_hist1d(n.random.normal(2,2,1e3), bins)
h2 = histfactory.generate_hist1d(n.random.normal(2,2,1e5), bins)

p.figure(figsize=(8,4))

(h1/1e8).scatter(log=1)
(h2/1e4).scatter(log=1)

p.savefig("histmath.png")
