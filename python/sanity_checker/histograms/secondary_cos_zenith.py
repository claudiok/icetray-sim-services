import numpy
from math import cos
from icecube import dataclasses
from I3Tray import I3Units
from ..bases.histogram import Histogram
from .utils import bins

def _frame_op(frame):
    tree = frame["I3MCTree"]
    z = list()
    for p in tree :
        if (p.type == dataclasses.I3Particle.MuMinus \
            or p.type == dataclasses.I3Particle.MuPlus) \
            and tree.depth(p) > 0 :               
            z.append(cos(p.dir.zenith))
    return z

_draw_args = { "bins" : bins(50,0,1),
               "label" : r"$\theta(\rm{deg})$",
               "title" : "Secondary Cosine Zenith (mu+,mu-) Spectrum",
               "figname" : "secondary_cos_zenith.png",
               "log" : True }

secondary_cos_zenith_h = Histogram(frame_op = _frame_op,
                                   draw_args = _draw_args)
