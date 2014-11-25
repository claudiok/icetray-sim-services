from icecube.sim_services.sanity_checker.modules.mctree_checkers import InIceMCTreeChecker
from icecube.sim_services.sanity_checker.test_framework import Test
from icecube.sim_services.sanity_checker.test_framework import test_registry
from icecube import dataclasses as dc
from I3Tray import I3Units

##########
# Empty Tree Test
##########
def setup_empty_tree(frame):
    frame["I3MCTree"] = dc.I3MCTree()

def empty_tree_cond(module):
    return module.emptyTreeCounter.counter > 0 

empty_tree_test = Test("empty_tree_test", InIceMCTreeChecker(),
                       empty_tree_cond, setup_empty_tree)

##########
# Empty Tree Test
##########
def setup_unpropagated_taus(frame):
    tree = dc.I3MCTree()
    p = dc.I3Particle()
    p.type = dc.I3Particle.TauPlus
    p.location_type = dc.I3Particle.InIce
    p.length = 0. * I3Units.m
    p.energy = 10. * I3Units.TeV
    p.pos = dc.I3Position(0*I3Units.km,0*I3Units.km,0*I3Units.km)
    tree.add_primary(p)
    frame["I3MCTree"] = tree

def unpropagated_taus_cond(module):
    return module.unpropagatedTausCounter.counter > 0 

unpropagated_taus_test = Test("unpropagated_taus_test", InIceMCTreeChecker(),
                              unpropagated_taus_cond,
                              setup_unpropagated_taus)

##########
# Empty Tree Test
##########
def setup_propagated_taus_outside_active_volume(frame):
    # a tau with a finite length and no daughters outside the active volume is ok
    tree = dc.I3MCTree()
    p = dc.I3Particle()
    p.type = dc.I3Particle.TauPlus
    p.location_type = dc.I3Particle.InIce
    p.length = 20. * I3Units.m
    p.energy = 10. * I3Units.TeV
    p.pos = dc.I3Position(1*I3Units.km,1*I3Units.km,1*I3Units.km)
    tree.add_primary(p)
    frame["I3MCTree"] = tree

def propagated_taus_outside_active_volume_cond(module):
    return module.unpropagatedTausCounter.counter == 0 

propagated_taus_outside_active_volume_test = Test("propagated_taus_outside_active_volume_test",
                                                InIceMCTreeChecker(),
                                                propagated_taus_outside_active_volume_cond,
                                                setup_propagated_taus_outside_active_volume)


test_registry.register_test(empty_tree_test)
test_registry.register_test(unpropagated_taus_test)
test_registry.register_test(propagated_taus_outside_active_volume_test)

