import numpy
from math import log10
from icecube import dataclasses
from I3Tray import I3Units
from ..bases.histogram import Histogram
from .utils import bins

def _frame_op(frame):
    if "I3MCTree" in frame :
        return [log10(p.energy/I3Units.GeV) \
                for p in frame["I3MCTree"].primaries\
                if p.energy > 0]
    return []

_draw_args = { "bins" : bins(50,0,10),
               "xlabel" : r"$\log_{10}(\rm{E/GeV})$",
               "title" : "Primary Energy Spectrum",
               "figname" : "primary_energy.png",
               "log" : True}

primary_energy_h = Histogram(frame_op = _frame_op,
                             draw_args = _draw_args,
                             name = "primary_energy")

