Sim-services
============

.. toctree::
   :maxdepth: 3
   
   release_notes

Undocumented Code
-----------------

This project contains the following classes, none of which are documented, so you have to look at the source code to see what they do.  This is highly sub-optimal and will be fixed in the next release.

- I3ModifyStartTime - Modifies the start time in the I3EventHeader in the frame.
- I3SimConstants - A list of static variables commonly used by physics simulation specifically cross-section calculations
- I3PropagatorService - Abstract base class for lepton propagator services.
- I3SumGenerator - A class for generating the sum of several random numbers which all have the same probability density.
- I3MCTreeHybridSimulationSplitter - Splits an I3MCTree into two trees, one for tracks and one for cascades.
- I3MCPEtoI3MCHitConverter - Converts an I3MCPESeriesMap to an I3MCHitSeriesMap.
- I3DownsampleMCPE - Randomly downsample MCPEs from one collection to another.
- I3PropagatorModule - Propagates all particles found in an MCTree.
- I3InIceCORSIKATrimmer - Removes muons that have no chance of reaching the detector.
- I3CombineMCPE - Combines several I3MCPEHitSeriesMaps into one.

Deprecated Modules
------------------
* I3ModifyEventID - This modified more than the event ID *and* it was actually only used to set the start time, so this has been replaced with I3ModifyStartTime.
   
I3RemoveLargeDT 
---------------
.. toctree:: 
   :titlesonly: 

   remove_large_dt

Sanity Checkers
----------------
.. toctree:: 
   :titlesonly: 

   sanity_checkers

GCD Validation
----------------
.. toctree:: 
   :titlesonly: 

   gcd_validation

Code Review 
^^^^^^^^^^^ 
.. toctree:: 
   :titlesonly: 

   code_review
