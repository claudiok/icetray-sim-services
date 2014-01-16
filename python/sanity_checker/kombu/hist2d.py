import copy
import numpy as n
import pylab as p
import matplotlib as mpl
import histfuncs
import histfactory
from icecube.icetray.i3logging import log_warn
from icecube.icetray.i3logging import log_error
from icecube.sim_services.sanity_checker.kombu.odict import OrderedDict
from infobox import InfoBox 
from histogram import histogram

def _process_colorscale(bincontent, kwargs):
    if not kwargs.get("norm", None):
        if kwargs.pop("log", False):
            from matplotlib.colors import LogNorm
            minval = 10**n.floor(n.log10(bincontent[bincontent>0].min()))
            maxval = bincontent[bincontent>0].max()
            kwargs["norm"] = LogNorm(vmin=minval, vmax=maxval)
        else:
            from matplotlib.colors import Normalize
            bincontent[bincontent==0] = n.nan
            kwargs["norm"] = Normalize(vmin=0, vmax=n.nanmax(bincontent))
    
    # Make sure the color map has an underflow color
    cmap = kwargs.pop("cmap", p.cm.jet)
    cmap = copy.deepcopy(cmap)
    cmap.set_under(color='w')
    cmap.set_bad(color='w')
    kwargs["cmap"] = cmap
    return kwargs

def _h2_transform_bins(self, kwargs):
    """
    Common manipulation for 2-d plots
    """
    bincontent = self.bincontent.T.copy()
    
    cumdir = kwargs.pop("cumdir", None)
    if cumdir is not None:
        bincontent = self._h_bincontent.cumsum(axis=1).cumsum(axis=0)[1:-1,1:-1].T
    
    _process_colorscale(bincontent, kwargs)
    return bincontent, kwargs


def _might_be_logspaced(bins):
    nslog = n.unique(bins[1:]/bins[:-1])
    nslin = n.unique(n.diff(bins))
    return len(nslog) < len(nslin)

class hist2d(histogram):
    """
        two dimensional specialization for :class:`histogram.histogram`
    """
    def __init__(self, binedges, bincontent=None, squaredweights=None, labels=None, title=None):
        histogram.__init__(self, 2, binedges,
                           bincontent=bincontent,
                           squaredweights=squaredweights,
                           labels=labels,
                           title=title)
    
    binedges   = property(lambda self : [i[1:-1] for i in self._h_binedges], None)
    bincenters = property(lambda self : self._h_bincenters, None)
    xerr       = property(lambda self : self._h_binwidths[0]/2.)
    yerr       = property(lambda self : self._h_binwidths[1]/2.)
    binerror   = property(lambda self : n.sqrt(self._h_squaredweights[self._h_visiblerange]), None)


    def _h2label(self, orientation='horizontal'):

        xlabel = self.labels[0]
        ylabel = self.labels[1]
        if orientation == 'vertical':
            xlabel, ylabel = ylabel, xlabel
        ax = p.gca()
        if ax.get_xlabel() == '' and ax.get_ylabel() == '':
            if (xlabel is not None):
                ax.set_xlabel(xlabel)
            if (ylabel is not None):
                ax.set_ylabel(ylabel)

    def empty_like(self):
        return hist2d(self._h_binedges, labels=self.labels, title=self.title)

    
    def normalized(self, norm=1., density=False):
        # please check formula for density!
        if density:
            return self/(norm*(((self.bincontent*self._h_binwidths[1]).transpose()*self._h_binwidths[0]).sum())) 
        else:
            return self / (norm*self.stats.weightsum)


    def imshow(self, **kwargs):
        """ plot the 2d histogram's  bincontent with pylab.imshow 
        Parameters:
        log    : if true create logartihmic plot. Empty
        bins will be filled up the lowest populated decade.
        
        (all other kwargs will be passed to pylab.imshow)
        """
        bincontent, kwargs = _h2_transform_bins(self, kwargs)

        kw = {"cmap": mpl.cm.jet, "aspect" : "auto", "interpolation" : "nearest" }
        kw.update(kwargs)
        img = p.imshow(bincontent, origin="lower", 
                       extent=(self.binedges[0][0],
                               self.binedges[0][-1],
                               self.binedges[1][0],
                               self.binedges[1][-1]), 
                       **kw) 
        self._h2label()
    
        return img

    def pcolor(self, **kwargs):
        """ plot the 2d histogram's  bincontent with pylab.pcolor 
        Parameters:
        log    : if true create logartihmic plot. Empty
        bins will be filled up the lowest populated decade.
        
        (all other kwargs will be passed to pylab.pcolor)
        """
        bincontent, kwargs = _h2_transform_bins(self, kwargs)

        img = p.pcolor(self.binedges[0], self.binedges[1], bincontent, **kwargs)
        self._h2label()
    
        return img

    def pcolormesh(self, **kwargs):
        """ plot the 2d histogram's  bincontent with pylab.pcolormesh 
        Parameters:
        log    : if true create logartihmic plot. Empty
        bins will be filled up the lowest populated decade.
        
        (all other kwargs will be passed to pylab.pcolor)
        """
        bincontent, kwargs = _h2_transform_bins(self, kwargs)

        img = p.pcolormesh(self.binedges[0], self.binedges[1], bincontent, **kwargs)
        self._h2label()
        
        return img

    def contour(self, levels=10, filled=False, clabels=False, **kwargs):
        """ plot the 2d histogram's  bincontent with pylab.imshow 
        Parameters:
        log    : if true create logartihmic plot. Empty
        bins will be filled up the lowest populated decade.
        filled : toggle beetween pylab.contour and pylab.contourf
        
        (all other kwargs will be passed to pylab.contour/pylab.contourf)
        """
        bincontent = self.bincontent.T.copy()

        if kwargs.pop("log", False):
            if bincontent.sum() > 0:
                bincontent[bincontent==0] = n.nan # won't be drawn by imshow
                bincontent = n.log10(bincontent) # log of nans are nans
        else:
            if bincontent.sum() > 0:
                bincontent[bincontent == 0] = n.nan
        
         

        kw = {}
        kw.update(kwargs)
        plfunc = None 
        cs = None
        if filled:
            cs = p.contourf(self.bincenters[0], self.bincenters[1], bincontent, levels, **kw) 
        else:
            cs = p.contour(self.bincenters[0], self.bincenters[1], bincontent, levels, **kw) 

        if clabels:
            p.clabel(cs, inline=1)
            
        self._h2label()
        
        return cs


    def stack1d(self, boxify=False, cmap=mpl.cm.jet, colorbar=True, cumdir=1, **kwargs):
        histos = []
        for i in xrange(self.nbins[1]):
            histos.append(histfactory.generate_hist1d(self.bincenters[0],
                                                      self.binedges[0],
                                                      weights=self.bincontent[:,i]))
    
        if cumdir >= 0:
            histocum = [reduce(lambda i,j : i+j, histos[:end]) for end in xrange(1,len(histos)+1)]
            it = reversed(list(enumerate(zip(histocum, self.bincenters[1]))))
        else:
            histocum = [reduce(lambda i,j : i+j, histos[begin:]) for begin in xrange(0,len(histos)-1)]
            it = iter(list(enumerate(zip(histocum, self.bincenters[1]))))
    
        log = _might_be_logspaced(self.binedges[1])
        if log:
            trafo = mpl.colors.LogNorm(vmin=self.bincenters[1][0], vmax=self.bincenters[1][-1])
        else:
            trafo = mpl.colors.Normalize(vmin=self.bincenters[1][0], vmax=self.bincenters[1][-1])
    
        for i,(h,cvalue) in it:
            if boxify:
                mask = histocum[-1]._h_bincontent > 0
                hscale = h.empty_like()
                hscale._h_bincontent[mask] = (h._h_bincontent / histocum[-1]._h_bincontent)[mask]
                hscale.line(filled=1, fc=cmap(trafo(cvalue)), **kwargs)
            else:
                h.line(filled=1, fc=cmap(trafo(cvalue)), **kwargs)
        p.xlim(self.binedges[0][0], self.binedges[0][-1])
        if boxify:
            p.ylim(0,1.05)
    
        cbargs = dict(cmap=cmap, norm=trafo, orientation='vertical')
        if log:
            # logspaced colorbars need a little hand-holding
            from matplotlib.ticker import LogFormatter
            cbargs['ticks'] = self.binedges[1]
            cbargs['format'] = LogFormatter()
    
        if colorbar:
            ax = p.gca()
            cax,cax_kw = mpl.colorbar.make_axes(ax)
            cax.hold(True)
            cb1 = mpl.colorbar.ColorbarBase(cax, **cbargs)
            p.gcf().sca(ax)
            if self.labels[0] is not None:
                ax.set_xlabel(self.labels[0])
            if self.labels[1] is not None:
                cb1.set_label(self.labels[1])
        else:
            return cbargs
