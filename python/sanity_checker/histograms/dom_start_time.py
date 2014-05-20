import numpy
from I3Tray import I3Units
from icecube import dataclasses
from ..bases.histogram import Histogram
from .utils import bins, event_weight

def _frame_op(frame):
    print frame
    if frame.Has("InIceRawData") :
        launches = frame["InIceRawData"]
        weight = event_weight(frame)
        rval = list()
        for k,v in launches :
            rval.extend([(d.time,weight) for d in v])
        return rval
    return []

_draw_args = { "bins" : bins(100,10000/I3Units.ns,12000/I3Units.ns),
               "xlabel" : "t(ns)",
               "title" : "DOM Start Time",
               "figname" : "dom_start_time.png",
               "ylabel" : r"$\Gamma(\rm{Hz})$",
               "log" : True }

dom_start_time_h = Histogram(frame_op = _frame_op,
                             draw_args = _draw_args,
                             name = 'dom_start_time')
