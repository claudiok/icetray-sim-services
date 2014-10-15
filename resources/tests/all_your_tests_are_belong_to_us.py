#!/usr/bin/env python

from icecube import icetray
from icecube.sim_services.sanity_checker.utils.configurations import HistogramConfigurations
from icecube.sim_services.sanity_checker.utils.configurations import RunConfigurations
from icecube.sim_services.sanity_checker.test_framework import test_registry
from icecube.sim_services.sanity_checker.bases.histogram import Histogram
from icecube.sim_services.sanity_checker.bases.sanity_checker import SanityChecker
from icecube.sim_services.sanity_checker.test_framework import TestFailure

import register_tests

histograms_that_should_have_tests = \
  set([h.name for run_type, hists in HistogramConfigurations.iteritems()\
       for h in hists])

histogram_tests = [ t for t in test_registry.registry \
                    if isinstance(t.module, Histogram)]                                        
histograms_that_have_tests = set([t.module.name for t in histogram_tests])

histograms_that_have_no_tests = \
  histograms_that_should_have_tests.difference(histograms_that_have_tests)

if len(histograms_that_have_no_tests) > 0 :
    print("The following histograms have no tests :")
    for name in histograms_that_have_no_tests :
        print("   %s" % name)

modules_that_should_have_tests = \
  set([m.pprint() for run_type, modules in RunConfigurations.iteritems()\
       for m in modules])

module_tests = [ t for t in test_registry.registry \
                 if isinstance(t.module, SanityChecker)]
modules_that_have_tests = set([t.module.name for t in module_tests])

modules_that_have_no_tests = \
  modules_that_should_have_tests.difference(modules_that_have_tests)

if len(modules_that_have_no_tests) > 0 :
    print("The following modules have no tests :")
    for name in modules_that_have_no_tests :
        print("   %s" % name)

if len(histograms_that_have_no_tests) > 0 \
  or len(modules_that_have_no_tests) > 0 :
    raise TestFailure("One or more modules have no tests.")
    
