import numpy
from icecube import dataclasses
from ..bases.histogram import Histogram
from .utils import event_weight

type_to_int_dict = {dataclasses.I3Trigger.SIMPLE_MULTIPLICITY : 0,
                    dataclasses.I3Trigger.CALIBRATION : 1,
                    dataclasses.I3Trigger.MIN_BIAS : 2,
                    dataclasses.I3Trigger.THROUGHPUT : 3,
                    dataclasses.I3Trigger.TWO_COINCIDENCE : 4,
                    dataclasses.I3Trigger.THREE_COINCIDENCE : 5,
                    dataclasses.I3Trigger.MERGED : 6,
                    dataclasses.I3Trigger.SLOW_PARTICLE : 7,
                    dataclasses.I3Trigger.FRAGMENT_MULTIPLICITY : 8,
                    dataclasses.I3Trigger.STRING : 9,
                    dataclasses.I3Trigger.VOLUME : 10,
                    dataclasses.I3Trigger.SPHERE : 11,
                    dataclasses.I3Trigger.UNBIASED : 12,
                    dataclasses.I3Trigger.SPASE_2 : 13,
                    dataclasses.I3Trigger.UNKNOWN_TYPE : 14
                    }

def _frame_op(frame):
    if "I3TriggerHierarchy" in frame :
        weight = event_weight(frame)
        rval = list()
        for t in frame["I3TriggerHierarchy"] :
            if t.key.type == dataclasses.I3Trigger.IN_ICE :
                rval.extend([ (1 + type_to_int_dict[t.key.type], weight) \
                if t.key.type in type_to_int_dict else (14,weight) ])
        return rval
    return []

_labels = list()
for i in range(15):
    for k,v in type_to_int_dict.iteritems() :
        if v == i : _labels.append(str(k))

_xticks_args = (range(15),_labels)
_xticks_kwargs = {"rotation" : -45,
                  "size" : "x-small"}

_draw_args = { "bins" : 15,
               "xlabel" : "",
               "title" : "Trigger Source",
               "figname" : "trig_ICtypeid.png",
               "ylabel" : r"$\Gamma(\rm{Hz})$",
               "log" : True,
               "xticks_args" : _xticks_args,
               "xticks_kwargs" : _xticks_kwargs
               }

trig_ICtypeid_h = Histogram(frame_op = _frame_op,
                            draw_args = _draw_args)
