Introduction
============
The goal of the project is to provide lightweight sanity checks on simulation production samples while they're running, flag problems, and stop the run if necessary.  The focus is on simplicity and portability.  Most problems in simulation production now are not easily caught, because they're not illegal for every frame, but accumulates over several frames.

Overview
========
All of the code is written in python (>= 2.6(?)) and the only hard dependency is matplotlib and numpy.

Very Broad Overview 
===================
The intended use for most people is going to be limited to writing sanity checkers themselves or hacking existing checkers.  To limit the CPU burden it's fine to just modify the existing checkers, but this is not necessary, especially if the checker you're writing isn't particularly CPU intensive.

* **SimulationSanityChecker** - This is the I3Module that you add to the end of your chain and has only two parameters.  This module currently lives in sim-services/python/sanity_checker/__init__.py.
- **RunType** - This will be a string that identifies the type of run you're checking.  (e.g. "CORSIKA" See Run Configurations below )
- **GenerateReferences** - A boolean, normally False, which when set to True will trigger the generation of reference histograms.

- **Run Configurations** - This is a simple dictionary that maps strings ( e.g. "CORSIKA" - the parameter that's passed to SimulationSanityChecker ) to a list of sanity checkers that should be installed.

- **Sanity Checker** - These are the modules in the RunConfigurations list that are run.  They inherit from Checker ( in sim-services/python/sanity_checker/checker.py ).  We'll go into this in more depth later.

* **Counter** - This is a simple class used by sanity checkers to keep track of how often an **undesirable** condition occurs.  The ``tolerance`` can be passed to the constructor.  When the internal counter ( counting the number of times the undesirable conditions occurs ) reaches the tolerance, the run is stopped.  **It's important to know that when the condition is false, the counter resets to 0.**  This should be used for conditions that should really never happen, but will happen under certain reasonable conditions.  For example, if the muon propagator is not installed all of the events will contain unpropagated muons ( i.e. InIce muons whose length is NaN ).  There are some cases where this is perfectly reasonable, but should not be common.  

* **Histogram** - Now that numpy and pylab are (or can be shipped) on all nodes, histogramming is done with a slim version of dashi_, called kombu.  More below.

.. _dashi : http://www.ifh.de/~middell/dashi/

To summarize, SimulationSanityChecker is the I3Module that loads various sanity checkers based on the run configuration.  Each sanity checker contains Counters and Histograms that are responsible for collecting statistics and flagging errors.  The plan is to have ( in sim-services/python/sanity_checker/modules ) one module for each frame object.  Those modules can contain various checkers for different run configurations.

Usage
=====
Running Sanity Checkers on Your Sample
--------------------------------------
The first thing you might want to do is run the SimulationSanityChecker on your sample.  Simply add the following to your script and set the ''RunType'' accordingly (See [[IceSim Sanity Checker Reference]] for a list of configurations).
 from icecube.sim_services.sanity_checker import SimulationSanityChecker
 tray.AddModule( SimulationSanityChecker, "sanitycheck",
 	 	RunType = "CORSIKA_weighted" )

Adding More Checks to Existing Sanity Checkers
----------------------------------------------
That was great and all, but there's a problem you want to flag that's currently not covered.  All you want to add is a small tweak to the existing sanity checkers.  For "CORSIKA_weighted" the module that checks the I3MCTree is called ``InIceTreeChecker`` and lives in sim-services/python/sanity_checker/modules/mctree_checkers.py ( NB : This is where all of the modules are and should be the only modules most people should modify. ).

Let's say you want to add the check for stochastics on muons.  Below is a slim sanity checker that currently only checks for NaN-length InIce muons.

::
 from icecube.sim_services.sanity_checkers.bases.sanity_checker import SanityChecker
 class InIceTreeChecker( SanityChecker ) :
     def __init__(self):
         SanityChecker.__init__(self)
         self.nanLengthMuonCounter = Counter()  
         self.nanLengthMuonCounter.failure_msg = "Too many events with NAN length InIce muons."
         self.registry.append(self.nanLengthMuonCounter)
         
     # returns True if all is well
     def check( self, frame ):
         mctree = frame.Get("I3MCTree")
 
         for p in mctree :
             # only consider inice muons
             if ( p.type == dc.I3Particle.MuPlus or \
                  p.type == dc.I3Particle.MuPlus ) and \
                  p.location_type == dc.I3Particle.InIce :
 
                 self.nanLengthMuonCounter.assert_true( isnan(p.length) )
 
         # call the base class 'check' method
         return SanityChecker.check(self)

So all you need to do is add a new Counter in the configure method, set its failure message, and register it.  If you don't like the default tolerance ( of 100 - meaning 'only stop the run if there are 100 events in a row meeting this condition' ), pass it to the Counter's constructor.

::
   self.daughterlessCounter = Counter( tolerance = 10 ) 
   self.daughterlessCounter.failure_msg = "Too many events with daughterless InIce muons."
   self.register( "daughterlessCounter"  )

Now in the 'check' method, this is where you're going to get whatever you need out of the frame, form the condition, and pass a bool to your Counter's ``assert_true`` method.  Adding the following lines in the loop ( and check for InIce muons ) should do the trick.

::
   stochastics = mctree.get_daughters( p )
   self.daughterlessCounter.assert_true( len(stochastics) == 0 )

That's all you need to do.  If a run generates 10 events **in a row** that contains InIce muons with no daughters, the run will stop and print the error message "Too many events with daughterless InIce muons."  Be sure to make this descriptive.  You're stopping a run and people are going to want to know why.

Customizing the Failure Message
-------------------------------
In the above example, say you wanted to add a little more information about why the run stopped.  Nothing's preventing you from modifying the failure message on-the-fly.

::
   stochastics = mctree.get_daughters( p )
   self.daughterlessCounter.assert_true( len(stochastics) == 0 )
   self.daughterlessCounter.failure_msg = "Gone %d frames without any daughters for the any InIce mouns." \
                                          % self.daughterlessCounter._counter




