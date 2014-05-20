#!/usr/bin/env python

from icecube import icetray
from icecube.sim_services.sanity_checker.utils.configurations import RunConfigurations
from icecube.sim_services.sanity_checker.utils.configurations import HistogramConfigurations

result = True
report = dict()
for run_type, module in RunConfigurations.iteritems() :
    frame = icetray.I3Frame()
    module.setup_test(frame)
    for test in dir(module) :
        if test.startswith("test") and callable(getattr(module,test)) :
            key = "%s : %s" % (str(module),test)
            test_result = getattr(module,test)(frame)
            report[key] = test_result
            result = result and test_result

for run_type, hist in HistogramConfigurations.iteritems() :
    frame = icetray.I3Frame()
    hist.setup_test(frame)
    hist.fill(frame)
    hist.generate_histogram()
    for test in dir(hist) :
        if test.startswith("test") and callable(getattr(hist,test)) :
            key = "%s : %s" % (str(hist),test)
            test_result = getattr(module,test)(frame)
            report[key] = test_result
            result = result and test_result

if not test_result :
    n_fail = 0
    for testname, result in report.iteritems() :
        print("%s : %s " % (testname, result))
        if not result : n_fail += 1
    print("%d out of %d tests failed" % (n_fail, len(report)))
    raise Exception
            
    

