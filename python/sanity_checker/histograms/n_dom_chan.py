import numpy
from icecube import dataclasses
from ..bases.histogram import Histogram
from .utils import bins, data_livetime

def _frame_op(frame):
    if frame.Has("InIceRawData") :
        weight = 1./data_livetime(frame)
        return (len(frame["InIceRawData"]), weight)
    return []

_draw_args = { "bins" : bins(200,0,200),
               "xlabel" : "nchan",
               "title" : "N Channels (DOM Launch)",
               "figname" : "n_dom_chan.png",
               "ylabel" : r"$\Gamma(\rm{Hz})$",
               "log" : True }

n_dom_chan_h = Histogram(frame_op = _frame_op,
                         draw_args = _draw_args)