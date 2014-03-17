import numpy
from icecube import dataclasses
from I3Tray import I3Units
from ..bases.histogram import Histogram
from .utils import bins, data_livetime

def _frame_op(frame):
    tree = frame["I3MCTree"]
    z = list()
    for p in tree :
        if (p.type == dataclasses.I3Particle.MuMinus \
            or p.type == dataclasses.I3Particle.MuPlus) \
            and tree.depth(p) > 0 :               
            z.append(p.dir.zenith/I3Units.degree)
    return z

_draw_args = { "bins" : bins(50,0,180),
               "xlabel" : r"$\theta(\rm{deg})$",
               "title" : "Secondary Zenith (mu+,mu-) Spectrum",
               "figname" : "secondary_zenith.png",
               "log" : True }

secondary_zenith_h = Histogram(frame_op = _frame_op,
                               draw_args = _draw_args)
