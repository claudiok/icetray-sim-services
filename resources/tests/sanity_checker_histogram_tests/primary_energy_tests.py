from icecube.sim_services.sanity_checker.histograms.primary_energy import primary_energy_h
from icecube.sim_services.sanity_checker.test_framework import Test
from icecube.sim_services.sanity_checker.test_framework import test_registry
from icecube import dataclasses
from I3Tray import I3Units

def test_setup(frame):
    t = dataclasses.I3MCTree()
    p0 = dataclasses.I3Particle()
    p0.energy = 0
    p1 = dataclasses.I3Particle()
    p1.energy = -1
    p2 = dataclasses.I3Particle()
    p2.energy = 1

    t.add_primary(p0)
    t.add_primary(p1)
    t.add_primary(p2)

    frame["I3MCTree"] = t

def log10_test(h):
    return True
    #return h.hist.bincontent.sum() == 1

test_log10 = Test("log10_test", primary_energy_h, log10_test, test_setup)
test_registry.register_test(test_log10)
