import numpy
from icecube import dataclasses
from ..bases.histogram import Histogram
from .utils import bins, event_weight

def _frame_op(frame):
    if "I3MCPESeriesMap" in frame:
        return len(frame["I3MCPESeriesMap"])
    return None

_draw_args = { "bins" : bins(200,0,200),
               "xlabel" : "nchan",
               "title" : "N Channels (MCPE)",
               "figname" : "n_mcpe_chan.png",
               "log" : True }

n_mcpe_chan_h = Histogram(frame_op = _frame_op,
                          draw_args = _draw_args,
                          name = 'n_mcpe_chan')
