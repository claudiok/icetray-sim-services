import copy
import numpy as n
import pylab as p
import matplotlib as mpl
import histfuncs
from icecube.icetray.i3logging import log_warn
from icecube.icetray.i3logging import log_error
from odict import OrderedDict
from infobox import InfoBox 
from histogram import histogram
from histogram import LegendProxy

def _h1label(h1):
    label = h1.labels[0]
    if label is not None:
        ax = p.gca()
        if ax.get_xlabel() == '':
            ax.set_xlabel(label)

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


def _h1_transform_bins(self, differential=False, cumulative=False, cumdir=1):
    """
    Transform bin contents in errors to values appropriate for a differential
    or cumulative histogram.
    
    For cumdir=1, the bins contain the sum of weights associated with values
    <= to the *right-hand* bin edge; for cumdir=-1, the bin value is the sum
    of weights for values > the *left-hand* bin edge.
    
    (JvS)
    """
    if (cumulative and differential):
        raise ValueError("cumulative and differential are mutually exclusive!")
    if cumulative:
        if cumdir == 1:
            vis = slice(1, -1)
            bincontent = n.cumsum(self._h_bincontent)[vis]
            binerror   = n.sqrt(n.cumsum(self._h_squaredweights))[vis]
        elif cumdir == -1:
            vis = slice(0, -2)
            bincontent = self._h_bincontent.sum() - n.cumsum(self._h_bincontent)[vis]
            binerror  = n.sqrt( (self._h_squaredweights).sum() - n.cumsum(self._h_squaredweights)[vis] )
        else:
            raise ValueError("cumdir should be 1 or -1")
    elif differential:
        bincontent = self.bincontent/self.binwidths
        binerror   = self.binerror/self.binwidths
    else:
        bincontent = self.bincontent
        binerror   = self.binerror
    return bincontent, binerror

class hist1d(histogram):
    """
        one dimensional specialization for :class:`histogram.histogram`
    """

    binedges   = property(lambda self : self._h_binedges[0][1:-1], None)
    bincenters = property(lambda self : self._h_bincenters[0], None)
    binwidths  = property(lambda self : self._h_binwidths[0])
    xerr       = property(lambda self : self._h_binwidths[0]/2.)
    binerror   = property(lambda self : n.sqrt(self._h_squaredweights[1:-1]), None)

    def __init__(self, binedges, bincontent=None, squaredweights=None, label=None, title=None):
        if label is not None:
            label = (label,)
        histogram.__init__(self, 1, (binedges,),\
                           bincontent=bincontent,\
                           squaredweights=squaredweights,\
                           labels=label,\
                           title=title)
    
    def empty_like(self):
        return hist1d(self._h_binedges[0].copy(), label=self.labels[0], title=self.title)

    def normalized(self, norm=1., density=False):
        if density:
            return self / (norm* ((self.bincontent*self.binwidths).sum()))
        else:
            return self / (norm*self.stats.weightsum)

    def rebin(self, bins_to_merge=2):
            """ rebins the histogram (was tried to be written by Arne, so don't blame Eike!)
            gives back new rebinned histogram (old one is not overwritten)
            if number of bins is not a multiple of number, excess bins are thrown in the overflow bin 
            
            bins_to_merge: number of bins to merge to new bin (standard value = 2)"""
            return histfuncs.Rebin(self, bins_to_merge)          

    @property
    def func(self):
        """
            return the bincontent of this histogram as a callable
        """
        def method(x):
            if x < self.binedges[0] or self.binedges[-1] < x:
                return 0.
            else:
                for i,(bin_min,bin_max) in enumerate(zip(self.binedges[:-1], self.binedges[1:])):
                    if (bin_min <= x) and (x<bin_max):
                        return self.bincontent[i]
        return n.vectorize(method)
            


    def scatter(self, log=False, cumulative=False, cumdir=1, color=None, differential=False, **kwargs):
        """ use pylab.errorplot to plot a 1d histogram 
        
        Parameters:
        log        : if true create logartihmic plot
        cumulative : plot the cumulative histogram
        
        (all other kwargs will be passed to pylab.errobar)
        """

        if self.stats.weightsum == 0:
            return
    
        bincontent, binerror = _h1_transform_bins(self, differential, cumulative, cumdir)
    
        ax = p.gca()
        if color is None:
            color = ax._get_lines.color_cycle.next()
    
        kw = {
            "xerr" : self.xerr,
            "yerr" : binerror,
            "fmt" : "k",
            "capsize" : 0.,
            "linestyle" : 'None',
            "color" : color,
            }
        # for log-scaled axes, clip the lower extent of the error bar
        if log:
            kw["yerr"] = [n.where(bincontent-binerror <= 0, abs(bincontent)*(1-1e-12), binerror), binerror]

        patches = None

        kw.update(kwargs)
        if len(ax.lines) > 0:
            minvalue,maxvalue = p.ylim()
        else:
            minvalue,maxvalue = float('inf'),-float('inf')
    
        if not hasattr(ax, "_legend_proxy"):
            ax._legend_proxy = LegendProxy(ax)
        label = kw.pop('label', self.title)
        ax._legend_proxy.add_scatter(label=label, **kw)
    
        if not log:
            if self.stats.weightsum > 0:
                minvalue = min( bincontent.min(), minvalue )
                maxvalue = max( 1.3*bincontent.max(), maxvalue)
                patches = p.errorbar(self.bincenters, bincontent, **kw) 
            else:
                p.xlim(self.binedges[0], self.binedges[-1])
        else:
            ax = p.gca()
            ax.set_yscale("log", nonposy='clip')
            ymi,yma = p.ylim()
            if len(p.gca().get_lines()) != 0:
                ymi = min([i._y[i._y > 0].min() for i in p.gca().get_lines()])
            if ymi == 0:
                ymi = float('inf')
            if self.stats.weightsum > 0:
                lower = bincontent[bincontent > 0]-binerror[bincontent>0]
                nonzerolower = lower[lower>0]
                if len(nonzerolower)>0:
                    if nonzerolower.min() > 0:
                        minvalue = min(0.1 * nonzerolower.min(), ymi)
                    else:
                        minvalue = min(0.05* lower.min(), ymi)
                else:
                    minvalue = ymi
                maxvalue = max( n.power(10, n.ceil (n.log10(100. * bincontent.max()))), maxvalue)
                patches = p.errorbar(self.bincenters, bincontent, **kw) 
            else:
                p.xlim(self.binedges[0], self.binedges[-1])
    
        p.ylim(minvalue, maxvalue)
        _h1label(self)

        return patches

    def band(self, log=False, type="steps", differential=False, cumulative=False, cumdir=1, **kwargs):
        """ plot a filled band to indicate bincontents and binerrors 
    
        Parameters:
        log : if true create logartihmic plot
        cumulative : plot the cumulative histogram
        type: ['line', 'steps']
        
        (all other kwargs will be passed to pylab.fill_between)
        """
        
        if self.stats.weightsum == 0:
            return

        bincontent, binerror = _h1_transform_bins(self, differential, cumulative, cumdir)

        if type.lower() == "line":
            x  = self.bincenters
            y1 = bincontent - binerror
            y2 = bincontent + binerror
        elif type.lower() == "steps":
            nbins = self.nbins[0]
            x = n.zeros(2*(nbins+1), dtype=float)
            y1 = n.zeros(2*(nbins+1), dtype=float)
            y2 = n.zeros(2*(nbins+1), dtype=float)

            for i in xrange(nbins):
                x[1+2*i] = self.binedges[i]
                x[2+2*i] = self.binedges[i+1]
                y1[1+2*i] = bincontent[i] - binerror[i]
                y1[2+2*i] = bincontent[i] - binerror[i]
                y2[1+2*i] = bincontent[i] + binerror[i]
                y2[2+2*i] = bincontent[i] + binerror[i]

            # left-most point
            x[0] = self.binedges[0]
            y1[0] = 0
            y2[0] = 0
            # right-most point
            x[-1] = self.binedges[-1]
            y1[-1] = 0
            y2[-1] = 0
        else:
            raise ValueError("did't understand given type %s" % type)

        minvalue,maxvalue = p.ylim()
        if log:
            ax = p.gca()
            ax.set_yscale("log", nonposy='clip')
            ymi,yma = p.ylim()
            nonemptybins = bincontent[bincontent > 0]
            if len(nonemptybins) > 0:
                minvalue = min(0.1 * nonemptybins.min(), ymi)
                maxvalue = max( n.power(10, n.ceil (n.log10(100. * nonemptybins.max()))), maxvalue)
        else:
            minvalue = min( bincontent.min(), minvalue )
            maxvalue = max( 1.3*bincontent.max(), maxvalue)

        ax = p.gca()
        if not hasattr(ax, "_legend_proxy"):
            ax._legend_proxy = LegendProxy(ax)
        label = kwargs.pop('label', self.title)
        ax._legend_proxy.add_fill(label=label, **kwargs)

        kw = {}
        kw.update(kwargs)
        p.fill_between(x, y1, y2, **kw)

    
        p.ylim(minvalue, maxvalue)
        _h1label(self)

    def line(self, log=False, cumulative=False, differential=False, cumdir=1,
             filled=False, color=None, orientation='horizontal', **kwargs):
        """ plot the histogram's  bincontent with a line using pylab.plot. 
        Parameters:
        log    : if true create logartihmic plot
        cumulative : plot the cumulative histogram
        filled : if true fill the region below the line 
    
        (all other kwargs will be passed to pylab.plot or pylab.fill)
        Note: pylab.plot and pylab.fill take quite different kwargs.
        """
    
        bincontent, binerror = _h1_transform_bins(self, differential, cumulative, cumdir)
    
        nonzerobc = bincontent[bincontent > 0]
        if len(nonzerobc) == 0:
            return

        nbins = self.nbins[0]

        xpoints = n.zeros(2*(nbins+1), dtype=float)
        ypoints = n.zeros(2*(nbins+1), dtype=float)

        for i in xrange(nbins):
            xpoints[1+2*i] = self.binedges[i]
            xpoints[2+2*i] = self.binedges[i+1]
            ypoints[1+2*i] = bincontent[i]
            ypoints[2+2*i] = bincontent[i]

        xpoints[0] = self.binedges[0]
        ypoints[0] = 0
        xpoints[-1] = self.binedges[-1]
        ypoints[-1] = 0
        # TODO eventually add another point to close area?
    
        ax = p.gca()
        if orientation == 'vertical':
            xpoints, ypoints = ypoints, xpoints
            ylim = p.xlim
            set_yscale = lambda v: ax.set_xscale(v, nonposx='clip')
        else:
            ylim = p.ylim
            set_yscale = lambda v: ax.set_yscale(v, nonposy='clip')

        minvalue,maxvalue = ylim()
        if minvalue == 0 and maxvalue == 1:
            maxvalue=0.
        if log:
            set_yscale("log")
            if len(nonzerobc) > 0:
                minvalue = min( n.power(10, n.floor(n.log10(0.1 * nonzerobc.min()))) , minvalue)
                maxvalue = max( n.power(10, n.ceil (n.log10(100. * bincontent.max()))), maxvalue)
                ypoints[ypoints == 0] = minvalue
        else:
            set_yscale("linear")
    
        if not hasattr(ax, "_legend_proxy"):
            ax._legend_proxy = LegendProxy(ax)
        label = kwargs.pop('label', self.title)
        if color is None:
            color = ax._get_lines.color_cycle.next()
        kwargs['color'] = color
        if filled:
            ax._legend_proxy.add_fill(label=label, **kwargs)
        else:
            ax._legend_proxy.add_line(label=label, **kwargs)
    
        if filled:
            kw = {"ec":"k", "fc": color}
            kw.update(kwargs)
            p.fill(xpoints, ypoints, **kw) 
            minvalue = min( bincontent.min(), minvalue )
            maxvalue = max( 1.3*bincontent.max() , maxvalue )
        else:
            kw = {"color":color}
            kw.update(kwargs)
            p.plot(xpoints, ypoints, "k-", **kw) 
            minvalue = min( bincontent.min(), minvalue )
            maxvalue = max( 1.3*bincontent.max() , maxvalue )
            
        ylim(minvalue, maxvalue)
        _h1label(self)

    def statbox(self, axes=None, name=None, loc=2, fontsize=10, prec=5, other=None, **kwargs):
        assert self.ndim == 1

        stringdict = OrderedDict()

        formatfunc = dashi.histfuncs.number_format

        if not name:       
            name = None   #empty string causes pylab crash
    
        stringdict["N"]     = formatfunc(self.stats.weightsum, prec)
        stringdict["mean"]  = formatfunc(self.stats.mean, prec)
        stringdict["std"]   = formatfunc(self.stats.std, prec) 
        stringdict["med"]   = formatfunc(self.stats.median, prec)
        stringdict["uflow"] = formatfunc(self.stats.underflow, prec)
        stringdict["oflow"] = formatfunc(self.stats.overflow, prec)
        stringdict["nans"] = formatfunc(self.stats.nans_wgt, prec)

        if other is not None:
            if not isinstance(other, dict):
                raise ValueError("other must be a dict")
        
            for key, func in other.iteritems():
                stringdict[key] = formatfunc(func(self), prec)

        infobox = InfoBox(stringdict,title=name, **kwargs) # need to pop kwargs
        infobox.textprops["fontsize"] = fontsize
        infobox.titleprops["fontsize"] = fontsize
        infobox.draw(axes, loc)
