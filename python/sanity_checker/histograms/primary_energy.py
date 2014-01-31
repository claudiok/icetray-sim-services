import numpy
from icecube import dataclasses
from ..bases.histogram import Histogram

def _primary_energy(frame):
    return [p.energy for p in frame["I3MCTree"]]    

_draw_args = { "bins" : numpy.arange(0,10,0.2),
               "label" : r"$log_{10}(E/GeV)$",
               "title" : "Primary Energy",
               "figname" : "primary_energy.png",
               "log" : True }

primary_energy_h = Histogram(frame_op = _primary_energy,
                             draw_args = _draw_args)
