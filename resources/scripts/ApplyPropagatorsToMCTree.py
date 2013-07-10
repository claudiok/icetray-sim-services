#!/usr/bin/env python

import math
from os.path import expandvars

from I3Tray import *
from icecube import icetray, dataclasses, dataio, phys_services, sim_services, simclasses, neutrino_generator
import sys

class HoboPropagatorServiceTracks(sim_services.I3PropagatorService):
    def __init__(self):
        sim_services.I3PropagatorService.__init__(self)
        self.random = None

    def SetRandomNumberGenerator(self, random):
        print("RANDOM!")
        self.random = random

    def Propagate(self, particle, frame):
        print("track!", particle.length)

        if "I3MMCTrackList" not in frame:
            trackList = simclasses.I3MMCTrackList()
        else:
            trackList = frame["I3MMCTrackList"]

        t = simclasses.I3MMCTrack()
        t.particle = particle
        trackList.append(t)

        if "I3MMCTrackList" in frame:
            del frame["I3MMCTrackList"]
        frame["I3MMCTrackList"] = trackList

        newP = dataclasses.I3Particle()

        return [newP, dataclasses.I3Particle()]


class HoboPropagatorServiceCascades(sim_services.I3PropagatorService):
    def __init__(self):
        sim_services.I3PropagatorService.__init__(self)
        self.random = None

    def SetRandomNumberGenerator(self, random):
        self.random = random

    def Propagate(self, particle, frame):
        return [dataclasses.I3Particle(), dataclasses.I3Particle()]


class HoboPropagatorServiceMonopole(sim_services.I3PropagatorService):
    def __init__(self):
        sim_services.I3PropagatorService.__init__(self)
        self.random = None

    def SetRandomNumberGenerator(self, random):
        self.random = random

    def Propagate(self, particle, frame):
        print("monopole!", particle.length)

        newP = dataclasses.I3Particle()
        newP.type = dataclasses.I3Particle.ParticleType.EMinus

        return [newP, dataclasses.I3Particle()]

if len(sys.argv) != 2: 
    print("usage: ApplyPropagatorsToMCTree.py output_file")
    sys.exit(-1)

tray = I3Tray()

# set up a random number generator
randomService = phys_services.I3SPRNGRandomService(
    seed = 12345,
    nstreams = 10000,
    streamnum = 1)

# re-use the same RNG for modules that need it on the context
tray.context['I3RandomService'] = randomService

tray.AddModule("I3InfiniteSource","streams",
               # Prefix=GCDFile,
               Stream=icetray.I3Frame.DAQ)

tray.AddModule("I3MCEventHeaderGenerator","gen_header",
               Year=2013,
               DAQTime=0,
               RunNumber=12345,
               EventID=1,
               IncrementEventID=True)

tray.AddModule("I3NeutrinoGenerator","neutrino",
    nevents = 1000,
    NeutrinoFlavor = "NuMu",
    zenithMin = 0.*I3Units.deg,
    zenithMax = 180.*I3Units.deg,
    azimuthMin = 0.*I3Units.deg,
    azimuthMax = 360.*I3Units.deg,
    EnergyMinLog = math.log10(10.*I3Units.GeV/I3Units.GeV),
    EnergyMaxLog = math.log10(1000.*I3Units.GeV/I3Units.GeV),

    GammaIndex = 2,

    # These settings are used in simprod
    injectionRadius = 1200.*I3Units.m,
    distanceExit = 1000.*I3Units.m,
    distanceEntrance = 1000.*I3Units.m,
    NoInEarthPropagation = False,
    AutomaticMuonFinalVolume = True,

    detectorDepth = 1950.*I3Units.m,
    TablesDir = expandvars("$I3_PORTS/share/anis-1.8.2"),
    SigmaFileCCNu = "/css/css_cc_nu.data",
    SigmaFileCCNuBar = "/css/css_cc_nubar.data",
    SigmaFileNCNu = "/css/css_nc_nu.data",
    SigmaFileNCNuBar = "/css/css_nc_nubar.data",
    )


muon_propagator = HoboPropagatorServiceTracks()
cascade_propagator = HoboPropagatorServiceCascades()
monopole_propagator = HoboPropagatorServiceMonopole()

propagatorDict = sim_services.I3ParticleTypePropagatorServiceMap({
# propagatorDict = {
    dataclasses.I3Particle.ParticleType.MuPlus   : muon_propagator,
    dataclasses.I3Particle.ParticleType.MuMinus  : muon_propagator,
    dataclasses.I3Particle.ParticleType.TauPlus  : muon_propagator,
    dataclasses.I3Particle.ParticleType.TauMinus : muon_propagator,
    dataclasses.I3Particle.ParticleType.Hadrons  : cascade_propagator,
    dataclasses.I3Particle.ParticleType.EPlus    : cascade_propagator,
    dataclasses.I3Particle.ParticleType.EMinus   : cascade_propagator,
    dataclasses.I3Particle.ParticleType.Monopole : monopole_propagator,
# }
})

tray.AddModule('I3PropagatorModule', 'propagate',
    InputMCTreeName = "I3MCTree",
    OutputMCTreeName = "I3MCTree_new",
    PropagatorServices = propagatorDict,
    RandomService = "I3RandomService"
    )

tray.AddModule('I3Writer', 'writer', Filename=sys.argv[1])
tray.AddModule('TrashCan', 'can')

tray.Execute()
tray.Finish()
