#!/usr/bin/env python

from icecube import icetray
from icecube.sim_services.sanity_checker.utils.configurations import RunConfigurations
from icecube.sim_services.sanity_checker.utils.configurations import HistogramConfigurations

result = True
report = dict()
for run_type, modules in RunConfigurations.iteritems() :
    for module in modules :
        frame = icetray.I3Frame()
        n_tests = len([t for t in dir(module) \
                       if t.startswith("test") and callable(getattr(module,t))])
        if n_tests == 0 :
            print("FAIL : '%s' has no tests" % module.pprint())
            result = False
        else:
            module.setup_test(frame)
            for test in dir(module) :
                if test.startswith("test") and callable(getattr(module,test)) :
                    key = "%s : %s" % (str(module),test)
                    test_result = getattr(module,test)(frame)
                    report[key] = test_result
                    result = result and test_result

for run_type, histograms in HistogramConfigurations.iteritems() :
    for hist in histograms :
        frame = icetray.I3Frame()

        n_tests = len([t for t in dir(hist) \
                       if t.startswith("test") and callable(getattr(hist,t))])
        if n_tests == 0 :
            print("FAIL : '%s' has no tests" % hist.name)
            result = False

        else :
            hist.setup_test(frame)            
            hist.fill(frame)
            hist.generate_histogram()

            for test in dir(hist) :
                if test.startswith("test") and callable(getattr(hist,test)) :
                    key = "%s : %s" % (hist.name,test)
                    test_result = getattr(hist,test)(hist,frame)
                    report[key] = test_result
                    result = result and test_result

if not result :
    n_fail = 0
    for testname, result_local in report.iteritems() :
        print("%s : %s " % (testname, result_local))
        if not result_local :
            print ("  n_fail = %d" % n_fail)
            n_fail += 1
    print("%d out of %d tests failed" % (n_fail, len(report)))
    raise Exception
            
    

