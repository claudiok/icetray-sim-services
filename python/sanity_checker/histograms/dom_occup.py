import numpy
from I3Tray import I3Units
from icecube import dataclasses
from ..bases.histogram import Histogram
from .utils import bins, data_livetime, event_weight

def _frame_op(frame):
    if "I3Geometry" in frame \
       and "InIceRawData" in frame :
        weight = event_weight(frame)
        geo = frame["I3Geometry"].omgeo
        launches = frame["InIceRawData"]
        rval = list()
        for k,v in launches :
            z = geo[k].position.z
            rval.append((z,weight*len(v)))
        return rval
    return []

_draw_args = { "bins" : bins(110,-550*I3Units.m,550*I3Units.m),
               "xlabel" : "z(m)",
               "title" : "DOM Occupancy",
               "figname" : "dom_occup.png",
               "ylabel" : r"$\Gamma(\rm{Hz})$",
               "log" : True }

dom_occup_h = Histogram(frame_op = _frame_op,
                        draw_args = _draw_args)
