import numpy
from icecube import dataclasses
from ..bases.histogram import Histogram
from .utils import bins, data_livetime

def _frame_op(frame):
    if "InIceRawData" in frame :
        weight = 1./data_livetime(frame)
        strings = list(set([omkey.string for omkey,d in frame["InIceRawData"]]))
        return [(s,weight) for s in strings]
    return []

_draw_args = { "bins" : bins(40,0,40),
               "xlabel" : "n_string",
               "title" : "N Strings (IC)",
               "figname" : "n_string_ic.png",
               "ylabel" : r"$\Gamma(\rm{Hz})$",
               "log" : True }

n_string_ic_h = Histogram(frame_op = _frame_op,
                          draw_args = _draw_args)
