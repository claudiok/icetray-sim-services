import matplotlib as mpl
mpl.use('agg')
import matplotlib.pyplot as plt

import copy
from copy import deepcopy
import numpy
from numpy import log10
#from ..kombu import histfactory

class Histogram :
    """
    This class manages a kombu.histograms, providing an interface that
    allows it to fit a little better into the sanity checker framework.
    """
    def __init__(self, frame_op, draw_args, name):
        """
        frame_op - A function that take a frame as an argument and
        returns a number, a list of numbers, a 2-tuple of numbers, or a
        list of 2-tuple.  2-tuple are interpreted as weighted values,
        where the first element is the data and the second is its weight,
        i.e. (data, weight).
        draw_args - Dictionary of drawing arguments.  Only supports the following.
          o log - Generate a log-y histogram.
          o xticks_args and xticks_kwargs - arguments and keyword arguments
            forwarded directly to pylab.xticks.
          o title - Fwd'd directly to pylab.title.
          o xlabel - Fwd'd directly to pylab.xlabel.
          o ylabel - Fwd'd directly to pylab.ylabel.
          o figname - Filename of the ouptut figure.
        name - String name of this histogram.
        """
        self.hist = None
        self.frame_op = frame_op
        self.data = list()
        self.draw_args = draw_args
        self.name = name

    def copy(self, name = None):
        """
        Makes an empty copy of the histogram with the same name if none
        is specfied. This is useful when adding two histograms together.
        """
        d = deepcopy(self.draw_args)
        return Histogram(self.frame_op, d, \
                         name = name if name else self.name)

    def fill(self, frame):
        """
        Applies self.frame_op to frame and adds the result to self.data.
        Calls append of the return value from frame_op is a scalar, otherwise
        it calls extend.
        """
        rval = self.frame_op(frame)
        if rval == None : return
        if numpy.isscalar(rval) or \
               isinstance(rval, tuple) :
            self.data.append( rval )
        else :
            self.data.extend( rval )

    def generate_histogram(self) :
        """
        Passes self.data to kombu.histfactory.generate_hist1d and stores the result
        as an internal member self.hist.
        """
        if len(self.data) == 0 : return
        if isinstance(self.data[0], tuple) \
          and len(self.data[0]) == 2 :
            _data = [t[0] for t in self.data if not numpy.isnan(t[0])]
            _weights = [t[1] for t in self.data if not numpy.isnan(t[0])]        
            # The data is weighted if the elements are tuples.
            self.hist = numpy.histogram( _data, weights = _weights, bins = self.draw_args["bins"])
            #self.hist = numpy.histogramdd( _data, weights = _weights, bins = self.draw_args["bins"])
            #self.hist = plt.hist( _data, weights = _weights, bins = self.draw_args["bins"])
        else:
            # The elements of data should be scalars
            if numpy.isscalar(self.data[0]) :
                _data = [d for d in self.data if not numpy.isnan(d)]
                #self.hist = plt.hist( _data, bins = self.draw_args["bins"])
                #self.hist = numpy.histogramdd( _data, bins = self.draw_args["bins"])
                self.hist = numpy.histogram( _data, bins = self.draw_args["bins"])
            else:
                print("Malformed data of type %s.", type(self.data[0]))
                print("This needs to either be a 2-tuple or a scalar.")
                print("  numpy.isscalar(data) = %s " % str(numpy.isscalar(self.data[0])))
                try :
                    print("  len(data) = ", len(self.data[0]))
                except TypeError as e :
                    print e

    def draw(self, path = "./", stats = True, label = None) :
        """
        Arguments
        path - path to store the output plot.
        stats - bool to display a stats box.
        label - Plot label.
        """
        if not self.hist : return 
        #self.hist.line(log = self.draw_args["log"] \
        #               if "log" in self.draw_args else False, label = label)
        #self.hist.line(label = label)

        _hist = self.hist[0]
        _bin_edges = self.hist[1]

        # need to make steps
        x = list()
        y = list()
        bin_length = _bin_edges[1] - _bin_edges[0]
        for be, hval in zip(_bin_edges, _hist):
            if self.draw_args["log"] : hval = log10(hval)
            x.append(be)
            y.append(hval)        
            x.append(be + bin_length)
            y.append(hval)

        plt.plot(x,y,"-")
            
        if "xticks_args" in self.draw_args :
            plt.xticks(*(self.draw_args["xticks_args"]),
                         **(self.draw_args["xticks_kwargs"]))

        #if stats :
        #    self.hist.statbox()

        if "title" in self.draw_args :
            plt.title(self.draw_args["title"])

        if "ylabel" in self.draw_args :
            plt.ylabel(self.draw_args["ylabel"])

        if "xlabel" in self.draw_args :
            plt.xlabel(self.draw_args["xlabel"])

    def save(self, path = "./") :
        """
        Draws and saves the plot with the name draw_args['figname'] in path.
        """
        plt.figure()
        self.draw()
        if not path.endswith("/") : path += "/"
        plt.savefig(path + self.draw_args["figname"])

    def save_as(self, filename) : 
        """
        Draws and saves the figure with the given name.
        """
        plt.figure()
        self.draw()
        plt.savefig(filename)

    def __iadd__(self, other):
        """
        Add the 'other' histogram to this histogram and return self
        e.g. hist += other_hist
        """
        self.hist += other.hist
        self.hist.bincontent += other.hist.bincontent
        return self

    def __add__(self, other):
        """
        Add the 'other' histogram to this historgram and return a copy.
        e.g. hist = hist1 + hist2
        """
        newhist = copy.deepcopy(self)
        newhist.hist += other.hist
        newhist.hist.bincontent += other.hist.bincontent
        return newhist

    def __idiv__(self, other):
        """
        Divide this histogram by the 'other' histogram.
        e.g. hist /= other_hist
        """
        self.hist /= other
        self.hist.bincontent /= other
        return self

    def __getstate__(self) :
        """
        When pickle writes an object this method is called and
        the return state is pickled as the contents of the instance.
        We purposely DO NOT pickle the data to save disk space.
        Only that's which needed to make a plot is stored.
        """
        state = { "hist" : self.hist,
                  "frame_op" : self.frame_op,
                  "draw_args" : self.draw_args,
                  "name" : self.name
                  }
        return state
