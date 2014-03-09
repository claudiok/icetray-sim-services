import numpy
from icecube import dataclasses
from ..bases.histogram import Histogram
from .utils import bins

def _frame_op(frame):
    if frame.Has("InIceRawData") :
        return len(frame["InIceRawData"])
    return []

_draw_args = { "bins" : bins(200,0,200),
               "label" : "nchan",
               "title" : "N Channels (DOM Launch)",
               "figname" : "n_dom_chan.png",
               "log" : True }

n_dom_chan_h = Histogram(frame_op = _frame_op,
                         draw_args = _draw_args)
