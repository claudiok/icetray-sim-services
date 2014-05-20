import numpy
from I3Tray import I3Units
from icecube import dataclasses
from ..bases.histogram import Histogram
from .utils import bins, event_weight

def _frame_op(frame):
    return [p.energy for p in frame["I3MCTree"]\
            if p.is_cascade]

_draw_args = { "bins" : bins(100,0,100.*I3Units.GeV),
               "xlabel" : "E(GeV)",
               "title" : "Cascade Energy Spectrum",
               "figname" : "cascade_energy.png",
               "log" : True }

cascade_energy_h = Histogram(frame_op = _frame_op,
                             draw_args = _draw_args,
                             name = 'cascade_energy')
