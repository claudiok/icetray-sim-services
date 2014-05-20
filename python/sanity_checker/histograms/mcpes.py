import numpy
from icecube import dataclasses
from ..bases.histogram import Histogram
from .utils import bins, event_weight

def _frame_op(frame):
    weight = event_weight(frame)
    if "I3MCPESeriesMap" in frame :
        return sum([len(v) for k,v in frame["I3MCPESeriesMap"]])
    return -1

_draw_args = { "bins" : bins(40,0,40),
               "xlabel" : "N",
               "title" : "N MCPEs",
               "figname" : "mcpes.png",
               "ylabel" : r"$\Gamma(\rm{Hz})$",
               "log" : True }

mcpes_h = Histogram(frame_op = _frame_op,
                    draw_args = _draw_args,
                    name = 'mcpes')

