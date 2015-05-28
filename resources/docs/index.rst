Sim-services
============

This project contains the following classes, none of which are documented, so you have to look at the source code to see what they do.  This is highly sub-optimal and will be fixed in the next release.

- I3ModifyEvent
- I3SimConstants
- I3PropagatorService
- I3SumGenerator
- I3MCTreeHybridSimulationSplitter
- I3MCPEtoI3MCHitConverter
- I3DownsampleMCPE
- I3PropagatorModule
- I3InIceCORSIKATrimmer
- I3CombineMCPE
- I3RemoveLargeDT - Removes any I3MCPEs with times beyond the distribution's median time +/- MaxDeltaT/2.
 * MaxDeltaT (Default : 100ms) - Largest time span of PEs in an event.
 * InputResponse (Default : "I3MCPESeriesMap") - Name of input I3MCPESeriesMap.
 * OutputResponse (Default : "OutputResponse") - Name of output I3MCPESeriesMap.
 * PreSorted (Default : true ) - If false PEs will be sorted. 
   
Sanity Checkers
----------------
sanity_checkers

GCD Validation
----------------
gcd_validation

Code Review 
^^^^^^^^^^^ 
.. toctree:: 
   :titlesonly: 

   code_review
