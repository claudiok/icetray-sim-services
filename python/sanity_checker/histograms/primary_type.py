import numpy
from icecube import dataclasses
from ..bases.histogram import Histogram

_type_to_int_dict = {dataclasses.I3Particle.EPlus : 0,
                     dataclasses.I3Particle.EMinus : 1,
                     dataclasses.I3Particle.MuPlus : 2,
                     dataclasses.I3Particle.MuMinus : 3,
                     dataclasses.I3Particle.Neutron : 4,
                     dataclasses.I3Particle.PPlus : 5,
                     dataclasses.I3Particle.PMinus : 6,
                     dataclasses.I3Particle.NuE : 7,
                     dataclasses.I3Particle.NuEBar : 8,
                     dataclasses.I3Particle.TauPlus : 9,
                     dataclasses.I3Particle.TauMinus : 10,
                     dataclasses.I3Particle.NuTau : 11,
                     dataclasses.I3Particle.NuTauBar : 12,
                     dataclasses.I3Particle.He4Nucleus : 13,
                     dataclasses.I3Particle.Li7Nucleus : 14,
                     dataclasses.I3Particle.Be9Nucleus : 15,
                     dataclasses.I3Particle.B11Nucleus : 16,
                     dataclasses.I3Particle.C12Nucleus : 17,
                     dataclasses.I3Particle.N14Nucleus : 18,
                     dataclasses.I3Particle.O16Nucleus : 19,
                     dataclasses.I3Particle.F19Nucleus : 20,
                     dataclasses.I3Particle.Ne20Nucleus : 21,
                     dataclasses.I3Particle.Na23Nucleus : 22,
                     dataclasses.I3Particle.Mg24Nucleus : 23,
                     dataclasses.I3Particle.Al27Nucleus : 24,
                     dataclasses.I3Particle.Si28Nucleus : 25,
                     dataclasses.I3Particle.P31Nucleus : 26,
                     dataclasses.I3Particle.S32Nucleus : 27,
                     dataclasses.I3Particle.Cl35Nucleus : 28,
                     dataclasses.I3Particle.Ar40Nucleus : 29,
                     dataclasses.I3Particle.K39Nucleus : 30,
                     dataclasses.I3Particle.Ca40Nucleus : 31,
                     dataclasses.I3Particle.Sc45Nucleus : 32,
                     dataclasses.I3Particle.Ti48Nucleus : 33,
                     dataclasses.I3Particle.V51Nucleus : 34,
                     dataclasses.I3Particle.Cr52Nucleus : 35,
                     dataclasses.I3Particle.Mn55Nucleus : 36,
                     dataclasses.I3Particle.Fe56Nucleus : 37,
                     dataclasses.I3Particle.Monopole : 38,
                     dataclasses.I3Particle.NuMu : 39,
                     dataclasses.I3Particle.NuMuBar : 40
                     }

def _primary_type(frame):
    return [_type_to_int_dict[p.type] \
            if p.type in _type_to_int_dict else 41\
            for p in frame["I3MCTree"].primaries]
    
_labels = list()
for key in _type_to_int_dict.keys():
    if "Nucleus" in str(key) :
        _labels.append(str(key)[:str(key).find("Nucleus")])
    else:
        _labels.append(str(key))
_xticks_args = (range(42),_labels)

_xticks_kwargs = {"rotation" : -45,
                  "size" : "x-small"}

_draw_args = { "bins" : 42,
               "label" : "",
               "title" : "Primary Type",
               "figname" : "primary_type.png",
               "log" : True,
               "xticks_args" : _xticks_args,
               "xticks_kwargs" : _xticks_kwargs}

primary_type_h = Histogram(frame_op = _primary_type,
                           draw_args = _draw_args)


