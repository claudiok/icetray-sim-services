#!/usr/bin/env python

from icecube import icetray
from icecube.sim_services.sanity_checker.test_framework import test_registry
from icecube.sim_services.sanity_checker.bases.sanity_checker import SanityChecker
from icecube.sim_services.sanity_checker.test_framework import TestFailure

import register_tests

module_tests = [ t for t in test_registry.registry \
                 if isinstance(t.module, SanityChecker)]
                                        
all_tests_passed = True
report = dict()
for t in module_tests :
    frame = icetray.I3Frame()
    t.frame_setup(frame)
    all_is_well = t.module.check(frame)
    if not all_is_well :
        t.module.fail()        

    key = "%s : %s " % (t.module.pprint(), t.name)
    report[key] = all_is_well
    all_tests_passed = all_tests_passed and all_is_well

if not all_tests_passed :
    n_fail = 0
    for testname, test_passed in report.iteritems() :
        print("%s : %s " % (testname, test_passed))
        if not test_passed :
            n_fail += 1
    print("%s : %d out of %d tests failed" % \
          ("PASS" if n_fail == 0 else "FAIL", n_fail, len(report)))
    raise TestFailure("One or more tests failed.")
else :
    print("%d out of %d tests passed." \
          % (len(module_tests),len(report)))
