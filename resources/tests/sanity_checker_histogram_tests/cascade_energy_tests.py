from icecube.sim_services.sanity_checker.histograms.cascade_energy import cascade_energy_h
from icecube.sim_services.sanity_checker.test_framework import Test
from icecube.sim_services.sanity_checker.test_framework import test_registry
from icecube import dataclasses
from I3Tray import I3Units

def test_setup(frame):
    t = dataclasses.I3MCTree()
    for i in range(100) :
        p = dataclasses.I3Particle()
        p.type = dataclasses.I3Particle.EMinus
        p.energy = 1 * I3Units.GeV
        t.add_primary(p)
    frame["I3MCTree"] = t

def test_bin_content(h):
    return h.hist.bincontent.sum() == 100

test_basic = Test("bin_content_test",cascade_energy_h, test_bin_content, test_setup)
test_registry.register_test(test_basic)
