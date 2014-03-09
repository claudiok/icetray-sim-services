import numpy
from icecube import dataclasses
from ..bases.histogram import Histogram
from .utils import bins

def _frame_op(frame):
    if "I3MCPESeriesMap" in frame :
        return sum([len(v) for k,v in frame["I3MCPESeriesMap"]])
    return -1

_draw_args = { "bins" : bins(40,0,40),
               "label" : "N",
               "title" : "N MCPEs",
               "figname" : "mcpes.png",
               "log" : True }

mcpes_h = Histogram(frame_op = _frame_op,
                    draw_args = _draw_args)

