import pylab
import copy 
from ..kombu import histfactory

class Histogram :
    def __init__(self, frame_op, draw_args ): 
        self.hist = None
        self.frame_op = frame_op
        self.data = list()
        self.draw_args = draw_args
        
    def fill(self, frame):
        self.data.extend( self.frame_op(frame) )

    def generate_histogram(self) :
        self.hist = histfactory.generate_hist1d( self.data,
                                                 bins = self.draw_args["bins"],
                                                 label = self.draw_args["label"],
                                                 title = self.draw_args["title"])

    def draw(self, path = "./") : 
        pylab.figure()
        self.hist.line(log = self.draw_args["log"] \
                       if "log" in self.draw_args else False)
        if "xticks_args" in self.draw_args :
            pylab.xticks(*(self.draw_args["xticks_args"]),
                         **(self.draw_args["xticks_kwargs"]))

        self.hist.statbox()

        if not path.endswith("/") : path += "/"
        pylab.savefig(path + self.draw_args["figname"])

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
