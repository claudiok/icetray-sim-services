import numpy
from icecube import dataclasses
from ..bases.histogram import Histogram
from I3Tray import I3Units
from .utils import bins

def _frame_op(frame):
    return [p.dir.zenith/I3Units.degree for p in frame["I3MCTree"].primaries]    

_draw_args = { "bins" : bins(50,0,180),
               "label" : r"$\theta(\rm{deg})$",
               "title" : "Primary Zenith Spectrum",
               "figname" : "primary_zenith.png",
               "log" : True }

primary_zenith_h = Histogram(frame_op = _frame_op,
                             draw_args = _draw_args)
