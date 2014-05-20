import pylab
import copy 
from copy import deepcopy
import numpy
from ..kombu import histfactory

class Histogram :
    def __init__(self, frame_op, draw_args, setup_test = lambda frame : True ): 
        self.hist = None
        self.frame_op = frame_op
        self.data = list()
        self.draw_args = draw_args
        self.setup_test = setup_test

    def add_test(name, func) :
        self.__dict__[test + name] = func

    def copy(self):
        d = deepcopy(self.draw_args)
        return Histogram(self.frame_op, d)
        
    def fill(self, frame):
        rval = self.frame_op(frame)
        if numpy.isscalar(rval) or \
               isinstance(rval, tuple) :
            self.data.append( rval )
        else : self.data.extend( rval )

    def generate_histogram(self) :
        if len(self.data) > 0 \
           and isinstance(self.data[0], tuple) :
            self.hist = histfactory.generate_hist1d( [t[0] for t in self.data],
                                                     weights = [t[1] for t in self.data],
                                                     bins = self.draw_args["bins"])
        else:
            self.hist = histfactory.generate_hist1d( self.data,
                                                     bins = self.draw_args["bins"])

    def draw(self, path = "./", stats = True) : 
        self.hist.line(log = self.draw_args["log"] \
                       if "log" in self.draw_args else False)
        if "xticks_args" in self.draw_args :
            pylab.xticks(*(self.draw_args["xticks_args"]),
                         **(self.draw_args["xticks_kwargs"]))

        if stats :
            self.hist.statbox()

        if "title" in self.draw_args :
            pylab.title(self.draw_args["title"])

        if "ylabel" in self.draw_args :
            pylab.ylabel(self.draw_args["ylabel"])

        if "xlabel" in self.draw_args :
            pylab.xlabel(self.draw_args["xlabel"])

    def save(self, path = "./") : 
        pylab.figure()
        self.draw()
        if not path.endswith("/") : path += "/"
        pylab.savefig(path + self.draw_args["figname"])

    def save_as(self, filename) : 
        pylab.figure()
        self.draw()
        pylab.savefig(filename)

    def __iadd__(self, other):
        self.hist += other.hist
        self.hist.bincontent += other.hist.bincontent
        return self

    def __add__(self, other):
        newhist = copy.deepcopy(self)
        newhist.hist += other.hist
        newhist.hist.bincontent += other.hist.bincontent
        return newhist

    def __idiv__(self, other):
        self.hist /= other
        self.hist.bincontent /= other
        return self

    def __getstate__(self) :
        state = { "hist" : self.hist,
                  "frame_op" : self.frame_op,
                  "draw_args" : self.draw_args 
                  }
        return state
