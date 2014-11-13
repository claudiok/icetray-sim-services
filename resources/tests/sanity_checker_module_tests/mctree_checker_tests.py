from icecube.sim_services.sanity_checker.modules.mctree_checkers import InIceMCTreeChecker
from icecube.sim_services.sanity_checker.test_framework import Test
from icecube.sim_services.sanity_checker.test_framework import test_registry
from icecube import dataclasses
from I3Tray import I3Units

def setup_empty_tree(frame):
    frame["I3MCTree"] = dataclasses.I3MCTree()

def empty_tree_cond(module):
    return module.emptyTreeCounter.counter > 0 

checker = InIceMCTreeChecker()
empty_tree_test = Test("empty_tree_test", checker, empty_tree_cond, setup_empty_tree)
test_registry.register_test(empty_tree_test)

