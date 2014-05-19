import numpy
from I3Tray import I3Units
from icecube import dataclasses
from ..bases.histogram import Histogram
from .utils import bins

def _frame_op(frame):
    return[ p.length/I3Units.m for p in frame["I3MCTree"]\
            if (p.type == dataclasses.I3Particle.MuMinus or \
                p.type == dataclasses.I3Particle.MuPlus) ]

_draw_args = { "bins" : bins(100,0,20.*I3Units.km),
               "xlabel" : "l(m)",
               "title" : r"$\mu \rm{ Length}$",
               "figname" : "mu_length.png",
               "log" : True }

mu_length_h = Histogram(frame_op = _frame_op,
                        draw_args = _draw_args)
