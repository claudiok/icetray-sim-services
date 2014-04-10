import numpy
from icecube import dataclasses
from ..bases.histogram import Histogram
from .utils import bins, data_livetime

def _frame_op(frame):
    if "I3MCPulseSeriesMap" in frame :
        weight = 1./data_livetime(frame)
        return (len(frame["I3MCPulseSeriesMap"]), weight)
    return -1

_draw_args = { "bins" : bins(200,0,200),
               "xlabel" : "nchan",
               "title" : "N Channels (PMT)",
               "figname" : "n_pmt_chan.png",
               "log" : True }

n_pmt_chan_h = Histogram(frame_op = _frame_op,
                         draw_args = _draw_args)