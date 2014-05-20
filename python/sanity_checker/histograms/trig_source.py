import numpy
from icecube import dataclasses
from ..bases.histogram import Histogram
from .utils import event_weight

type_to_int_dict = {dataclasses.I3Trigger.IN_ICE : 0,
                    dataclasses.I3Trigger.ICE_TOP : 1,
                    dataclasses.I3Trigger.AMANDA_TWR_DAQ : 2,
                    dataclasses.I3Trigger.EXTERNAL : 3,
                    dataclasses.I3Trigger.GLOBAL : 4,
                    dataclasses.I3Trigger.AMANDA_MUON_DAQ : 5,
                    dataclasses.I3Trigger.SPASE : 6,
                    dataclasses.I3Trigger.UNKNOWN_SOURCE : 7
                    }

def _frame_op(frame):
    if "I3TriggerHierarchy" in frame :
        weight = event_weight(frame)
        return [(type_to_int_dict[t.key.source], weight) \
                if t.key.source in type_to_int_dict else (7, weight) \
                for t in frame["I3TriggerHierarchy"]]

_labels = list()
for i in range(8):
    for k,v in type_to_int_dict.iteritems() :
        if v == i : _labels.append(str(k))

_xticks_args = (range(8),_labels)
_xticks_kwargs = {"rotation" : -45,
                  "size" : "x-small"}

_draw_args = { "bins" : 8,
               "xlabel" : "",
               "title" : "Trigger Source",
               "figname" : "trig_source.png",
               "ylabel" : r"$\Gamma(\rm{Hz})$",
               "log" : True,
               "xticks_args" : _xticks_args,
               "xticks_kwargs" : _xticks_kwargs
               }

trig_source_h = Histogram(frame_op = _frame_op,
                          draw_args = _draw_args,
                          name = 'trig_source')
