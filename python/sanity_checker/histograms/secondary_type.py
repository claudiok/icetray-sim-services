import numpy
from icecube import dataclasses
from ..bases.histogram import Histogram

type_to_int_dict = {dataclasses.I3Particle.Gamma : 0,
                    dataclasses.I3Particle.EPlus : 1,
                    dataclasses.I3Particle.EMinus : 2,
                    dataclasses.I3Particle.MuPlus : 3,
                    dataclasses.I3Particle.MuMinus : 4,
                    dataclasses.I3Particle.Pi0 : 5,
                    dataclasses.I3Particle.PiPlus : 6,
                    dataclasses.I3Particle.PiMinus : 7,
                    dataclasses.I3Particle.K0_Long : 8,
                    dataclasses.I3Particle.KPlus : 9,
                    dataclasses.I3Particle.KMinus : 10,
                    dataclasses.I3Particle.Neutron : 11,
                    dataclasses.I3Particle.PPlus : 12,
                    dataclasses.I3Particle.PMinus : 13,
                    dataclasses.I3Particle.K0_Short : 14,
                    dataclasses.I3Particle.NuE : 15,
                    dataclasses.I3Particle.NuEBar : 16,
                    dataclasses.I3Particle.NuMu : 17,
                    dataclasses.I3Particle.NuMuBar : 18,
                    dataclasses.I3Particle.TauPlus : 19,
                    dataclasses.I3Particle.TauMinus : 20,
                    dataclasses.I3Particle.NuTau : 21,
                    dataclasses.I3Particle.NuTauBar : 22,
                    dataclasses.I3Particle.CherenkovPhoton : 23,
                    dataclasses.I3Particle.Brems : 24,
                    dataclasses.I3Particle.DeltaE : 25,
                    dataclasses.I3Particle.PairProd : 26,
                    dataclasses.I3Particle.NuclInt : 27,
                    dataclasses.I3Particle.Hadrons : 28,
                    dataclasses.I3Particle.unknown : 29
                    }

def _frame_op(frame):
    tree = frame["I3MCTree"]
    types = list()
    for p in tree :
        if tree.depth(p) > 0 :
            if p.type in type_to_int_dict :
                types.append(type_to_int_dict[p.type])
            else :
                types.append(29)
    return types

_labels = list()
for i in range(30):
    for k,v in type_to_int_dict.iteritems() :
        if v == i :
            if k == dataclasses.I3Particle.CherenkovPhoton :
                _labels.append("Cherenkov")
            else:
                _labels.append(str(k))

_xticks_args = (range(30),_labels)
_xticks_kwargs = {"rotation" : -45,
                  "size" : "x-small"}

_draw_args = { "bins" : 30,
               "label" : "",
               "title" : "Secondary Type",
               "figname" : "secondary_type.png",
               "log" : True,
               "xticks_args" : _xticks_args,
               "xticks_kwargs" : _xticks_kwargs}

secondary_type_h = Histogram(frame_op = _frame_op,
                             draw_args = _draw_args)


