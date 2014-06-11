
import numpy
import copy
from enum import enum

try:
	from icecube.icetray import I3Units
except ImportError:
	# hobo versions of IceTray enumerations
	class I3Units:
		GeV = 1.
		TeV = 1e3
		PeV = 1e6
		EeV = 1e9
	
		m   = 1.
	
		degree = numpy.pi/180.
		deg = degree

class ParticleType(enum):
	PPlus       =   14
	He4Nucleus  =  402
	N14Nucleus  = 1407
	Al27Nucleus = 2713
	Fe56Nucleus = 5626
	NuE         =   66
	NuEBar      =   67
	NuMu        =   68
	NuMuBar     =   69
	NuTau       =  133
	NuTauBar    =  134

class PDGCode(enum):
	PPlus       =       2212
	He4Nucleus  = 1000020040
	N14Nucleus  = 1000070140
	Al27Nucleus = 1000130270
	Fe56Nucleus = 1000260560
	NuE         =         12
	NuEBar      =        -12
	NuMu        =         14
	NuMuBar     =        -14
	NuTau       =         16
	NuTauBar    =        -16

PDGCode.from_corsika = classmethod(lambda cls, i: getattr(cls, ParticleType.values[i].name))

class GenerationProbability(object):
	"""
	A probability distribution from which MC events are drawn.
	
	Generation probabilities may be multiplied by an integer to express
	a joint generation probability for multiple identical simulation runs
	or added together to express a set of distinct runs.
	"""
	def __init__(self, emin, emax, nevents=1, area=1, particle_type=None):
		"""
		:param emin: minimum energy for which this probability is non-zero
		:param emax: maximum energy for which this probability is non-zero
		:param nevents: number of generated events
		:param area: integrated area-times-solid-angle of the sampling surface
		:param particle_type: particle type for which this probability is valid. if None,
		                      then it is valid for all particle types.
		"""
		self.emin = emin
		self.emax = emax
		self.nevents = nevents
		self.area = area
		self.particle_type = particle_type
	
	def to_PDG(self):
		"Translate to a form that understands PDG codes instead of CORSIKA"
		if self.particle_type is not None and getattr(PDGCode, self.particle_type.name) == self.particle_type:
			return self
		new = copy.copy(self)
		if new.particle_type is not None:
			new.particle_type = PDGCode.from_corsika(new.particle_type)
		return new
	
	def __imul__(self, factor):
		self.nevents *= factor
		return self
	
	def __mul__(self, factor):
		t = copy.copy(self)
		t *= factor
		return t
	
	def __rmul__(self, factor):
		return self*factor
	
	def __idiv__(self, factor):
		self.nevents /= factor
		return self
		
	def __div__(self, factor):
		t = copy.copy(self)
		t /= factor
		return t
		
	def __iadd__(self, other):
		if isinstance(other, type(self)):
			if self.is_compatible(other):
				self.nevents += other.nevents
				return self
			else:
				return GenerationProbabilityCollection([self, other])
		else:
			raise TypeError("Can't add a %s to this %s" % (type(other).__name__, type(self).__name__))
	
	def __add__(self, other):
		if isinstance(other, type(self)):
			if self.is_compatible(other):
				t = copy.copy(self)
				t += other
				return t
			else:
				return GenerationProbabilityCollection([self, other])
		else:
			return GenerationProbabilityCollection([self, other])
	
	def __call__(self, E, particle_type=None):
		"""
		Calculate the generated fluence of particles in this simulation scheme
		
		:param E: the energy of particle in GeV
		:param particle_type: the particle type for which to return a fluence
		:returns: a fluence in units of :math:`m^{-2} sr^{-1} GeV^{-1}`
		"""
		E = numpy.asarray(E)
		if numpy.ndim(E) == 0:
			if (E>=self.emin and E<self.emax and (particle_type is None or particle_type == self.particle_type)):
				return self.nevents*self.generation_probability(E)/self.area
			else:
				return 0
		else:
			mask = (E>=self.emin)&(E<self.emax)
			if particle_type is not None:
				mask &= (particle_type==self.particle_type)
			return numpy.where(mask, self.nevents*self.generation_probability(E)/self.area, 0)
	
	def is_compatible(self, other):
		raise NotImplementedError("should be overridden")
	
	def __eq__(self, other):
		return self.nevents == other.nevents and self.is_compatible(other)
	
	def __ne__(self, other):
		return not (self == other)
	
	def generation_probability(self, E):
		raise NotImplementedError("should be overridden")

class Null(object):
	"""
	An identity object, useful as a starting point for accumulators, e.g.::
		total = Null()
		for i in xrange(10):
			total += SomeClassThatImplementsAddition(i)
	"""
	def __add__(self, other):
		return other
	def __radd__(self, other):
		return other
	def __iadd__(self, other):
		return other
	def __mul__(self, f):
		return self
	def __rmul__(self, f):
		return self
	def __imul__(self, f):
		return self
	def __eq__(self, other):
		return isinstance(other, Null) or 0 == other
	def __ne__(self, other):
		return not (self == other)

class GenerationProbabilityCollection(object):
	"""
	A collection of generation spectra, possibly for different particle types.
	"""
	def __init__(self, spectra):
		"""
		:param spectra: a collection of GenerationProbabilities.
		"""
		from collections import defaultdict
		self.spectra = defaultdict(list)
		for dist in spectra:
			self.spectra[dist.particle_type].append(dist)
	
	def to_PDG(self):
		spectra = []
		for v in self.spectra.values():
			spectra += [s.to_PDG() for s in v]
		return self.__class__(spectra)
		
	def __call__(self, E, particle_type=None):
		if particle_type is None:
			return sum([prob(E) for spectra in self.spectra.itervalues() for prob in spectra])
		else:
			if numpy.ndim(particle_type) == 0:
				return sum([prob(E) for prob in self.spectra[particle_type]])
			else:
				E = numpy.asarray(E)
				count = numpy.zeros(E.shape)
				for ptype in numpy.unique(particle_type):
					mask = particle_type==ptype
					if numpy.any(mask):
						Em = E[mask]
						count[mask] += sum([prob(Em) for prob in self.spectra[ptype]])
			return count
	
	def __imul__(self, factor):
		for spectra in self.spectra.itervalues():
			for prob in spectra:
				prob *= factor
		return self
	
	def __idiv__(self, factor):
		self *= (1./factor)
		return self
	
	def __mul__(self, factor):
		clone = copy.deepcopy(self)
		clone *= factor
		return clone
	
	def __div__(self, factor):
		return self*(1./factor)
	
	def __rmul__(self, factor):
		return self*factor
	
	def __iadd__(self, other):
		if isinstance(other, type(self)):
			for pt, ospectra in other.spectra.iteritems():
				for ospec in ospectra:
					for spec in self.spectra[pt]:
						if spec.is_compatible(ospec):
							spec += ospec
							break
					else:
						self.spectra[pt].append(ospec)
			return self
		else:
			for spec in self.spectra[other.particle_type]:
				if spec.is_compatible(other):
					spec += other
					break
			else:
				self.spectra[other.particle_type].append(other)
			return self
	
	def __add__(self, other):
		t = copy.copy(self)
		t += other
		return t
	
	def __eq__(self, other):
		# must handle the same set of particle types
		if set(self.spectra.keys()) != set(other.spectra.keys()):
			return False
		for k in self.spectra:
			s1 = self.spectra[k]
			s2 = other.spectra[k]
			# must have the same number of unique spectra
			if len(s1) != len(s2):
				return False
			# exactly one match for each spectrum
			for p1 in s1:
				if sum(p1 == p2 for p2 in s2) != 1:
					return False
		return True
	
	def __ne__(self, other):
		return not self == other
	
class PowerLaw(GenerationProbability):
	"""
	Power-law spectra are easy.
	"""
	def __init__(self, eslope, *args, **kwargs):
		"""
		:param eslope: index of the power law :math:`E^{-\gamma}`
		"""
		super(PowerLaw, self).__init__(*args, **kwargs)
		self.eslope = eslope
		self.gen_norm = self.norm(self.emin, self.emax, self.eslope)
	
	@staticmethod
	def norm(emin, emax, eslope):
		if eslope < -1:
			g = eslope+1
			return (emax**g - emin**g)/g
		else:
			return numpy.log(emax/emin)
	
	def __repr__(self):
		return "PowerLaw(%.2f, emin=%.2e, emax=%.2e, nevents=%.2e)" % (self.eslope, self.emin, self.emax, self.nevents)
	
	def generation_probability(self, E):
		return E**(self.eslope)/self.gen_norm
	
	def invert(self, p):
		"""
		Return CDF^{-1}(p)
		"""
		return (p*(self.emax**(self.eslope+1) - self.emin**(self.eslope+1)) + self.emin**(self.eslope+1))**(1./(self.eslope+1))
	
	def is_compatible(self, other):
		if isinstance(other, type(self)):
			return self.emin == other.emin and self.emax == other.emax and self.eslope == other.eslope and self.particle_type == other.particle_type
		else:
			return False

class HoerandelComponent(GenerationProbability):
	"""
	A power law with a rigidity-dependent knee.
	"""
	def __init__(self, z, eslope, *args, **kwargs):
		"""
		:param z: charge of the p
		
		"""
		super(HoerandelComponent, self).__init__(*args, **kwargs)
		self.z = z
		self.gamma = eslope
		self.gen_norm = self.fluxsum(self.emin, self.emax, z, self.gamma)
	
	def generation_probability(self, E):
		return self.fluxdiff(E, self.z, self.gamma)/self.gen_norm
	
	def is_compatible(self, other):
		if isinstance(other, type(self)):
			return self.emin == other.emin and self.emax == other.emax and self.gamma == other.gamma and self.particle_type == other.particle_type and self.z == other.z
		else:
			return False
	
	@staticmethod	
	def fluxdiff(e, z, gamma, delta_gamma=2.1, eps_cutoff=1.9, E_knee=4.49*I3Units.PeV):
		"""
		Differential (unnormalized) Hoerandel flux
		"""
		return e**(-gamma)*(1+(e/(E_knee*z))**eps_cutoff)**(-delta_gamma/eps_cutoff)
	
	@staticmethod
	def fluxsum(emin, emax, z, gamma, delta_gamma=2.1, eps_cutoff=1.9, E_knee=4.49*I3Units.PeV):
		"""
		Integral Hoerandel flux
		"""
		# the Gauss hypergeometric function. whee!
		from scipy.special import hyp2f1
		antideriv = lambda e: ((e**(1-gamma))/(1-gamma))*hyp2f1(delta_gamma/eps_cutoff, (1-gamma)/eps_cutoff, (1-gamma)/eps_cutoff+1, -(e/(E_knee*z))**eps_cutoff)
		return antideriv(emax) - antideriv(emin)

def FiveComponent(nevents, emin, emax, normalization=[10., 5., 3., 2., 1.], gamma=[-2.]*5, spric=True, height=1600, radius=800):
	"""
	Special case: 5-component dCORSIKA
	
	:param normalization: relative normalizations of the 5 components
	:param gamma: power law index for each component
	:param spric: make lower energy proportional to primary mass
	:param height: full height of the target cylinder in meters
	:param radius: radius of the target cylinder in meters
	
	.. note:: The return value of the GenerationProbability will be in units of :math:`GeV^{-1} sr^{-1} m^{-2}`
	"""
	pt = [getattr(ParticleType, p) for p in ('PPlus', 'He4Nucleus', 'N14Nucleus', 'Al27Nucleus', 'Fe56Nucleus')]
	if spric:
		masses = [[1, p/100][p>100] for p in pt]
	else:
		masses = [1.]*5
	# DCORSIKA does this integral in TeV, so we have to do so as well.
	fluxsums = numpy.array([n*PowerLaw.norm(emin*m/I3Units.TeV, emax/I3Units.TeV, g) for m, g, n in zip(masses, gamma, normalization)])
	nshower = nevents*fluxsums/fluxsums.sum()
	area = numpy.pi**2*radius*(radius+height)
	return GenerationProbabilityCollection([PowerLaw(g, emin*m, emax, nevents=n, area=area, particle_type=p) for m, g, n, p in zip(masses, gamma, nshower, pt)])

def Hoerandel(nevents, emin, emax, dslope=0, height=1600, radius=800):
	"""
	Special case: in RANPRI=2 mode, dCORSIKA produces a "natural" spectrum after
	the parameterization of Hoerandel. In order to combine this with 5-component
	simulation we use only H, He, N, Al, and Fe, ignoring all other
	components.
	
	:param dslope: multiply all fluxes by E^dslope
	:param height: full height of the target cylinder in meters
	:param radius: radius of the target cylinder in meters
	
	.. note:: The return value of the GenerationProbability will be in units of :math:`GeV^{-1} sr^{-1} m^{-2}`
	"""
	# Ripped from dCORSIKA source (with RANPRI=2)
	mass_number = numpy.round([1.00797, 4.0026, 6.939, 9.0122, 10.811, 12.0112, 14.0067, 15.9994, 18.9984, 20.183, 22.9898, 24.312, 26.9815, 28.086, 30.984, 32.064, 35.453, 39.948, 39.102, 40.08, 44.956, 47.9, 50.942, 51.996, 54.938, 55.847])
	fluxes = numpy.array([0.0873, 0.0571, 0.00208, 0.000474, 0.000895, 0.0106, 0.00235, 0.0157, 0.000328, 0.0046, 0.000754, 0.00801, 0.00115, 0.00796, 0.00027, 0.00229, 0.000294, 0.000836, 0.000536, 0.00147, 0.000304, 0.00113, 0.000631, 0.00136, 0.00135, 0.0204])
	gammas = numpy.array([2.71, 2.64, 2.54, 2.75, 2.95, 2.66, 2.72, 2.68, 2.69, 2.64, 2.66, 2.64, 2.66, 2.75, 2.69, 2.55, 2.68, 2.64, 2.65, 2.7, 2.64, 2.61, 2.63, 2.67, 2.46, 2.59])
	
	# Integrate each power law (without a knee) to get the total flux
	# This calculation is done in TeV (it matters!)
	emin = emin*mass_number/I3Units.TeV
	# Generation spectrum includes the slope change
	gamma = gammas + dslope
	fluxsums = fluxes*(emin**(1-gamma))/(gamma-1)
	nshower = nevents*fluxsums/fluxsums.sum()
	
	components = []
	area = numpy.pi**2*radius*(radius+height)
	for pt in [getattr(ParticleType, p) for p in ('PPlus', 'He4Nucleus', 'N14Nucleus', 'Al27Nucleus', 'Fe56Nucleus')]:
		if pt < 100:
			z = 1
		else:
			z = pt % 100
		components.append(HoerandelComponent(z, gamma[z-1], emin[z-1]*I3Units.TeV, emax,
		    nevents=nshower[z-1], area=area, particle_type=pt))
	return GenerationProbabilityCollection(components)

def NeutrinoGenerator(NEvents, EnergyMinLog, EnergyMaxLog, GammaIndex, NeutrinoFlavor,
    InjectionRadius=1200*I3Units.m, ZenithMin=0, ZenithMax=numpy.pi, AzimuthMin=0, AzimuthMax=2*numpy.pi,**kwargs):
	"""
	Construct a GenerationProbability appropriate for NeutrinoGenerator simulation. The arguments
	have the same meaning as those to the I3NeutrinoGenerator.
	
	.. warning:: The return value of the GenerationProbability will be in units of :math:`GeV^{-1} sr^{-1} m^{-2}`
	             rather than :math:`cm^{-2}`. Make sure that you use fluxes in the appropriate units!
	"""
	probs = []
	for p in (NeutrinoFlavor, NeutrinoFlavor+'Bar'):
		pt = getattr(ParticleType, p)
		# NeutrinoGenerator samples on a cylindrical surface whose axis
		# passes through the origin and is parallel to the incoming
		# neutrino.
		area = (numpy.cos(ZenithMin)-numpy.cos(ZenithMax))*(AzimuthMax-AzimuthMin)*numpy.pi*InjectionRadius**2
		# Half of the flux comes as neutrinos, half as anti-neutrinos
		probs.append(PowerLaw(-GammaIndex, 10**EnergyMinLog, 10**EnergyMaxLog, (NEvents/2.), area, particle_type=pt))
	return GenerationProbabilityCollection(probs)

def _import_mysql():
	"Import the flavor of the month"
	import importlib
	for impl in 'MySQLdb', 'mysql.connector', 'pymysql':
		try:
			mysql = importlib.import_module(impl)
			return mysql
		except ImportError:
			pass

	raise ImportError('No MySQL bindings found!')

def from_simprod(dataset_id, use_muongun=False):
	"""
	Extreme laziness: parse weighting scheme out of the simulation production database
	
	:param dataset_id: the number of the SimProd dataset
	:returns: a tuple whose first element is the number of files that finished processing
	          successfully, and the second
	"""
	import re
	mysql = _import_mysql()
	
	db = mysql.connect(host='dbs4.icecube.wisc.edu', user='i3simprod-ro', passwd='Twed9~Dret', db='i3simprod')
	cursor = db.cursor()
	
	if isinstance(dataset_id, str):
		raise UnboundLocalError
	cursor.execute("SELECT COUNT(*) FROM dataset WHERE dataset_id=%s", (dataset_id,))
	if cursor.fetchone()[0] == 0:
		raise UnboundLocalError
	
	# In case this is a post-processed set, chase the chain back until we hit the real generated set
	while True:
		cursor.execute("SELECT description FROM dataset WHERE dataset_id=%s", (dataset_id,))
		match = re.match(r'.*(from|of) dataset (\d{4})', cursor.fetchone()[0], re.IGNORECASE)
		if match:
			dataset_id = int(match.group(2))
		else:
			break
	
	
	typemap=dict(string=str, int=int, double=float, bool=bool)
	
	cursor.execute("SELECT category, jobs_completed FROM dataset JOIN simcat ON dataset.simcat_id=simcat.simcat_id and dataset.dataset_id=%s", (dataset_id,))
	row = cursor.fetchone()
	category, nfiles = row[0], int(row[1])
	
	cursor.execute("SELECT name, type, value FROM steering_parameter WHERE dataset_id=%s", (dataset_id,))
	steering = {}
	for name, typus, value in cursor.fetchall():
		try:
			steering[name] = typemap[typus](value)
		except ValueError:
			pass
	
	if category == 'Test':
		if steering['mctype'] == 'corsika':
			category = 'CORSIKA-in-ice'
		elif steering['mctype'].startswith('nugen'):
			category = 'neutrino-generator'
	
	def get_tray_parameter(dataset_id, key):
		cursor.execute("SELECT cparameter_id, type, value FROM cparameter WHERE dataset_id=%s AND name=%s", (dataset_id, key))
		row = cursor.fetchone()
		if not row:
			return None
		if row[1] in typemap:
			return typemap[row[1]](row[2])
		else:
			cursor.execute("SELECT value FROM carray_element WHERE cparameter_id=%s", (row[0],))
			return [float(v[0]) for v in cursor.fetchall()]
	
	if category == 'neutrino-generator':
		generator = NeutrinoGenerator(NEvents=steering['nevents'],
		    EnergyMinLog   =steering['NUGEN::elogmin'],
		    EnergyMaxLog   =steering['NUGEN::elogmax'],
		    GammaIndex     =steering['NUGEN::gamma'],
		    NeutrinoFlavor =steering['NUGEN::flavor'],
		    InjectionRadius=steering['NUGEN::injectionradius']*I3Units.m,
		    ZenithMin      =steering['NUGEN::zenithmin']*I3Units.deg,
		    ZenithMax      =steering['NUGEN::zenithmax']*I3Units.deg)
	elif category == 'CORSIKA-in-ice':
		if steering['composition'].startswith('5-component'):
			gamma = get_tray_parameter(dataset_id, "pgam")
			if gamma is None:
				gamma = [-2]*5
			else:
				gamma = [-v for v in gamma]
			norm = get_tray_parameter(dataset_id, "pnorm")
			if norm is None:
				norm = [10., 5., 3., 2., 1.]
			if get_tray_parameter(dataset_id, 'CutoffType') == "EnergyPerParticle":
				spric = False
			else:
				spric = True
			if use_muongun:
				from icecube import MuonGun
				nevents = steering['CORSIKA::showers']
				print gamma
				# FIXME: more careful checking of the parameters
				if gamma[0] == -2.0:
					model = 'Standard5Comp'
				elif gamma[0] == -2.6:
					model = 'CascadeOptimized5Comp'
				elif gamma[0] == -2.65:
					length = steering['CORSIKA::length']*I3Units.m
					radius = steering['CORSIKA::radius']*I3Units.m
					area = numpy.pi**2*radius*(radius+length)
					areanorm = 0.131475115*area
					model = 'Hoerandel5'
					nevents /= areanorm
				else:
					raise ValueError("Unknown CORSIKA configuration!")
				generator = nevents*MuonGun.corsika_genprob(model)
			else:
				generator = FiveComponent(steering['CORSIKA::showers'],
				    emin=steering['CORSIKA::eprimarymin']*I3Units.GeV,
				    emax=steering['CORSIKA::eprimarymax']*I3Units.GeV,
				    normalization=norm, gamma=gamma, spric=spric,
				    height=steering['CORSIKA::length']*I3Units.m,
				    radius=steering['CORSIKA::radius']*I3Units.m)
		elif steering['composition'].startswith('polygonato') or steering['composition'].startswith('Hoerandel'):
			if use_muongun:
				from icecube import MuonGun
				length = steering['CORSIKA::length']*I3Units.m
				radius = steering['CORSIKA::radius']*I3Units.m
				area = numpy.pi**2*radius*(radius+length)
				areanorm = 0.131475115*area
				generator = (steering['CORSIKA::showers']/areanorm)*MuonGun.corsika_genprob('Hoerandel5')
			else:
				generator = Hoerandel(steering['CORSIKA::showers'],
				    emin=steering['CORSIKA::eprimarymin']*I3Units.GeV,
				    emax=steering['CORSIKA::eprimarymax']*I3Units.GeV,
				    dslope=steering['CORSIKA::dslope'],
				    height=steering['CORSIKA::length']*I3Units.m,
				    radius=steering['CORSIKA::radius']*I3Units.m)
	
	return nfiles, generator

class EnergyWeight(object):
	def __init__(self, target_flux, generation_spectrum):
		self.target_flux = target_flux
		self.generation_spectrum = generation_spectrum
	def __call__(self, E, zenith):
		return self.target_flux(E)/self.generation_spectrum(E, zenith)

