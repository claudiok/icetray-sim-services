import numpy
from icecube import dataclasses
from ..bases.histogram import Histogram
from .utils import bins, data_livetime

def _frame_op(frame):
    weight = 1./data_livetime(frame)
    if "I3MCPESeriesMap" in frame :
        return (len(frame["I3MCPESeriesMap"]), weight)
    return -1

_draw_args = { "bins" : bins(200,0,200),
               "xlabel" : "nchan",
               "title" : "N Channels (MCPE)",
               "figname" : "n_mcpe_chan.png",
               "log" : True }

n_mcpe_chan_h = Histogram(frame_op = _frame_op,
                          draw_args = _draw_args)
