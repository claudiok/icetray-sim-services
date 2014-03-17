import numpy
from math import cos
from icecube import dataclasses
from ..bases.histogram import Histogram
from .utils import bins, data_livetime

def _frame_op(frame):
    return [cos(p.energy) for p in frame["I3MCTree"].primaries]    

_draw_args = { "bins" : bins(50,0,1),
               "xlabel" : r"$\cos(\theta)$",
               "title" : "Primary Cosine Zenith Spectrum",
               "figname" : "primary_cos_zenith.png",
               "log" : True }

primary_cos_zenith_h = Histogram(frame_op = _frame_op,
                             draw_args = _draw_args)
