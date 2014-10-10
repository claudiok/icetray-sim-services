import numpy
from I3Tray import I3Units
from icecube import dataclasses
from ..bases.histogram import Histogram
from .utils import bins, event_weight

def _frame_op(frame):
    if "I3MCTree" in frame :
        t = frame["I3MCTree"]
        return [p.energy for p in t if p.is_cascade]                
    return []

_draw_args = { "bins" : bins(100,0,100.*I3Units.GeV),
               "xlabel" : "E(GeV)",
               "title" : "Cascade Energy Spectrum",
               "figname" : "cascade_energy.png",
               "log" : True }

def _setup_test(frame):
    t = dataclasses.I3MCTree()
    for i in range(100) :
        p = dataclasses.I3Particle()
        p.type = dataclasses.I3Particle.EMinus
        p.energy = 1 * I3Units.GeV
        t.add_primary(p)
    frame["I3MCTree"] = t

def _basic(self, frame):
    return self.hist.bincontent.sum() == 100

            
cascade_energy_h = Histogram(frame_op = _frame_op,
                             draw_args = _draw_args,
                             name = 'cascade_energy')

cascade_energy_h.add_test("basic", _basic, _setup_test)
