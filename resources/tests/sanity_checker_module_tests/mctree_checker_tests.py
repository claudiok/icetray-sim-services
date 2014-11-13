from icecube.sim_services.sanity_checker.modules.mctree_checkers import InIceMCTreeChecker
from icecube.sim_services.sanity_checker.test_framework import Test
from icecube.sim_services.sanity_checker.test_framework import test_registry
from icecube import dataclasses as dc
from I3Tray import I3Units

def setup_empty_tree(frame):
    frame["I3MCTree"] = dc.I3MCTree()

def empty_tree_cond(module):
    return module.emptyTreeCounter.counter > 0 

def setup_unpropagated_taus(frame):
    tree = dc.I3MCTree()
    p = dc.I3Particle()
    p.type = dc.I3Particle.TauPlus
    p.location_type = dc.I3Particle.InIce
    p.length = 0. * I3Units.m
    p.energy = 10. * I3Units.TeV
    tree.add_primary(p)
    frame["I3MCTree"] = tree

def unpropagated_taus_cond(module):
    return module.unpropagatedTausCounter.counter > 0 

checker = InIceMCTreeChecker()

empty_tree_test = Test("empty_tree_test", checker,
                       empty_tree_cond, setup_empty_tree)
unpropagated_taus_test = Test("unpropagated_taus_test", checker,
                              unpropagated_taus_cond, setup_unpropagated_taus)

test_registry.register_test(empty_tree_test)
test_registry.register_test(unpropagated_taus_test)

