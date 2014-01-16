
import numpy as n
import pylab as p
from histogram import LegendProxy

class points2d(object):
    def __init__(self, npoints=None):
        if npoints is None:
            self.x = None
            self.y = None
            self.xerr = None
            self.yerr = None
        else:
            self.x = n.zeros(npoints, dtype=float)
            self.y = n.zeros(npoints, dtype=float)
            self.xerr = n.zeros(npoints, dtype=float)
            self.yerr = n.zeros(npoints, dtype=float)
        
        self.labels = [None, None]

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

    def scatter(self, log=False, color=None, label=None, orientation='horizontal', **kwargs):
        """ use pylab.errorplot to visualize these scatter points
        
        Parameters:
        log        : if true create logartihmic plot
        
        (all other kwargs will be passed to pylab.errobar)
        """

        ax = p.gca()
        if color is None:
            color = ax._get_lines.color_cycle.next()
    
        kw = {"xerr" : self.xerr,
              "yerr" : self.yerr,
              "fmt" : "k",
              "capsize" : 0.,
              "linestyle" : 'None',
              "color" : color}
        # for log-scaled axes, clip the lower extent of the error bar
        if log:
            kw["yerr"] = [n.where(self.y-self.yerr <= 0, abs(self.y)*(1-1e-12), self.yerr), self.yerr]
        kw.update(kwargs)

        if len(ax.lines) > 0:
            minvalue,maxvalue = p.ylim()
        else:
            minvalue,maxvalue = float('inf'),-float('inf')
    
        if orientation == 'vertical':
            set_xlim, set_ylim = ax.set_ylim, ax.set_xlim
            get_xlim, get_ylim = ax.get_ylim, ax.get_xlim
            set_yscale = ax.set_xscale
            x, y = self.y, self.x
            kw["xerr"], kw["yerr"] = kw["yerr"], kw["xerr"]
        else:
            set_xlim, set_ylim = ax.set_xlim, ax.set_ylim
            get_xlim, get_ylim = ax.get_xlim, ax.get_ylim
            set_yscale = ax.set_yscale
            x, y = self.x, self.y
    
        if len(self.x > 0):
            if not log:
                if n.abs(self.y).sum() > 0:
                    minvalue = min( self.y.min(), minvalue )
                    maxvalue = max( 1.3*self.y.max(), maxvalue)
                    p.errorbar(x, y, **kw) 
                else:
                    set_xlim(self.x[0], self.x[-1])
            else:
                set_yscale("log", nonposy='clip')
                ymi,yma = get_ylim()
                if n.abs(self.y).sum() > 0:
                    minvalue = min(0.1 * self.y[self.y > 0].min(), ymi)
                    maxvalue = max( n.power(10, n.ceil (n.log10(100. * self.y.max()))), maxvalue)
                    p.errorbar(x, y, **kw) 
                else:
                    set_xlim(self.x[0], self.x[-1])
        
            if not hasattr(ax, "_legend_proxy"):
                ax._legend_proxy = LegendProxy(ax)
            ax._legend_proxy.add_scatter(label=label, color=color)
        
            set_ylim(minvalue, maxvalue)
        self._h2label(orientation)


class grid2d(object):
    def __init__(self, shape=None):
        if shape is None:
            self.x = None
            self.y = None
            self.z = None
            self.xerr = None
            self.yerr = None
            self.zerr = None
        else:
            self.x = n.zeros(shape, dtype=float)
            self.y = n.zeros(shape, dtype=float)
            self.z = n.zeros(shape, dtype=float)
            self.xerr = n.zeros(shape, dtype=float)
            self.yerr = n.zeros(shape, dtype=float)
            self.zerr = n.zeros(shape, dtype=float)
        
        self.labels = [None, None, None]

    def imshow(self, **kwargs):
        """ plot the grid2d with pylab.imshow()
        Parameters:
        log    : if true create logartihmic plot. Empty
        bins will be filled up the lowest populated decade.
        
        (all other kwargs will be passed to pylab.imshow)
        """
        z = self.z.T.copy()

        if kwargs.pop("log", False):
            if z.sum() > 0:
                z[z==0] = n.nan # won't be drawn by imshow
                z = n.log10(z) # log of nans are nans
        else:
            if z.sum() > 0:
                z[z == 0] = n.nan                 

        kw = {"cmap": mpl.cm.jet, "aspect" : "auto", "interpolation" : "nearest" }
        kw.update(kwargs)
        p.imshow(z, origin="lower", 
                 extent=(self.x[0], self.x[-1], self.y[0], self.y[-1]), **kw) 
        _h2label(self)

    def pcolor(self, **kwargs):
        """ plot the grid2d with pylab.pcolor()
        Parameters:
        log    : if true create logartihmic plot. Empty
        bins will be filled up the lowest populated decade.
        
        (all other kwargs will be passed to pylab.pcolor)
        """
        bincontent = self.z.T.copy()
        x = n.concatenate((self.x-self.xerr, [self.x[-1]+self.xerr[-1]]))
        y = n.concatenate((self.y-self.yerr, [self.y[-1]+self.yerr[-1]]))
        
        _process_colorscale(bincontent, kwargs)
        return p.pcolor(x, y, bincontent, **kwargs)
