import numpy
from icecube import dataclasses
from ..bases.histogram import Histogram
from .utils import bins

def _frame_op(frame):
    return[ len([1 for p in frame["I3MCTree"]\
                 if (p.type == dataclasses.I3Particle.MuMinus or \
                     p.type == dataclasses.I3Particle.MuPlus)]) ]

_draw_args = { "bins" : bins(100,0,100),
               "xlabel" : "N",
               "title" : "N (mu+,mu-)",
               "figname" : "n_mu.png",
               "log" : True }

n_mu_h = Histogram(frame_op = _frame_op,
                   draw_args = _draw_args)
