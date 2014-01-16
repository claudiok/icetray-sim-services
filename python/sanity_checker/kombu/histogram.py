import copy
import numpy as n
import pylab as p
import matplotlib as mpl
import icecube.sim_services.sanity_checker.kombu.histfuncs
from icecube.icetray.i3logging import log_warn
from icecube.icetray.i3logging import log_error
from icecube.sim_services.sanity_checker.kombu.odict import OrderedDict
from infobox import InfoBox 

class LegendProxy(object):
    """
    Provide proxies to draw legend entries for unsupported artist types.
    """
    def __init__(self, ax):
        self.ax = ax
        # replace Axes.legend() with a call to ourselves
        ax._legend = ax.legend
        ax.legend = self.__call__
        self.artists = []
        self.labels = []
    
    def _validate(self, **kwargs):
        if not 'label' in kwargs:
            return False
        if kwargs['label'] is None or kwargs['label'].startswith('__nolabel'):
            return False
        return True
    
    def add_line(self, **kwargs):
        if not self._validate(**kwargs):
            return
        line = mpl.lines.Line2D([0,0], [1,1],
                                linewidth=kwargs.get('linewidth', 1),
                                color=kwargs.get('color', None),
                                linestyle=kwargs.get('ls', None))
        self.artists.append(line)
        self.labels.append(kwargs.get('label', None))
    
    def add_fill(self, **kwargs):
        if not self._validate(**kwargs):
            return
        rect = mpl.patches.Rectangle((0, 0), 1, 1,
                                     color=kwargs.get('color', None),
                                     alpha=kwargs.get('alpha', 1.))
        self.artists.append(rect)
        self.labels.append(kwargs.get('label', None))
    
    def add_scatter(self, **kwargs):
        if not self._validate(**kwargs):
            return
        if not 'color' in kwargs:
            colors = None
        else:
            colors = mpl.colors.colorConverter.to_rgba_array(kwargs['color'], 1.0)
        artist = mpl.collections.AsteriskPolygonCollection(4, 0, [20., 20.],
                                                           facecolors=colors,
                                                           edgecolors=colors,
                                                           offsets=[[0,0], [1,1]])
        self.artists.append(artist)
        self.labels.append(kwargs.get('label', None))

    def __call__(self, **kwargs):
        artists, labels = self.ax.get_legend_handles_labels()
        for a, l in zip(self.artists, self.labels):
            if l is not None and not l.startswith('__nolabel'):
                artists.append(a)
                labels.append(l)
        if len(artists) > 0:
            return self.ax._legend(artists, labels, **kwargs)
        else:
            return self.ax._legend(**kwargs)

class histogram_statistics(object):
    """
        Holds counters and methods to calculate statistics. It uses a dictionary
        cache for faster access and provides access to its values via properties.
    """

    def __init__(self, histogram):
        self.values = dict()
        self.histogram = histogram # keep a reference to the histogram
        self.clear()

    def clear(self):
        " set all counters to zero "

        zerovalue = 0 if self.histogram.ndim == 1 else (0,) * self.histogram.ndim
        self.values["nentries"]   = 0   # count entries in this histogram
        self.values["nans"]       = 0   # count nan entries
        self.values["nans_wgt"]   = 0.  # sum weights of nan entries
        self.values["nans_sqwgt"] = 0.  # sum squared weights of nan entries
        self.values["mean"]       = zerovalue
        self.values["var"]        = zerovalue
        self.values["std"]        = zerovalue
        self.values["median"]     = zerovalue
        self.values["ske"]        = zerovalue
        self.values["kur"]        = zerovalue
        self.values["exc"]        = zerovalue
        self.values["underflow"]  = zerovalue
        self.values["overflow"]   = zerovalue 

    def __get_stats(self, varname):
        """
            frontend for self.values
            triggers recalculation of statistics if new data has been filled in 
        """
        if self.histogram._h_newdataavailable:
            self.calc_stats()
        return self.values[varname]

    # read only properties
    weightsum  = property(lambda self: self.histogram._h_bincontent.sum() + self.__get_stats("nans_wgt"))
    neffective = property(lambda self :  ( (self.histogram._h_bincontent.sum() + self.__get_stats("nans_wgt"))**2 ) \
                                           / ( self.histogram._h_squaredweights.sum() + self.__get_stats("nans_sqwgt") ))
    nentries   = property(lambda self : self.__get_stats("nentries") )
    mean       = property(lambda self : self.__get_stats("mean"), doc="sample mean" )
    var        = property(lambda self : self.__get_stats("var"), doc="(biased) sample variance" )
    std        = property(lambda self : self.__get_stats("std"), doc="(biased) sqrt (sample variance)"  )
    median     = property(lambda self : self.__get_stats("median"), doc="median" )
    skewness   = property(lambda self : self.__get_stats("ske"), doc="skewness")
    kurtosis   = property(lambda self : self.__get_stats("kur"), doc="kurtosis")
    excess     = property(lambda self : self.__get_stats("exc"), doc="excess")
    underflow  = property(lambda self : self.__get_stats("underflow"), doc="underflow" )
    overflow   = property(lambda self : self.__get_stats("overflow"), doc="overflow" )
    nans       = property(lambda self : self.__get_stats("nans"), doc="nan entries" )
    nans_wgt   = property(lambda self : self.__get_stats("nans_wgt"), doc="sum of weights of nan entries" )
    nans_sqwgt = property(lambda self : self.__get_stats("nans_sqwgt"), doc="sum of weights^2 of nan entries" )


    def calc_stats(self):
        """
            calculate statistics and set _h_newdataavailable to false
        """
        if self.histogram.ndim == 1:
            # for these calculations consider only entries in the bin range (ignore under-,overflow and nan bins)
            wsum = self.histogram.bincontent.sum()
            mean = (self.histogram.bincontent * self.histogram.bincenters).sum() / wsum
            meansquared = (self.histogram.bincontent * self.histogram.bincenters**2).sum() / wsum
            var = meansquared - mean**2
            std =  n.sqrt( var )
            medianindex = (self.histogram.bincontent.cumsum() / wsum).searchsorted(0.5, side="right")
            median =  self.histogram.bincenters[medianindex]
            meancubed = (self.histogram.bincontent * self.histogram.bincenters**3).sum() / wsum
            skewness = (meancubed - 3*mean*var - mean**3)/ (var ** (3./2.))
            meanpow4 = (self.histogram.bincontent * self.histogram.bincenters**4).sum() / wsum
            kurtosis = (meanpow4 - 4*mean*meancubed+ 6*(mean**2)*meansquared - 4*(mean**4) + mean**4) / (var ** 2)
            excess = kurtosis - 3 
            self.values["mean"] = mean
            self.values["var"]  = var
            self.values["std"]  = std
            self.values["median"]  = median
            self.values["ske"]  = skewness
            self.values["kur"]  = kurtosis
            self.values["exc"]  = excess
            self.values["underflow"] = self.histogram._h_bincontent[0]
            self.values["overflow"]  = self.histogram._h_bincontent[-1]
        else:
            means, vars, stds, medians,uflows, oflows   = [],[],[],[],[],[]

            for dim in xrange(self.histogram.ndim):
                tmp = histfuncs.project_bincontent(self.histogram, dim)
                wsum = tmp.sum() # == self.histogram.bincontent.sum() == sum of weights
                mean        = (tmp * self.histogram._h_bincenters[dim]   ).sum() / wsum
                meansquared = (tmp * self.histogram._h_bincenters[dim]**2).sum() / wsum
                var = meansquared - mean**2 
                medianindex = (tmp.cumsum() / wsum).searchsorted(0.5, side="right")
                median =  self.histogram._h_bincenters[dim][medianindex] 
                
                means.append(mean)
                vars.append(var)
                stds.append( n.sqrt(var) )
                medians.append( median)
                # FIXME find a good representation for these
                uflows.append( n.nan )
                oflows.append( n.nan )
            
            self.values["mean"]   = tuple(means)
            self.values["var"]    = tuple(vars)
            self.values["std"]    = tuple(stds)
            self.values["median"] = tuple(medians)
            self.values["underflow"] = tuple(uflows)
            self.values["overflow"] = tuple(oflows)

        self.histogram._h_newdataavailable = False   # works only if stats is the only listener

class histogram(object):
    """
        ndimensional histogram. essentialy a wrapper around numpy.histogramdd + extra functionality.
        This is the generic base class for :class:`~histogram.hist1d` and :class:`~histogram.hist2d`
    """

    _h_binwidths  = property(lambda self : [ (edges[2:-1] - edges[1:-2]) for edges in self._h_binedges],
                             doc="a list of arrays containing the widths of all bins")
    _h_bincenters = property(lambda self : [ 0.5*(edges[2:-1] + edges[1:-2]) for edges in self._h_binedges],
                             doc="a list of arrays containing the centers of all bins")
    
    def __init__(self, ndim, binedges, bincontent=None, squaredweights=None, labels=None, title=None):
        """
            Constructor for an empty n-dimensional histogram.
            **Parameters:**
                *ndim* : int
                    number of dimensions of this histogram
                *binedges* : tuple 
                    tuple containing ndim arrays that hold the binedges
                *labels* : tuple
                    tuple of ndim strings which describe the axes of the histogram
                *title* : str
                    a string describing the histogrammed data
        """

        assert len(binedges) == ndim
        self.ndim = ndim
       
        # lists containing for every dimension the binedges, -centers and -widths
        self._h_binedges   = []

        # these will be n-dimensional  datacubes holding the bincontent and the squared weights
        self._h_bincontent = None
        self._h_squaredweights = None

        # each histogram holds a reference to a statistics object
        self.stats = histogram_statistics(self)  
        
        # store a string for every dimension. can be used to label plots
        if labels is not None:
            assert (len(labels) == ndim)
            self.labels = list(labels)
        else:
            self.labels = [None] * ndim

        # histrogram title string
        self.title = title

        # now go through the provided bin edges and construct the data arrays
        # if not already existent: add overflow bins to binedges
        for edges in binedges:
            edg = n.asarray(edges)
            if  n.isfinite(edg[0]) and n.isfinite(edg[-1]): # have to add overflow bins
                self._h_binedges.append(n.hstack((-n.inf, edg, n.inf)))
            elif n.isinf(edg[0]) and n.isinf(edg[-1]):      # overflowbins already there
                self._h_binedges.append(edg)
            else:
                raise ValueError("first and last bins must be together either finite or infinite (overflow bins)")
            assert len(self._h_binedges[-1]) - 3 == len(self._h_bincenters[-1])
            assert len(self._h_bincenters[-1]) == len(self._h_binwidths[-1])
        
        # create the histogram arrays (n-dimensional datacubes)
        # these internal arrays have overflow bins
        datacubeshape = [len(i)-1 for i in self._h_binedges]
        # use external arrays for storage if provided
        if bincontent is None:
            self._h_bincontent     = n.zeros(datacubeshape, dtype=float)
        else:
            self._h_bincontent = n.asarray(bincontent)
            assert self._h_bincontent.shape == tuple(datacubeshape)
        if squaredweights is None:
            self._h_squaredweights = n.zeros(datacubeshape, dtype=float)
        else:
            self._h_squaredweights = n.asarray(squaredweights)
            assert self._h_squaredweights.shape == tuple(datacubeshape)

        # present views of the non overflow bins to the outside 
        self._h_visiblerange = [slice(1,-1) for i in xrange(self.ndim)]
        self.bincontent      = self._h_bincontent[self._h_visiblerange]
        self.squaredweights  = self._h_squaredweights[self._h_visiblerange]
        self.nbins           = tuple([i-2 for i in self._h_bincontent.shape])

        self._h_newdataavailable = True # internal trigger for the recalculation of dervived values (e.g. errors, stats,..)



    def fill(self, sample, weights=None):
        """
            fill values from sample into the histogram
            
            **Parameters:**
                *sample* : [ numpy.ndarray | tuple ]
                    the value(s) to be filled into this histogram. If a numpy array is given
                    it must have the shape (nentries, ndim). A tuple of len(ndim) holding
                    equally sized arrays is accepted, too.
                *weights* : [ None, numpy.ndarray ]
                    If given, each entry in sample contributes with its respective weight to
                    the histogram. The lengths of the sample and weights array must be
                    compatible.
        """

        if isinstance(sample, tuple):
            if len(sample) != self.ndim:
                raise ValueError("given tuple must have lenght of ndim=%d" % self.ndim)
            if not all([hasattr(i, "__len__") for i in sample]):
                raise ValueError("given tuple doesn't contain iterable items")
            if not len( n.unique([len(i) for i in sample])) == 1:
                raise ValueError("given tuple contains unequal sized arrays")

            sample = n.vstack(sample).T
        elif isinstance(sample, n.ndarray):
            if len(sample.shape) == 1:
                if self.ndim > 1:
                    raise ValueError("shape mismatch. provided sample has 1 dimension but the histogram has %d" % self.ndim)
                else:
                    sample = sample.reshape(len(sample), 1)
            elif len(sample.shape) == 2:
                if not sample.shape[1] == self.ndim:
                    raise ValueError("shape mismatch. provided sample should have dimensions (len, ndim=%d)" % self.ndim)
        else:
            raise ValueError("sample must be either a numpy array or a tuple of numpy arrays")

        if not len(sample) >= 1:
            return

        # catch nans ...
        if weights != None:
            assert len(weights) == len(sample)
            if n.isnan(weights).any():
                raise ValueError("given weights contain nans!")

        nanmask = n.zeros(len(sample), dtype=bool)
        for dim in xrange(self.ndim):
            nanmask |= n.isnan(sample[:,dim])

        nnans = len(nanmask[nanmask])

        nentries = len(sample)
        self.stats.values["nentries"] += nentries
        if nnans > 0:
            if weights==None:
                self.stats.values["nans"] += nnans
                self.stats.values["nans_wgt"] += nnans
                self.stats.values["nans_sqwgt"] += nnans
            else:
                w = weights[nanmask]
                self.stats.values["nans"] += nnans
                self.stats.values["nans_wgt"] += w.sum()
                self.stats.values["nans_sqwgt"] += n.power(w,2).sum()
        
            goodmask = n.logical_not(nanmask)

            # ... and get rid of them
            sample = sample[goodmask]
            if weights != None:
                weights = weights[goodmask]

        self._h_newdataavailable = True

        if len(sample) > 0:
            # call numpy histogramdd to get the bincontents and add it to the local bincontent array
            histnd, edges = n.histogramdd(sample,bins=self._h_binedges, weights=weights)
            self._h_bincontent += histnd
            if weights == None:
                self._h_squaredweights += histnd
            else:
                histnd, edges = n.histogramdd(sample,bins=self._h_binedges,weights=n.power(weights,2))
                self._h_squaredweights += histnd
        else:
            log_warn("histogram : Only nans or infinites were passed to fill!")

    def clear(self):
        """
            Sets all counters back to zero.
        """
        self._h_bincontent.fill(0)    
        self._h_squaredweights.fill(0)
        self.stats.clear()

    def empty_like(self):
        """
            Creates a new empty histogram like this one.
            Will be overridden by hist1d and hist2d.
        """
        return histogram(self.ndim, [edges.copy() for edges in self._h_binedges], labels=list(self.labels), title=self.title)

    def is_compatible(self, other):
        """
            Tests for compatibility of two histograms, i.e. it
            checks for the correct dimensionality and the exact
            same binning.
        """
        try:   
            assert (type(self) == type(other))
            assert (self.ndim == other.ndim)
            for i in xrange(self.ndim):
                assert (self._h_binedges[i] == other._h_binedges[i]).all()
        except AssertionError, ex:
            log_error("histogram : %s" % str(ex))
            return False

        return True

    # arithmetic operations on histograms
    def __add__(self, other):
        "implement histograms + histogram"

        if not self.is_compatible(other):
            raise ValueError("histograms are not compatible")

        newhist = self.empty_like()
        newhist += self
        newhist += other

        return newhist

    def __radd__(self,other):
        return self.__add__(other)

    def __iadd__(self, other):
        "implement histogram += histogram"
        if not self.is_compatible(other):
            raise ValueError("histograms are not compatible")
        
        self._h_bincontent     += other._h_bincontent
        self._h_squaredweights += other._h_squaredweights
        self._h_newdataavailable = True
        self.stats.values["nentries"] += other.stats.values["nentries"]
        self.stats.values["nans"] += other.stats.values["nans"]
        self.stats.values["nans_wgt"] += other.stats.values["nans_wgt"]
        self.stats.values["nans_sqwgt"] += other.stats.values["nans_sqwgt"]

        return self
    
    def __sub__(self, other):
        raise NotImplementedError()
    
    def __imul__(self, other):
        "implement histogram *= scalar"
        if not n.isscalar(other):
            raise ValueError("multiplication is only implemented for scalars")
        
        self._h_bincontent *= float(other)
        self._h_squaredweights *= float(other)**2
        # /!\ keep nentries and nans unmodified <-> multiplication scales weights
        self.stats.values["nans_wgt"] *= float(other)
        self.stats.values["nans_sqwgt"] *= float(other)**2
        self._h_newdataavailable = True
        return self
    
    def __mul__(self, other):
        "implement histogram * scalar"
        if not n.isscalar(other):
            raise ValueError("division is only implemented for scalars")
        
        newhist = self.empty_like()
        newhist += self
        newhist *= other
        return newhist
    
    def __rmul__(self,other):
        "implement scalar * histogram"
        return self.__mul__(other)

    def __idiv__(self, other):
        "implement histogram /= scalar"
        if not n.isscalar(other):
            raise ValueError("division is only implemented for scalars")
        
        if other != 0:
            self *= ( 1. / float(other) )
        return self
    
    def __div__(self, other):
        "implement histogram / scalar"
        if not n.isscalar(other):
            raise ValueError("division is only implemented for scalars")
        if other != 0:
            return (self * ( 1. / float(other) ) )
        else:
            return self
            
    def __getitem__(self, slice_):
        """
        implement histogram[index]
        """
        idxs = [i for i, sl in enumerate(slice_) if isinstance(sl, slice)]
        subedges = [self._h_binedges[i] for i in idxs]
        ndim = len(subedges)
        if ndim == 1:
            new = hist1d(subedges[0])
        elif ndim == 2:
            new = hist2d(subedges)
        else:
            new = histogram(ndim, subedges)
        new._h_bincontent = self._h_bincontent[slice_]
        new._h_squaredweights = self._h_squaredweights[slice_]
    
        new.bincontent = new._h_bincontent[new._h_visiblerange]
        new.squaredweights = new._h_squaredweights[new._h_visiblerange]
    
        return new
    
    def project(self, dims=[-1]):
        """
        Project the histogram onto a subset of its dimensions by summing
        over all dimensions not provided in *dim*.
        """
        if len(dims) > self.ndim or max(dims) >= self.ndim:
            raise ValueError("Can't slice dimensions %s out of a %d-d histogram" % (dims, self.ndim))
        dims.sort()
        subedges = [self._h_binedges[i] for i in dims]
        if len(dims) == 1:
            new = hist1d(subedges[0])
        elif len(dims) == 2:
            new = hist2d(subedges)
        else:
            new = histogram(len(dims), subedges)
    
        new._h_bincontent = self._h_bincontent
        new._h_squaredweights = self._h_squaredweights
    
        off = 0
        for i in xrange(self.ndim):
            if i in dims:
                continue
            new._h_bincontent = new._h_bincontent.sum(axis=i-off)
            new._h_squaredweights = new._h_squaredweights.sum(axis=i-off)
            off += 1
    
        new.bincontent = new._h_bincontent[new._h_visiblerange]
        new.squaredweights = new._h_squaredweights[new._h_visiblerange]

        return new
