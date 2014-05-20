import numpy
from icecube import dataclasses
from ..bases.histogram import Histogram
from .utils import bins, event_weight

def _frame_op(frame):
    weight = event_weight(frame)
    nchannel = 0
    if "I3MCPESeriesMap" in frame :
        for dom, mcpeseries in frame["I3MCPESeriesMap"]:
            for mcpe in mcpeseries:
                if(mcpe.major_ID == 0 and mcpe.minor_ID == 0):
                    nchannel +=1
                    break     
        return (nchannel, weight)
    return -1

_draw_args = { "bins" : bins(200,0,200),
               "xlabel" : "nchan",
               "title" : "N Channels Noise (MCPE)",
               "figname" : "n_mcpe_noise_chan.png",
               "log" : True }

n_mcpe_noise_chan_h = Histogram(frame_op = _frame_op,
                            draw_args = _draw_args,
                            name = 'n_mcpe_noise_chan')