#!/usr/bin/env python

from icecube import icetray
from icecube.sim_services.sanity_checker.utils.configurations import HistogramConfigurations

all_is_well = True
report = dict()
n_histograms_with_no_tests = 0

histograms_to_test = dict()
for run_type, histograms in HistogramConfigurations.iteritems() :
    for hist in histograms :
        if hist.name not in histograms_to_test.keys() :
            histograms_to_test[hist.name] = hist

for hist_name, hist in histograms_to_test.iteritems() :
    frame = icetray.I3Frame()

    n_tests = len([t for t in dir(hist) \
        if t.startswith("test") and callable(getattr(hist,t))])
    if n_tests == 0 :
        print("ERROR : Histogram '%s' has no tests" % hist.name)
        n_histograms_with_no_tests += 1
        all_is_well = False
    else :
        for test in dir(hist) :
            if test.startswith("test") and callable(getattr(hist,test)) :
                hist.setup_test[test](frame)            
                hist.fill(frame)
                hist.generate_histogram()

                key = "%s : %s" % (hist.name,test)
                test_result = getattr(hist,test)(hist,frame)
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
    print("%s : %d out of %d histograms have no tests" % \
          ("PASS" if n_histograms_with_no_tests == 0 else "FAIL", \
           n_histograms_with_no_tests, len(histograms_to_test)))
    raise Exception
            
    

