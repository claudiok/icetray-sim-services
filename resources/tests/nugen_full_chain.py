#!/usr/bin/env python
#
# Simple NuGen example
# for Old NuGen setup (NuGen v2)
#
# For parameter options, see 
# http://wiki.icecube.wisc.edu/index.php/Neutrino_Generator/Setting_Parameters
#

from I3Tray import I3Tray
from I3Tray import I3Units

import os
from os.path import expandvars

from icecube import icetray
from icecube import dataclasses
from icecube import phys_services
from icecube import sim_services
from icecube import dataio
from icecube import neutrino_generator
from icecube.simprod.segments.PropagateMuons import PropagateMuons
from icecube.clsim import I3CLSimMakeHits 
#from icecube.simprod.detectors import DetectorSegment 
from icecube import vuvuzela
from icecube import DOMLauncher
from icecube import trigger_sim

import sys


tray = I3Tray()
ev_n=100

# for this example I set propmode as AUTODETECT
# to keep all input neutrinos. 
# if you want to simulate CC interaction inside Earth 
# so that some neutrinos will be absorbed,
# set nugen.nopropweight instead.
# AUTODETECT option takes into account of particle flavor,
# if a propagating particle is NuTau it switch off weighted propataion.
propmode = neutrino_generator.autodetect

flavorString = "NuMu"
earth = ["PREM_mmc"]
material = ["Standard"]
xsec = "csms"

# energy setting
gamma = "1"
emin = "2"
emax = "8"

# zenith setting
minzen = "0"
maxzen = "180"
zenithweightparam = "1"

# icecap model
# default is now SimpleIceCap. You need to set IceSheet explicitly 
# if you want to reproduce old simulation.
#icecapmodel = "SimpleIceCap"
icecapmodel = "IceSheet"

# injection type
# default is now Surface (old name : Cylinder), which is similar to MuonGun(more efficient).
# You need to set Circle explicitly if you want to reproduce old simulation.
injectionmode = "Circle"
injectionrad = 1200.*I3Units.m
distanceEntrance = 1200*I3Units.m
distanceExit = 1200*I3Units.m
cylinderparams = [injectionrad, distanceEntrance, distanceExit]

outi3filename = flavorString + ".i3"

random = phys_services.I3GSLRandomService(42)
tray.context['I3RandomService'] = random

from os.path import expandvars
gcdfile = os.path.expandvars("$I3_TESTDATA/sim/GeoCalibDetectorStatus_IC86.55380_corrected.i3.gz")
tray.AddModule("I3InfiniteSource", "source",
               prefix = gcdfile
               )

tray.AddModule("I3MCEventHeaderGenerator","ev")

#
# At least EarthModelService & Steering Service are required
#
tray.AddService("I3EarthModelServiceFactory", "EarthModelService",
                EarthModels = earth,
                MaterialModels = material,
                IceCapType = icecapmodel,
                DetectorDepth = 1948*I3Units.m,
                PathToDataFileDir = "")

#
# try new parameter setting:
# parameter set must be in string "Key:value"
# and value can not contain I3Units::m etc.
# Use legacy setting if you want to use I3Units.
#
tray.AddService("I3NuGSteeringFactory", "steering",
                EarthModelName = "EarthModelService",
                NEvents = ev_n,
                SimMode = "Full",
                VTXGenMode = "NuGen",
                InjectionMode = injectionmode,
                CylinderParams = cylinderparams
                )

tray.AddService("I3NuGInjectorFactory", "injector",
                RandomService = random,
                SteeringName = "steering",
                NuFlavor = flavorString,
                GammaIndex = float(gamma),
                EnergyMinLog = float(emin),
                EnergyMaxLog = float(emax),
                ZenithMin = float(minzen)*I3Units.degree,
                ZenithMax = float(maxzen)*I3Units.degree,
                ZenithWeightParam = float(zenithweightparam) 
               )

tray.AddService("I3NuGInteractionInfoFactory", "interaction",
                RandomService = random,
                SteeringName = "steering",
                CrossSectionModel = xsec
               )

tray.AddModule("I3NeutrinoGenerator","generator",
                RandomService = random,
                SteeringName = "steering",
                InjectorName = "injector",
                InteractionInfoName = "interaction",
                PropagationWeightMode = propmode 
              )
tray.AddSegment(PropagateMuons, "lprop", \
                    RandomService = random, \
                    SaveState = False, \
                    InputMCTreeName = "I3MCTree")

tray.AddSegment(I3CLSimMakeHits)

tray.AddSegment(vuvuzela.AddNoise,
                InputName = "MCPESeriesMap",
                OutputName = "I3MCPESeriesMap")

tray.AddModule("PMTResponseSimulator")

tray.AddModule("DOMLauncher", Output = "InIceRawData")

tray.AddSegment(trigger_sim.TriggerSim, gcd_file = dataio.I3File(gcdfile))

#FIXME : we should be able to add python functors too.
class stats :
    def __init__(self):
        self.tree_lengths = list()
        self.trigger_lengths = list()

    def __call__(self, frame):
        if "I3MCTree" in frame :
            tree = frame["I3MCTree"]
            print("len(tree) = ", len(tree))
        if "I3TriggerHierarchy" in frame :
            triggers = frame["I3TriggerHierarchy"]
            print("len(triggers) = ", len(triggers))

# FIXME : the default should be to run on every frame.
# keep the option to select a particlar type, but the
# default of just running on Physics frame blows.
#tray.AddModule(stats, streams = [icetray.I3Frame.DAQ])

class test(icetray.I3Module):
    def __init__(self, context):
        icetray.I3Module.__init__(self, context)
        self.n_frames = 0
        self.AddOutBox("OutBox")

    def DAQ(self, frame):
        self.n_frames += 1
        if "I3MCTree" not in frame :
            sys.exit(1)
        if len(frame["I3MCTree"]) == 0 :
            sys.exit(1)
        if "InIceRawData" not in frame :
            sys.exit(1)

    def Configure(self) :
        pass

    def Finish(self):
        # in this particular sample only 16 frames are triggered.
        # this hard-coded value is only temporary
        if self.n_frames == 0, :
            print "No events triggered."
            sys.exit(1)

tray.AddModule(test)

tray.AddModule("I3Writer","writer")(
    ("filename", outi3filename))

tray.AddModule("TrashCan", "the can")

tray.Execute()
tray.Finish()

