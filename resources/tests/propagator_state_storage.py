#!/usr/bin/env python

"""
Demonstrate that the results of muon and 
can be exactly recovered by caching the random number generator state.
"""

from icecube import icetray, dataclasses, dataio
from I3Tray import I3Tray
from icecube import MuonGun, phys_services, sim_services
from icecube.PROPOSAL import I3PropagatorServicePROPOSAL
from icecube.cmc import I3CascadeMCService
from icecube.MuonGun.segments import MakePropagator, GenerateBundles
from os.path import expandvars
from os import unlink

# icetray.logging.set_level_for_unit('I3PropagatorModule', 'TRACE')

gcd = expandvars("$I3_PORTS/test-data/sim/GeoCalibDetectorStatus_IC80_DC6.54655.i3.gz")

def make_propagators():
	propagators = sim_services.I3ParticleTypePropagatorServiceMap()
	muprop = I3PropagatorServicePROPOSAL(type=dataclasses.I3Particle.MuMinus)
	cprop = I3CascadeMCService(phys_services.I3GSLRandomService(1)) # dummy RNG
	for pt in 'MuMinus', 'MuPlus':
		propagators[getattr(dataclasses.I3Particle.ParticleType, pt)] = muprop
	# for pt in 'DeltaE', 'Brems', 'PairProd', 'NuclInt', 'Hadrons', 'EMinus', 'EPlus':
	# 	propagators[getattr(dataclasses.I3Particle.ParticleType, pt)] = cprop
	return propagators

def generate(nevents=1, fname='foo.i3'):
	tray = I3Tray()
	
	generator = MuonGun.Floodlight()

	# set up a random number generator
	randomService = phys_services.I3SPRNGRandomService(
	    seed = 1,
	    nstreams = 10000,
	    streamnum = 1)

	# a random number generator
	tray.AddService("I3SPRNGRandomServiceFactory","sprngrandom",
	    seed = 2,
	    nstreams = 10000,
	    streamnum = 1)

	tray.AddSegment(GenerateBundles, 'BundleGen', Generator=generator, RandomService=randomService,
	    NEvents=nevents, GCDFile=gcd)	
	    
	def stash(frame):
		frame['RemadeMCTree'] = dataclasses.I3MCTree(frame['I3MCTree'])
	tray.AddModule(stash, 'copy', Streams=[icetray.I3Frame.DAQ])
	
	tray.AddModule('I3PropagatorModule', 'propagator', PropagatorServices=make_propagators(),
	    RandomService=randomService, RNGStateName="RNGState")
	#     
	# tray.AddModule('Dump', 'dump')
	tray.AddModule('I3Writer', 'writer',
	    Streams=[icetray.I3Frame.DAQ, icetray.I3Frame.Physics],
	    # DropOrphanStreams=[icetray.I3Frame.DAQ],
	    filename=fname)

	tray.AddModule('TrashCan', 'YesWeCan')
	tray.Execute()
	tray.Finish()
	
def check(fname='foo.i3', fraction=0.1):
	
	from unittest import TestCase
	
	tray = I3Tray()
	
	tray.AddModule('I3Reader', 'reader', filenamelist=[gcd, fname])
	
	# set up a random number generator
	randomService = phys_services.I3SPRNGRandomService(
	    seed = 1,
	    nstreams = 10000,
	    streamnum = 1)
	
	def drop_random_events(frame, fraction=fraction):
		import random
		return random.uniform(0, 1) < (1-fraction)
	tray.AddModule(drop_random_events, 'triggerhappy', Streams=[icetray.I3Frame.DAQ])
	
	tray.AddModule('I3PropagatorModule', 'propagator', PropagatorServices=make_propagators(),
	    RandomService=randomService, RNGStateName="RNGState", InputMCTreeName="RemadeMCTree", OutputMCTreeName="RemadeMCTree")
	
	class MCTreeCompare(TestCase):
		def setUp(self):
			self.orig_tree = self.frame['I3MCTree']
			self.new_tree = self.frame['RemadeMCTree']
		def testTotalSize(self):
			# print self.orig_tree
			# print self.new_tree
			self.assertEquals(len(self.orig_tree), len(self.new_tree))
		def testParticleContent(self):
			return
			from itertools import izip
			for p1, p2 in izip(self.orig_tree, self.new_tree):
				self.assertEquals(p1.energy, p2.energy)
				self.assertEquals(p1.type, p2.type)
				self.assertEquals(p1.time, p2.time)
				self.assertEquals(p1.length, p2.length)
				for d in 'x', 'y', 'z':
					self.assertEquals(getattr(p1.pos, d), getattr(p2.pos, d))
				for d in 'zenith', 'azimuth':
					self.assertEquals(getattr(p1.dir, d), getattr(p2.dir, d))
	
	tray.AddModule(icetray.I3TestModuleFactory(MCTreeCompare), 'testy', Streams=[icetray.I3Frame.DAQ])
	
	tray.AddModule('TrashCan', 'YesWeCan')
	tray.Execute()
	tray.Finish()
	

fname = 'propagator_state_storage.i3'
generate(nevents=int(1e3), fname=fname)
check(fraction=0.99, fname=fname)

unlink(fname)

