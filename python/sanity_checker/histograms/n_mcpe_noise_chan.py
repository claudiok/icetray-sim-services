import numpy
from icecube import dataclasses
from ..bases.histogram import Histogram
from .utils import bins

def _frame_op(frame):
    if "I3MCPESeriesMap" in frame :
        nchannel = 0
        for dom, mcpeseries in frame["I3MCPESeriesMap"]:
            for mcpe in mcpeseries:
                if(mcpe.ID.majorID == 0 and mcpe.ID.minorID == 0):
                    nchannel +=1
                    break     
        return nchannel
    return None

_draw_args = { "bins" : bins(200,0,200),
               "xlabel" : "nchan",
               "title" : "N Channels Noise (MCPE)",
               "figname" : "n_mcpe_noise_chan.png",
               "log" : True }

n_mcpe_noise_chan_h = Histogram(frame_op = _frame_op,
                            draw_args = _draw_args,
                            name = 'n_mcpe_noise_chan')
