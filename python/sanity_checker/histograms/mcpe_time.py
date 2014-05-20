import numpy
from I3Tray import I3Units
from icecube import dataclasses
from ..bases.histogram import Histogram
from .utils import bins, event_weight

def _frame_op(frame):
    if "I3MCPESeriesMap" in frame :
        weight = event_weight(frame)
        t = list()
        for omkey, peseries in frame["I3MCPESeriesMap"] :
            t.extend([pe.time for pe in peseries])
        return t
    return []

_draw_args = { "bins" : bins(100,10000/I3Units.ns,12000/I3Units.ns),
               "xlabel" : "t(ns)",
               "title" : "I3MCPE Time",
               "figname" : "mcpe_time.png",
               "ylabel" : r"$\Gamma(\rm{Hz})$",
               "log" : True }

mcpe_time_h = Histogram(frame_op = _frame_op,
                             draw_args = _draw_args)
