import numpy
from icecube import dataclasses
from ..bases.histogram import Histogram
from .utils import bins

def _frame_op(frame):
    if "InIceRawData" in frame :
        return list(set([omkey.string for omkey,d in frame["InIceRawData"]]))
    return []

_draw_args = { "bins" : bins(40,0,40),
               "label" : "n_string",
               "title" : "N Strings (IC)",
               "figname" : "n_string_ic.png",
               "log" : True }

n_string_ic_h = Histogram(frame_op = _frame_op,
                          draw_args = _draw_args)
