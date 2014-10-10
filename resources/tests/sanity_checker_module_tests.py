#!/usr/bin/env python

from icecube import icetray
from icecube.sim_services.sanity_checker.utils.configurations import RunConfigurations

modules_to_test = dict()

for run_type, modules in RunConfigurations.iteritems() :
    for module in modules :
        if module.__class__.__name__ not in modules_to_test.keys() :
            modules_to_test[module.__class__.__name__] = module

all_is_well = True
report = dict()
n_modules_with_no_tests = 0

for class_name, module in modules_to_test.iteritems() :
    frame = icetray.I3Frame()
    n_tests = len([t for t in dir(module) \
            if t.startswith("test") and callable(getattr(module,t))])
    if n_tests == 0 :
        print("ERROR : Module '%s' has no tests" % module.pprint())
        n_modules_with_no_tests += 1
        all_is_well = False
    else:
        for test in dir(module) :
            if test.startswith("test") and callable(getattr(module,test)) :
                module.setup_test[str(test)](frame)
                key = "%s : %s" % (str(module),test)
                test_result = getattr(module,test)(frame)
                report[key] = test_result
                all_is_well = all_is_well and test_result

if not all_is_well :
    n_fail = 0
    for testname, test_passed in report.iteritems() :
        print("%s : %s " % (testname, test_passed))
        if not test_passed :
            n_fail += 1
    print("%s : %d out of %d tests failed" % \
          ("PASS" if n_fail == 0 else "FAIL", n_fail, len(report)))
    print("%s : %d out of %d modules have no tests" % \
          ("PASS" if n_modules_with_no_tests == 0 else "FAIL", \
          n_modules_with_no_tests, len(modules_to_test)))
    raise Exception
            
    

