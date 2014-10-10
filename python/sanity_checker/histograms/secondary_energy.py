import numpy
from math import log10
from icecube import dataclasses
from I3Tray import I3Units
from ..bases.histogram import Histogram
from .utils import bins, event_weight

def _frame_op(frame):
    e = list()
    if "I3MCTree" in frame :
        tree = frame["I3MCTree"]
        for p in tree :
            if (p.type == dataclasses.I3Particle.MuMinus \
                or p.type == dataclasses.I3Particle.MuPlus) \
                and tree.depth(p) > 0 \
                and p.energy > 0 :
                e.append(log10(p.energy/I3Units.GeV))
    return e

_draw_args = { "bins" : bins(50,0,10),
               "xlabel" : r"$log_{10}(E/GeV)$",
               "title" : "Secondary Energy (mu+,mu-) Spectrum",
               "figname" : "secondary_energy.png",
               "log" : True }

def _setup_test(frame):
    t = dataclasses.I3MCTree()
    p0 = dataclasses.I3Particle()
    p0.energy = 0
    p1 = dataclasses.I3Particle()
    p1.energy = -1
    p2 = dataclasses.I3Particle()
    p2.type = dataclasses.I3Particle.MuMinus
    p2.energy = 1

    p3 = dataclasses.I3Particle()
    p3.energy = 1

    t.add_primary(p0)
    t.append_child(p0,p3)

    t.add_primary(p1)
    t.append_child(p1,p2)

    frame["I3MCTree"] = t

def _log10(self, frame):
    print "log10 : self.hist.bincontent.sum() = ", self.hist.bincontent.sum()
    return self.hist.bincontent.sum() == 1
    
secondary_energy_h = Histogram(frame_op = _frame_op,
                               draw_args = _draw_args,
                               name = 'secondary_energy')

secondary_energy_h.add_test("log10", _log10, _setup_test)
