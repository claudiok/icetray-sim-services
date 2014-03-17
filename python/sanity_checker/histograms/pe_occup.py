import numpy
from I3Tray import I3Units
from icecube import dataclasses
from ..bases.histogram import Histogram
from .utils import bins, data_livetime

def _frame_op(frame):
    if "I3Geometry" in frame \
       and "I3MCPESeriesMap" in frame :
        weight = 1./data_livetime(frame)
        geo = frame["I3Geometry"].omgeo
        pemap = frame["I3MCPESeriesMap"]
        rval = list()
        for k,v in pemap :
            z = geo[k].position.z
            rval.append((z,len(v)*weight))
        return rval
    return []

_draw_args = { "bins" : bins(110,-550*I3Units.m,550*I3Units.m),
               "xlabel" : "z(m)",
               "title" : "PE Occupancy",
               "figname" : "pe_occup.png",
               "ylabel" : r"$\Gamma(\rm{Hz})$",
               "log" : True }

pe_occup_h = Histogram(frame_op = _frame_op,
                       draw_args = _draw_args)

