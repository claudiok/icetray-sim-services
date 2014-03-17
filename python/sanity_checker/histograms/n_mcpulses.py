import numpy
from icecube import dataclasses
from ..bases.histogram import Histogram
from .utils import bins, data_livetime

def _frame_op(frame):
    if "I3MCPulseSeriesMap" in frame :
        return sum([len(v) for k,v in frame["I3MCPulseSeriesMap"]])
    return -1

_draw_args = { "bins" : bins(40,0,40),
               "xlabel" : "N",
               "title" : "N MCPulses",
               "figname" : "n_mcpulses.png",
               "log" : True }

n_mcpulses_h = Histogram(frame_op = _frame_op,
                         draw_args = _draw_args)

