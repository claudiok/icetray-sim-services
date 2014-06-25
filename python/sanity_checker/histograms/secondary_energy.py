import numpy
from math import log10
from icecube import dataclasses
from I3Tray import I3Units
from ..bases.histogram import Histogram
from .utils import bins, event_weight

def _frame_op(frame):
    e = list()
    if "I3MCTree" in frame :
        tree = frame["I3MCTree"]
        for p in tree :
            if (p.type == dataclasses.I3Particle.MuMinus \
                or p.type == dataclasses.I3Particle.MuPlus) \
                and tree.depth(p) > 0 :               
                e.append(log10(p.energy/I3Units.GeV))
    return e

_draw_args = { "bins" : bins(50,0,10),
               "xlabel" : r"$log_{10}(E/GeV)$",
               "title" : "Secondary Energy (mu+,mu-) Spectrum",
               "figname" : "secondary_energy.png",
               "log" : True }

secondary_energy_h = Histogram(frame_op = _frame_op,
                               draw_args = _draw_args,
                               name = 'secondary_energy')
