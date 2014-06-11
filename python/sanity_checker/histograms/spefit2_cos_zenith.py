import numpy
from math import cos
from icecube import dataclasses
from ..bases.histogram import Histogram
from .utils import bins, event_weight

def _frame_op(frame):
    if "SPEFit2" in frame :
        weight = event_weight(frame)
        return (cos(frame["SPEFit2"].dir.zenith), weight)

_draw_args = { "bins" : bins(50,-1,1),
               "xlabel" : r"$\cos(\theta)$",
               "title" : "SPEFit2 Cosine Zenith Spectrum",
               "figname" : "spefit2_cos_zenith.png",
               "log" : True }

spefit2_cos_zenith_h = Histogram(frame_op = _frame_op,
                                 draw_args = _draw_args,
                                 name = 'spefit2_cos_zenith')
