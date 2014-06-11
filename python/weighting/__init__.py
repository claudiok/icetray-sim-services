
__all__ = ["WeightCalculator"]

import numpy

def get_weighted_primary(frame):
	"""
	Weighted CORSIKA simulation can have coincidences mixed in that should
	not be used to calculate weights, as they were chosen at natural
	frequency. Find the primary that was chosen from a biased spectrum.
	"""
	from weighting import ParticleType
	
	if not 'I3MCTree' in frame or (len(frame['I3MCTree'].primaries) == 0):
		return
	primaries = frame['I3MCTree'].primaries
	if len(primaries) == 1:
		idx = 0
	if 'I3MCWeightDict' in frame:
		idx = [i for i in range(len(primaries)) if primaries[i].is_neutrino][0]
	elif 'CorsikaWeightMap' in frame:
		wmap = frame['CorsikaWeightMap']
		# Only filter by particle type if we're still using CORSIKA-style
		# codes. This is a horrendous hack that will have to be revisited
		# once PDG-coded simulation becomes more common.
		if dataclasses.I3Particle.PPlus == ParticleType.PPlus:
			if 'PrimaryType' in wmap:
				primaries = [p for p in primaries if p.type==wmap['PrimaryType']]
			elif 'ParticleType' in wmap:
				primaries = [p for p in primaries if p.type==wmap['ParticleType']]
		if len(primaries) == 0:
			return
		elif len(primaries) == 1:
			idx = 0
		elif 'PrimarySpectralIndex' in wmap:
			prim_e = wmap['Weight']**(-1./wmap['PrimarySpectralIndex'])
			idx = int(numpy.argmin([abs(p.energy-prim_e) for p in primaries]))
		else:
			idx = 0
	
	frame['MCPrimary'] = primaries[idx]


from icecube import icetray, dataclasses

class WeightCalculator(icetray.I3ConditionalModule):
	def __init__(self, context):
		from fluxes import GaisserH3a
		super(WeightCalculator, self).__init__(context)
		self.AddOutBox("OutBox")
		self.AddParameter("Dataset", "Number of Simprod dataset, or a pre-constructed GenerationProbabilityCollection", None)
		self.AddParameter("Flux", "Primary CR flux model", GaisserH3a())
	
	def Configure(self):
		from weighting import from_simprod, PDGCode, GenerationProbabilityCollection
		dataset = self.GetParameter("Dataset")
		if dataset is None:
			icetray.logging.log_fatal("Unknown dataset ID %s" % dataset)
		elif isinstance(dataset, GenerationProbabilityCollection):
			self.norm = dataset
		else:
			self.norm = from_simprod(dataset)[1]
		self.flux = self.GetParameter("Flux")
		# detect which side of the CORSIKA->PDG code switch we're on
		if dataclasses.I3Particle.PPlus == PDGCode.PPlus:
			self.norm = self.norm.to_PDG()
			self.flux = self.flux.to_PDG()
	
	def DAQ(self, frame):
		if not 'MCPrimary' in frame:
			get_weighted_primary(frame)
		if not 'MCPrimary' in frame:
			self.PushFrame(frame)
			return
		primary = frame['MCPrimary']
		
		norm = self.norm(primary.energy, primary.type)
		if norm == 0:
			weight = 0
		else:
			weight = self.flux(primary.energy, primary.type)/norm
		frame[self.name] = dataclasses.I3Double(weight)
		
		self.PushFrame(frame)
