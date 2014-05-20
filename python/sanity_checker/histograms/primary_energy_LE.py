import numpy
from math import log10
from icecube import dataclasses
from I3Tray import I3Units
from ..bases.histogram import Histogram
from .utils import bins

def _frame_op(mctype, frame) :

    part = dataclasses.I3Particle()
    tree = frame["I3MCTree"]
    
    if corsika and tree.most_energetic_primary :
            part =dataclasses.I3Particle(tree.most_energetic_primary)
#            most_energetic_primary = part1.energy

    if neutrino and tree.most_energetic_neutrino :
            part =dataclasses.I3Particle(tree.most_energetic_neutrino)
#            most_energetic_primary = part2.energy

    return [log10(part.energy/I3Units.GeV) \
            part.energy]



def _setup_test(frame):
    mctree = dataclasses.I3MCTree()
    mctree.add_primary( dataclasses.I3Particle() )
    frame["I3MCTree"] = mctree

    primary_energy_h = Histogram(frame_op = _frame_op, \
                                 draw_args = _draw_args, \
                                 setup_test = _setup_test)

    lambda existence = lamdba frame : frame.Has("I3MCTree")

    def one_primary(frame) :
        return len(frame["I3MCTree"]) == 1

    primary_energy_h.add_test(existence)
    primary_energy_h.add_test(one_primary)

_draw_args = { "bins" : bins(50,0,10),
               "xlabel" : r"$\log_{10}(\rm{E/GeV})$",
               "title" : "Primary Energy Spectrum",
               "figname" : "primary_energy.png",
               "log" : True}

primary_energy_h = Histogram(frame_op = _frame_op,
                             draw_args = _draw_args,
                             name = "primary_energy_LE")
